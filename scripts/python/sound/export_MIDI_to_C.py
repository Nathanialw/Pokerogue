import pretty_midi
import os
import glob

TICKS_PER_QUARTER = 16
MAX_VOICES = 4
DEFAULT_BPM = 120

ARTICULATION_NONE = 0
ARTICULATION_STACCATO = 1
ARTICULATION_LEGATO = 2

INSTRUMENT_MAP = {0: 0, 40: 0, 41: 0, 42: 0}


class CNote:
    def __init__(self, note, rest, duration, instrument, volume, articulation):
        self.note = note
        self.rest = rest
        self.duration = duration
        self.instrument = instrument
        self.volume = volume
        self.articulation = articulation


class CVoice:
    def __init__(self, notes):
        self.notes = notes


class CSong:
    def __init__(self, name, voices):
        self.name = name
        self.voices = voices
        self.total_notes = sum(len(v.notes) for v in voices)


def load_midi(filename):
    midi = pretty_midi.PrettyMIDI(filename)
    global bpm

    tempo_times, tempos = midi.get_tempo_changes()
    if len(tempos) > 0:
        bpm = int(round(tempos[0]))
        print(f"  Using MIDI tempo: {bpm}")
    else:
        bpm = int(midi.estimate_tempo() or DEFAULT_BPM)
        print(f"  Estimated/fallback tempo: {bpm}")

    quarter = 60.0 / bpm
    tick_time = quarter / TICKS_PER_QUARTER
    print(f"  Tick time: {tick_time:.6f} s")
    return midi, tick_time


def quantize_ticks(ticks):
    if ticks < 1:
        return 1
    return max(1, round(ticks))


