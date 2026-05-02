// //
// // Created by nathanial on 3/14/26.
// //

#include "pico/stdlib.h"
#include "pico/audio_i2s.h"
#include "hardware/adc.h"

#include <stdint.h>
#include <string.h>

#include "sounds.h"
#include "hardware.h"

#include "pico_constants.h"
#include "lib_types.h"
#include "lib_enums.h"
// #include "memory_rom.h"
// #include "memory_ram.h"
// #include "sound.h"


#define SAMPLE_RATE 44100
static struct audio_buffer_pool* ap;

static audio_format_t audio_format =
{
    .sample_freq = SAMPLE_RATE,
    .format = AUDIO_BUFFER_FORMAT_PCM_S16,
    .channel_count = 2
};


/**********************************************************************************************************************/
/**  Runs the main init of the sound system
**********************************************************************************************************************/
void Pico_AudioInit(void)
{
    //DEBUG("Initializing audio at %d Hz", SAMPLE_RATE);

    struct audio_buffer_format producer_format = {
        .format = &audio_format,
        .sample_stride = 4
    };

    ap = audio_new_producer_pool(&producer_format, 4, 512); // larger buffers consume quite a bit more memory

    if (!ap)
    {
        //DEBUG("Failed to create audio pool");
        while (1) sleep_ms(1000);
    }

    struct audio_i2s_config config = {
        .data_pin = DIN,
        .clock_pin_base = BCLK,
    };

    const struct audio_format* out = audio_i2s_setup(&audio_format, &config);
    if (!out)
    {
        //DEBUG("audio_i2s_setup failed");
        while (1) sleep_ms(1000);
    }

    if (!audio_i2s_connect(ap))
    {
        //DEBUG("audio_i2s_connect failed");
        while (1) sleep_ms(1000);
    }

    audio_i2s_set_enabled(true);
    //DEBUG("Audio initialized successfully");
}

/**********************************************************************************************************************/
/*
 *  compressed WAV files
 *
**********************************************************************************************************************/

/**********************************************************************************************************************/
/**  TODO - currently unused, wav files are way too big
**********************************************************************************************************************/
const unsigned int SoundWavLen = 60620;
uint16_t SoundWav[100];


void AudioPlaySamples(const int16_t* data, int sample_count)
{
    int pos = 0;

    while (pos < sample_count)
    {
        struct audio_buffer* buffer = take_audio_buffer(ap, true);
        int16_t* samples = (int16_t*)buffer->buffer->bytes;

        int count = buffer->max_sample_count / 2;
        if (count > sample_count - pos)
            count = sample_count - pos;

        for (int i = 0; i < count; i++)
        {
            int16_t sample = data[pos + i];

            samples[i * 2] = sample;
            samples[i * 2 + 1] = sample;
        }

        static int32_t filter = 0;

        for (int i = 0; i < count; i++)
        {
            int16_t sample = data[pos + i];

            // simple low-pass filter
            filter += (sample - filter) >> 3;

            int16_t smooth = filter;

            samples[i * 2] = smooth;
            samples[i * 2 + 1] = smooth;
        }

        buffer->sample_count = count;
        give_audio_buffer(ap, buffer);

        pos += count;
    }
}

/**********************************************************************************************************************/
/**  TODO - currently unsused, wav files are way too big
**********************************************************************************************************************/
void Play()
{
    AudioPlaySamples((int16_t*)(SoundWav + 44), (SoundWavLen - 44) / 2);
}

/**********************************************************************************************************************/
/*
 *  SOUND PROCESSING
 *
**********************************************************************************************************************/


/**********************************************************************************************************************/
/**  Footsteps sound effects values
**********************************************************************************************************************/
const int16_t FootstepSamples[] = {
    0, // silence at start
    1000, // beginning of footstep
    5000, // peak of sound
    8000, // loudest part
    6000, // trailing off
    3000,
    1000,
    500,
    200,
    0, // back to silence
};

#define FOOTSTEP_SAMPLE_COUNT (sizeof(FootstepSamples) / sizeof(int16_t))
volatile bool EffectPlaying = false;
volatile uint32_t EffectSamplePos = 0;
volatile uint32_t EffectTotalSamples = 0;
volatile const int16_t* EffectData = NULL;

/**********************************************************************************************************************/
/**  Plays the footsteps sound effect
 *  TODO - this is a proof of concept, generalise to use all sound effects the game will need
**********************************************************************************************************************/
void PlaySoundEffect()
{
    EffectData = FootstepSamples;
    EffectTotalSamples = FOOTSTEP_SAMPLE_COUNT;
    EffectSamplePos = 0;
    EffectPlaying = true;
}

void CheckVolume()
{
    static int16_t last_raw = 0;
    int16_t raw = Pico_VolumeControl();
    // //DEBUG("Raw: %d", raw);
    if (raw < 0)
    {
        RecoverADS1115();
        raw = last_raw;
    }
    else
    {
        last_raw = raw;
    }

    // g_run.music.master_volume = (MAX_VOLUME - raw) * 2;
    // //DEBUG("Volume: %d %d", raw, g_run.music.master_volume);
}


/**********************************************************************************************************************/
/**  Music Notes const data
**********************************************************************************************************************/
#define NOTE_REST 127
#define BPM 30
#define TICK_MS ((60 * 1000) / (BPM * 16))  // = 31.25
#define FIXED_SHIFT 16
#define FIXED_ONE (1 << 16)
#define MAX_AMPLITUDE 30000  // Leave headroom to prevent clipping
#define VIOLIN_FILTER 2048   // ≈ 0.03125 in Q16.16


typedef struct
{
    int32_t phase; // Q16.16: 0 to FIXED_ONE
    int32_t step; // Q16.16: frequency / SAMPLE_RATE
} Osc;

/**********************************************************************************************************************/
/**  Music sound types
 *  Voice state - holds all runtime data for a voice
**********************************************************************************************************************/
typedef struct
{
    Osc osc1;
    Osc osc2;

    int32_t target_step1;
    int32_t target_step2;

    int32_t env;
    int32_t filter;

    uint8_t current_instrument;
    uint8_t current_articulation;

    int32_t crossfade_counter;
    int32_t prev_sample;
} VoiceState;

/**********************************************************************************************************************/
/**  voide data arrays
**********************************************************************************************************************/
static VoiceState SynthVoices[MAX_VOICES];

/**********************************************************************************************************************/
/**  Oscillator helper (pure, no articulation or instrument specifics)
**********************************************************************************************************************/
static int32_t OscNext(Osc* o)
{
    o->phase += o->step;
    if (o->phase >= FIXED_ONE)
        o->phase -= FIXED_ONE;
    return (o->phase << 1) - FIXED_ONE;
}

/**********************************************************************************************************************/
/**  Instrument implementations (pure sound generation)
**********************************************************************************************************************/
typedef struct
{
    void (*note_start)(VoiceState* v, int32_t freq_fixed);
    int16_t (*sample)(VoiceState* v);
} Instrument;