def separate_into_voices(midi, tick_time):
    """
    Take all notes from MIDI and separate them into MAX_VOICES monophonic streams
    """
    # Collect all notes with their timing
    all_notes = []
    for instrument in midi.instruments:
        if not instrument.notes:
            continue
        inst_id = INSTRUMENT_MAP.get(instrument.program, 0)
        for note in instrument.notes:
            all_notes.append({
                'pitch': note.pitch,
                'start': note.start,
                'end': note.end,
                'velocity': note.velocity,
                'instrument': inst_id
            })

    if not all_notes:
        return []

    # Sort by start time
    all_notes.sort(key=lambda x: x['start'])

    # Initialize voices (each will be a list of notes)
    voice_notes = [[] for _ in range(MAX_VOICES)]
    voice_end_times = [0.0] * MAX_VOICES

    # For tracking active notes per voice (to detect polyphony)
    active_notes = [[] for _ in range(MAX_VOICES)]

    for note in all_notes:
        # Find the best voice for this note
        best_voice = -1

        # First, try to find a voice that's completely free at this time
        for v in range(MAX_VOICES):
            if voice_end_times[v] <= note['start'] + 0.01:
                best_voice = v
                break

        # If no voice is free, find the voice that ends earliest
        if best_voice == -1:
            best_voice = voice_end_times.index(min(voice_end_times))

        # Add to this voice
        v = best_voice
        current_voice_notes = voice_notes[v]

        # Add rest if needed (only if there's a significant gap)
        if voice_end_times[v] < note['start'] - 0.01:
            rest_sec = note['start'] - voice_end_times[v]
            rest_ticks = max(1, int(round(rest_sec / tick_time)))

            # Split rests if too long
            rest_remaining = rest_ticks
            while rest_remaining > 0:
                chunk = min(rest_remaining, 16)
                current_voice_notes.append(CNote(
                    note=0,
                    rest=1,
                    duration=chunk - 1,
                    instrument=note['instrument'],
                    volume=3,
                    articulation=ARTICULATION_NONE
                ))
                rest_remaining -= chunk

        # Calculate note duration in ticks
        dur_sec = note['end'] - note['start']
        dur_ticks = max(1, int(round(dur_sec / tick_time)))

        # Split long notes (max 16 ticks per note)
        remaining = dur_ticks
        first_chunk = True
        while remaining > 0:
            chunk = min(remaining, 16)
            current_voice_notes.append(CNote(
                note=note['pitch'] + 7,  # Adjust to your note range
                rest=0,
                duration=chunk - 1,
                instrument=note['instrument'],
                # volume=min(7, note['velocity'] // 16),
                volume=min(7, note['velocity'] // 16),
                articulation=ARTICULATION_NONE
            ))
            remaining -= chunk
            first_chunk = False

        voice_end_times[v] = note['end']

    # Create voice objects, filtering out empty ones
    result = []
    for notes in voice_notes:
        if notes:  # Only include voices that have notes
            result.append(CVoice(notes))

    # Pad to exactly MAX_VOICES (with empty voices if needed)
    while len(result) < MAX_VOICES:
        result.append(CVoice([]))

    return result[:MAX_VOICES]


def process_midi_file(filepath):
    try:
        print(f"Processing: {os.path.basename(filepath)}")
        midi, tick_time = load_midi(filepath)
        name = os.path.splitext(os.path.basename(filepath))[0]

        voices = separate_into_voices(midi, tick_time)

        # Count non-empty voices for reporting
        non_empty = [v for v in voices if v.notes]
        print(f"  Separated into {len(non_empty)} voices with notes")
        for i, v in enumerate(non_empty):
            print(f"    Voice {i}: {len(v.notes)} notes")

        return CSong(name, voices)

    except Exception as e:
        print(f"  ERROR processing {filepath}: {e}")
        return None


def export_all_songs(songs, notes_filename, metadata_filename):
    all_notes = []
    current_offset = 0

    for song in songs:
        for voice in song.voices:
            all_notes.extend(voice.notes)
            current_offset += len(voice.notes)

    with open(notes_filename, 'w') as f:
        f.write("// Auto-generated notes array from MIDI files\n\n")
        for n in all_notes:
            f.write(f"{{ {n.note}, {n.rest}, {n.duration},  {n.instrument}, {n.articulation}, {n.volume}}},\n")
        f.write(f"\n//TOTAL_MUSIC_NOTES = {len(all_notes)}\n")

    with open(metadata_filename, 'w') as f:
        f.write("// Auto-generated music metadata from MIDI files\n\n")
        note_offset = 0
        for song_idx, song in enumerate(songs):
            f.write(f"// {song.name} - {song.total_notes} notes\n")
            f.write("{ ")
            f.write(f".idx = {song_idx}, ")
            f.write(f".BPM = {bpm}, ")
            f.write(f".numVoices = {len(song.voices)}, ")
            f.write(".offset = { ")
            for v in song.voices:
                f.write(f"{note_offset},")
                note_offset += len(v.notes)
            for _ in range(len(song.voices), MAX_VOICES):
                f.write("0,")
            f.write("}, ")
            f.write(".length = {")
            for v in song.voices:
                f.write(f"{len(v.notes)},")
            for _ in range(len(song.voices), MAX_VOICES):
                f.write("0,")
            f.write("}, ")
            f.write(".delay = {0,0,0,0} ")
            f.write("},\n")

        f.write(f"\n// Total notes across all songs: {len(all_notes)}\n")

    print(f"\nExported {len(songs)} songs:")
    print(f"  Notes: {notes_filename} ({len(all_notes)} notes)")
    print(f"  Metadata: {metadata_filename}")


def main():
    midi_folder = "../../../assets_raw/music"
    notes_output = "../../../src/GameCore/inc/sounds_music.inc"
    metadata_output = "../../../src/GameCore/inc/sounds_music_data.inc"

    midi_files = glob.glob(os.path.join(midi_folder, "*.mid")) + \
                 glob.glob(os.path.join(midi_folder, "*.midi"))

    if not midi_files:
        print(f"No MIDI files found in '{midi_folder}'")
        return

    print(f"Found {len(midi_files)} MIDI files")
    for f in midi_files:
        print(f"  {os.path.basename(f)}")

    songs = []
    for filepath in sorted(midi_files):
        song = process_midi_file(filepath)
        if song:
            songs.append(song)

    if songs:
        os.makedirs(os.path.dirname(notes_output), exist_ok=True)
        export_all_songs(songs, notes_output, metadata_output)
        print("\nDone!")
    else:
        print("No valid songs found!")


if __name__ == "__main__":
    main()