/**********************************************************************************************************************/
/**   Violin implementation
**********************************************************************************************************************/
static void ViolinNoteStart(VoiceState* v, int32_t freq_fixed)
{
    int64_t f1 = freq_fixed;
    int64_t f2 = (freq_fixed * 1005) / 1000;

    v->target_step1 = (int32_t)((f1 << FIXED_SHIFT) / (100 * SAMPLE_RATE));
    v->target_step2 = (int32_t)((f2 << FIXED_SHIFT) / (100 * SAMPLE_RATE));

    if (v->current_articulation == ARTICULATION_STACCATO)
    {
        v->osc1.step = v->target_step1;
        v->osc2.step = v->target_step2;

        v->osc1.phase = 0;
        v->osc2.phase = 0;
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
static int16_t ViolinSample(VoiceState* v)
{
    // smooth glide toward target pitch
    v->osc1.step += (v->target_step1 - v->osc1.step) >> 4;
    v->osc2.step += (v->target_step2 - v->osc2.step) >> 4;

    int32_t s1 = OscNext(&v->osc1);
    int32_t s2 = OscNext(&v->osc2);

    // Mix oscillators with proper scaling
    int32_t mix = (s1 * 3 + s2) >> 2; // This stays within ±32767 range

    // Apply envelope with headroom
    mix = (int32_t)(((int64_t)mix * v->env) >> FIXED_SHIFT);

    // Apply filter with scaling to prevent overflow
    v->filter += (int32_t)(((int64_t)(mix - v->filter) * VIOLIN_FILTER) >> FIXED_SHIFT);

    // Scale final output to leave headroom for mixing
    return (int16_t)((v->filter * MAX_AMPLITUDE) >> FIXED_SHIFT);
}

/**********************************************************************************************************************/
/**  Piano implementation
**********************************************************************************************************************/
static void PianoNoteStart(VoiceState* v, int32_t freq_fixed)
{
    int64_t f1 = freq_fixed;

    // Piano has a more pure tone - just one oscillator with some harmonics
    v->target_step1 = (int32_t)((f1 << FIXED_SHIFT) / (100 * SAMPLE_RATE));

    // Add a second oscillator at a higher octave for brightness
    int64_t f2 = freq_fixed * 2; // Octave higher
    v->target_step2 = (int32_t)((f2 << FIXED_SHIFT) / (100 * SAMPLE_RATE));

    if (v->current_articulation == ARTICULATION_STACCATO)
    {
        v->osc1.step = v->target_step1;
        v->osc2.step = v->target_step2;
        v->osc1.phase = 0;
        v->osc2.phase = 0;
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
static int16_t PianoSample(VoiceState* v)
{
    // Faster pitch glide for piano (less portamento)
    v->osc1.step += (v->target_step1 - v->osc1.step) >> 6;
    v->osc2.step += (v->target_step2 - v->osc2.step) >> 6;

    int32_t s1 = OscNext(&v->osc1);
    int32_t s2 = OscNext(&v->osc2);

    // Mix with less second oscillator (just for brightness)
    int32_t mix = (s1 * 3 + s2) >> 2; // 75% fundamental, 25% octave

    // Apply envelope
    mix = (int32_t)(((int64_t)mix * v->env) >> FIXED_SHIFT);

    // Piano has a brighter sound (less filtering)
    // v->filter += (int32_t)(((int64_t)(mix - v->filter) * 3277) >> FIXED_SHIFT); // Less filtering

    // For now, skip filter for cleaner piano sound
    return (int16_t)((mix * 30000) >> FIXED_SHIFT); // Match violin's volume
}

/**********************************************************************************************************************/
/**  Instrument table
**********************************************************************************************************************/
static const Instrument InstrumentSound[4] = {
    [VIOLIN] = {.note_start = ViolinNoteStart, .sample = ViolinSample},
    [PIANO] = {.note_start = PianoNoteStart, .sample = PianoSample},
    [FLUTE] = {.note_start = NULL, .sample = NULL},
    [DRUM] = {.note_start = NULL, .sample = NULL},
};

/**********************************************************************************************************************/
/**  Articulation implementations (envelope control)
**********************************************************************************************************************/
typedef struct
{
    void (*note_start)(VoiceState* v);
    int16_t (*process)(VoiceState* v, int32_t samples_into_note, int32_t total_samples, int16_t instrument_sample);
} ArticulationHandler;

/**********************************************************************************************************************/
/**  Staccato articulation
**********************************************************************************************************************/
static void StaccatoNoteStart(VoiceState* v)
{
    v->env = 0; // Reset envelope
}

/**********************************************************************************************************************/
/**  Staccato articulation
**********************************************************************************************************************/
static int16_t StaccatoProcess(VoiceState* v, int32_t samples_into_note, int32_t total_samples, int16_t instrument_sample)
{
    // Only play first 40% of note
    int32_t play_until = (total_samples * 40) / 100;
    if (samples_into_note >= play_until)
    {
        return 0;
    }

    // Fast attack
    if (v->env < FIXED_ONE)
    {
        v->env += 4000;
        if (v->env > FIXED_ONE) v->env = FIXED_ONE;
    }

    // Apply envelope to the instrument sample
    return (int16_t)(((int64_t)instrument_sample * v->env) >> FIXED_SHIFT);
}

/**********************************************************************************************************************/
/**  Legato articulation
**********************************************************************************************************************/
static void LegatoNoteStart(VoiceState* v)
{
    v->env = FIXED_ONE; // keep envelope open
}

/**********************************************************************************************************************/
/** Legato articulation
**********************************************************************************************************************/
static int16_t LegatoProcess(VoiceState* v, int32_t samples_into_note, int32_t total_samples, int16_t instrument_sample)
{
    return instrument_sample;
}

/**********************************************************************************************************************/
/** Standard articulation
**********************************************************************************************************************/
static void NoneNoteStart(VoiceState* v)
{
    v->env = FIXED_ONE;
}

/**********************************************************************************************************************/
/**  Standard articulation
**********************************************************************************************************************/
static int16_t NoneProcess(VoiceState* v, int32_t samples_into_note, int32_t total_samples, int16_t instrument_sample)
{
    // play most of the note but leave a small gap
    int32_t play_until = (total_samples * 85) / 100;

    if (samples_into_note >= play_until)
        return 0;

    return instrument_sample;
}

/**********************************************************************************************************************/
/**  Articulation table
**********************************************************************************************************************/
static const ArticulationHandler Articulations[3] = {
    [ARTICULATION_NONE] = {.note_start = NoneNoteStart, .process = NoneProcess},
    [ARTICULATION_STACCATO] = {.note_start = StaccatoNoteStart, .process = StaccatoProcess},
    [ARTICULATION_LEGATO] = {.note_start = LegatoNoteStart, .process = LegatoProcess},
};

/**********************************************************************************************************************/
/**  Generate the music sample
**********************************************************************************************************************/
int16_t GetMusicSample(int active, int32_t mix)
{
    int16_t music_sample = 0;
    if (active > 0)
    {
        // Apply master volume reduction based on number of active voices
        int32_t master_scale = FIXED_ONE;
        if (active > 1)
        {
            // Reduce volume when multiple voices are active to prevent clipping
            master_scale = (FIXED_ONE * 8) / (10 * active); // 0.8/active scaling
        }

        mix = (int32_t)(((int64_t)mix * master_scale) >> FIXED_SHIFT);

        // Soft clipping instead of hard clipping
        if (mix > 30000) mix = 30000 + ((mix - 30000) >> 2);
        if (mix < -30000) mix = -30000 - ((mix + 30000) >> 2);

        // Final hard limit
        if (mix > 32767) mix = 32767;
        if (mix < -32768) mix = -32768;

        music_sample = (int16_t)mix;
    }

    return music_sample;
}

/**********************************************************************************************************************/
/**  Mix effects sound with the music output
**********************************************************************************************************************/
int16_t MixEffects(int16_t final_sample)
{
    if (EffectPlaying && EffectData != NULL)
    {
        if (EffectSamplePos < EffectTotalSamples)
        {
            int source_index = (EffectSamplePos * FOOTSTEP_SAMPLE_COUNT) / EffectTotalSamples;
            int16_t effect_sample = EffectData[source_index];

            // Better mixing with proper scaling
            int32_t mixed = (final_sample * 3 / 4) + (effect_sample * 1 / 4);

            // Apply soft clipping
            if (mixed > 30000) mixed = 30000 + ((mixed - 30000) >> 2);
            if (mixed < -30000) mixed = -30000 - ((mixed + 30000) >> 2);

            if (mixed > 32767) mixed = 32767;
            if (mixed < -32768) mixed = -32768;

            final_sample = (int16_t)mixed;
            EffectTotalSamples++;
        }
    }
    return final_sample;
}

/**********************************************************************************************************************/
/**  Main playback engine
**********************************************************************************************************************/
void AudioPlayVoices(const MusicData* music_data, const Note* music)
{
    //DEBUG("Starting audio_play_voices with %d voices", music_data->numVoices);

    int voice_count = music_data->numVoices;

    memset(SynthVoices, 0, sizeof(SynthVoices));
    for (int v = 0; v < MAX_VOICES; v++)
    {
        SynthVoices[v].filter = 0;
    }

    int idx[MAX_VOICES] = {0};
    int samples_left[MAX_VOICES] = {0};
    int samples_into_note[MAX_VOICES] = {0};
    int volume[MAX_VOICES];
    int delay_samples[MAX_VOICES];
    int total_samples_note[MAX_VOICES] = {0};

    // Arrays to hold voice info
    const Note* voice_notes[MAX_VOICES];
    int voice_lengths[MAX_VOICES];

    for (int v = 0; v < voice_count; v++)
    {
        // voice_notes[v] = &g_gameFlash.sounds.music.songs[music_data->offset[v]];
        voice_notes[v] = &music[music_data->offset[v]];
        voice_lengths[v] = music_data->length[v];

        volume[v] = 4;
        int delay_ms = (music_data->delay[v] * TICK_MS);
        delay_samples[v] = (delay_ms * SAMPLE_RATE) / 1000;
        if (delay_samples[v] > 10 * SAMPLE_RATE)
        {
            //DEBUG("Capping excessive delay for voice %d", v);
            delay_samples[v] = 0;
        }
    }

    int total_samples_processed = 0;

    while (1)
    {
        int running = 0;
        for (int v = 0; v < voice_count; v++)
        {
            if (delay_samples[v] > 0 || idx[v] < voice_lengths[v] || samples_left[v] > 0)
            {
                running = 1;
                break;
            }
        }
        if (!running)
        {
            //DEBUG("All voices finished after %d samples", total_samples_processed);
            break;
        }

        struct audio_buffer* buffer = take_audio_buffer(ap, false);
        if (!buffer)
        {
            sleep_ms(1);
            continue;
        }

        int16_t* samples = (int16_t*)buffer->buffer->bytes;
        int count = buffer->max_sample_count;
        if (count > 1024) count = 1024;
        memset(samples, 0, count * 2 * sizeof(int16_t));

        for (int i = 0; i < count; i++)
        {
            int32_t mix = 0;
            int active = 0;

            for (int v = 0; v < voice_count; v++)
            {
                if (delay_samples[v] > 0)
                {
                    delay_samples[v]--;
                    continue;
                }

                if (samples_left[v] <= 0 && idx[v] < voice_lengths[v])
                {
                    Note n = voice_notes[v][idx[v]++];

                    if (n.duration > 15)
                    {
                        //DEBUG("Invalid duration %d at voice %d index %d", n.duration, v, idx[v]-1);
                        continue;
                    }

                    int duration_ms = (n.duration + 1) * TICK_MS;
                    samples_into_note[v] = 0;

                    if (n.rest)
                    {
                        SynthVoices[v].target_step1 = 0;
                        SynthVoices[v].target_step2 = 0;
                        SynthVoices[v].osc1.step = 0;
                        SynthVoices[v].osc2.step = 0;
                        volume[v] = 0;
                    }
                    else
                    {
                        if (n.note > 127)
                        {
                            //DEBUG("Invalid note %d at voice %d", n.note, v);
                            continue;
                        }

                        int32_t freq = 0;//g_gameFlash.noteFreqFixed[n.note];

                        SynthVoices[v].current_instrument = n.instrument;
                        SynthVoices[v].current_articulation = n.articulation;

                        if (n.instrument < 4 && InstrumentSound[n.instrument].note_start)
                        {
                            InstrumentSound[n.instrument].note_start(&SynthVoices[v], freq);
                        }

                        if (n.articulation < 3 && Articulations[n.articulation].note_start)
                        {
                            Articulations[n.articulation].note_start(&SynthVoices[v]);
                        }

                        volume[v] = n.volume;
                    }

                    samples_left[v] = (duration_ms * SAMPLE_RATE) / 1000;
                    total_samples_note[v] = samples_left[v];
                    if (samples_left[v] > 1000000)
                    {
                        //DEBUG("Voice %d sample count %d too large!", v, samples_left[v]);
                        samples_left[v] = 1000;
                    }
                }

                if (samples_left[v] > 0)
                {
                    int16_t instrument_sample = 0;
                    int16_t final_sample = 0;

                    uint8_t inst = SynthVoices[v].current_instrument;
                    if (inst < 4 && InstrumentSound[inst].sample)
                    {
                        instrument_sample = InstrumentSound[inst].sample(&SynthVoices[v]);
                    }

                    uint8_t art = SynthVoices[v].current_articulation;
                    if (art < 3 && Articulations[art].process)
                    {
                        final_sample = Articulations[art].process(
                            &SynthVoices[v],
                            samples_into_note[v],
                            total_samples_note[v],
                            instrument_sample
                        );
                    }
                    else
                    {
                        final_sample = instrument_sample;
                    }

                    if ((i % 1000) == 0)
                    {
                        // if (g_run.music.w)
                        //     CheckVolume();
                        // else
                        //     UpdateBacklight();
                        // g_run.music.w = !g_run.music.w;
                    }

                    // int32_t scaled_sample = (int32_t)(((int64_t)final_sample * g_run.music.master_volume) >> FIXED_SHIFT);
                    int32_t scaled_sample = 0;
                    mix += scaled_sample;

                    samples_left[v]--;
                    samples_into_note[v]++;
                    active++;
                }
            }

            // Generate the music sample
            int16_t final_sample = GetMusicSample(active, mix);
            // mix effects
            final_sample = MixEffects(final_sample);

            // Output to both stereo channels
            samples[i * 2] = final_sample;
            samples[i * 2 + 1] = final_sample;
        }

        buffer->sample_count = count;
        give_audio_buffer(ap, buffer);
        total_samples_processed += count;
        tight_loop_contents();
    }

    //DEBUG("Playback complete");
}


/**********************************************************************************************************************/
/** Music type
**********************************************************************************************************************/
typedef enum
{
    BACKGROUND_MUSIC = 0
} Music;
