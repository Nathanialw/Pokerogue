import requests
import time
import re
import os
from pathlib import Path

from python.data import creature_descriptors, _object_img_data
from python.data import _trainer_img_data
from python.data import _creature_img_data
from python.data import _item_img_data
from python.data import _skill_img_data
from python.data import _spell_img_data
from python.data import db_manager
from python.config import constants

from . import validation

# Model identification - Using the Mistral 119B model
# Note: This is a split model, so we point to the first split file
MODEL_NAME = constants.MODEL_NAMES["mistral"]  # Use constant
TEST_MODE = False

# For now, using the provided filename - you may need to adjust
FIRST_SPLIT_FILE = "mistralai_Mistral-Small-4-119B-2603-Q6_K-00001-of-00003.gguf"
MODEL_PATH = os.path.join(constants.MODEL_BASE_PATH, FIRST_SPLIT_FILE)

# Model-specific settings for 119B model
# This model is huge, so GPU layers need careful tuning
GPU_LAYERS = 33  # Start conservative - adjust based on VRAM
CONTEXT_SIZE = 1024  # Smaller context to save memory
BATCH_SIZE = 512


def check_split_files():
    """Verify that all split files exist"""
    model_dir = Path(constants.MODEL_BASE_PATH)

    # Look for all split files matching the pattern
    pattern = f"{MODEL_NAME}*.gguf"
    split_files = list(model_dir.glob(pattern))

    if not split_files:
        print(f"❌ No model files found matching: {pattern}")
        print(f"   Checked in: {constants.MODEL_BASE_PATH}")
        return False

    print(f"✅ Found {len(split_files)} split files:")
    for f in sorted(split_files):
        print(f"   - {f.name}")

    # Check if we have the expected number of files
    # You may need to adjust this based on actual splits
    if len(split_files) < 2:
        print("⚠️  Warning: Expected multiple split files for 119B model")

    return True


def estimate_memory():
    """Estimate memory requirements for the model"""
    # Rough estimate: Q6_K quantization ~ 6.5 bits per parameter
    # 119B parameters * 6.5 bits / 8 = ~96.7 GB for full model
    model_size_gb = (119 * 6.5) / 8  # ~96.7 GB

    print("\n" + "=" * 60)
    print("MEMORY ESTIMATION")
    print("=" * 60)
    print(f"Model: 119B parameters (Q6_K quantization)")
    print(f"Estimated full model size: {model_size_gb:.1f} GB")
    print(f"GPU Layers: {GPU_LAYERS} (~{(GPU_LAYERS / 80) * 100:.1f}% of model)")
    print(f"Context Size: {CONTEXT_SIZE} tokens")
    print("\nRecommendations:")
    print("- With 24GB VRAM: Try -ngl 15-20")
    print("- With 48GB VRAM: Try -ngl 30-35")
    print("- With 80GB+ VRAM: Try -ngl 50-60")
    print("=" * 60)


def clean_text_for_c_string(text):
    """Clean and escape text for use in C string literals"""
    if not text:
        return ""

    text = re.sub(r'```.*?```', '', text, flags=re.DOTALL)
    text = re.sub(r'`.*?`', '', text)
    text = ' '.join(text.split())
    text = text.replace('\\', '\\\\')
    text = text.replace('"', '\\"')
    text = text.replace('\n', '\\n')
    text = text.replace('\r', '\\r')
    text = text.replace('\t', '\\t')

    return text.strip()


def extract_description(text):
    """Extract the final description from model output"""
    # Remove thinking tags
    text = re.sub(r'<think>.*?</think>', '', text, flags=re.DOTALL).strip()

    # Look for explicit markers
    for marker in ['Final Description:', 'Final Answer:', 'Best Description:']:
        if marker in text:
            parts = text.split(marker)
            candidate = parts[-1].strip().split('\n')[0].strip()
            if candidate and len(candidate.split()) > 5:
                return candidate

    # Take first sentence
    sentences = re.split(r'(?<=[.!?])\s+', text)
    if sentences:
        return sentences[0]

    return text


def generate_response(word, table, entity, max_attempts=5):
    """Send prompt to llama-server and get a proper description"""

    system_instruction = f"You are a folklore expert. Provide only the {entity} description, no explanations. Do not include the {entity}'s name."
    # Get terms hint
    terms_hint = validation.get_terms_hint(word, 1)

    # Build the user message
    user_message = f"Describe a fantasy {word} {entity} in one vivid sentence, {int(constants.MIN_WORDS)}-{int(constants.MAX_WORDS)} words. {terms_hint}"

    # Use the CORRECT Mistral chat format
    prompt = f"<s>[INST] {system_instruction}\n\n{user_message} [/INST]"
    print(prompt)

    for attempt in range(max_attempts):
        if not db_manager.needs_generation(word, table, MODEL_NAME, constants.DESIRED_COUNT):
            print(f"  ✅ {word} already has {constants.DESIRED_COUNT} descriptions")
            return None, attempt

        payload = {
            "prompt": prompt,
            "max_tokens": 512,
            "temperature": 0.7 + (attempt * 0.001),
            "top_p": 0.95,
            "stop": ["</s>"]  # Stop at end token
        }

        try:
            print(f"  Sending request for {word} (attempt {attempt + 1})...")
            # print(f"  PROMPT: {prompt[:100]}...")  # Debug print

            response = requests.post(
                f"{constants.SERVER_URL}",
                json=payload,
                timeout=300
            )

            if response.status_code != 200:
                print(f"  Status code: {response.status_code}, retrying...")
                continue

            result = response.json()
            text = result.get("content", "").strip()

            # Debug print
            print(f"  RAW RESPONSE: {text}")

            if not text:
                print(f"  ✗ Attempt {attempt + 1}: Empty response")
                continue

            if constants.MAX_CHARS == 128:
                text = validation.check_for_name(text, attempt, word)

            if text == "":
                continue

            clean_text = ' '.join(text.split())
            words = clean_text.split()

            if len(words) < constants.MIN_WORDS:
                print(f"  ✗ Attempt {attempt + 1}: Too short ({len(words)} words)")
                continue

            # Creature validation
            is_valid, matches = validation.validate_creature_description(
                word, clean_text, attempt
            )
            if not is_valid:
                continue

            # Length check
            is_valid, word_count = validation.check_length_and_format(
                clean_text, attempt, max_attempts
            )
            if not is_valid:
                continue

            print(f"  ✓ Valid response ({word_count} words)")
            return clean_text, attempt + 1

        except requests.exceptions.Timeout:
            print(f"  ✗ Attempt {attempt + 1}: Timeout (model is slow)")
            time.sleep(5)
        except Exception as e:
            print(f"  ✗ Attempt {attempt + 1}: {type(e).__name__}")
            time.sleep(2)

    return None, max_attempts


def generate_list(test_creatures, table, entity_type):
    generated = 0
    skipped = 0
    failed = 0
    n = 0
    while n < constants.DESIRED_COUNT:
        n += 1
        for i, word in enumerate(test_creatures):
            print(f"\n  {i + 1}/{len(test_creatures)}: {word}")

            if not db_manager.needs_generation(word, table, MODEL_NAME, constants.DESIRED_COUNT):
                existing = db_manager.get_existing_count(word, table, MODEL_NAME)
                print(f"  ✅ Already have {existing}/{constants.DESIRED_COUNT} descriptions, skipping")
                skipped += 1
                continue

            result = generate_response(word, table, entity_type)

            if result is None or result[0] is None:
                print(f"  ⚠ Failed to generate for {word}")
                failed += 1
                continue

            response, attempt_num = result

            # Calculate matches
            matches = 0
            if word in creature_descriptors.CREATURE_VALIDATION:
                required_terms = creature_descriptors.CREATURE_VALIDATION[word]["required_terms"]
                matches = sum(1 for term in required_terms if term in response.lower())
            else:
                print(f"{word} not in validation")

            # Save to database
            db_manager.save_to_db(
                table=table,
                creature_name=word,
                description=response,
                attempt_num=attempt_num,
                matches=matches,
                model_name=MODEL_NAME,
                prompt_version=constants.PROMPT_VERSION,
                temperature=0.7 + ((attempt_num - 1) * 0.001)
            )

            generated += 1
            time.sleep(2)  # Longer delay for 119B model

    # Summary
    print("\n" + "=" * 60)
    print("GENERATION COMPLETE")
    print("=" * 60)
    print(f"Total processed: {len(test_creatures)}")
    print(f"Newly generated: {generated}")
    print(f"Skipped: {skipped}")
    print(f"Failed: {failed}")
    print(f"\n")


def main():
    # Check for split files first
    print("\n🔍 Checking for model files...")
    if not check_split_files():
        print("\n❌ Cannot proceed without model files.")
        print("Please check that the model files exist and adjust the filename pattern.")
        return

    estimate_memory()

    print("\n⚠️  IMPORTANT NOTES:")
    print("1. The 119B model is VERY large (~97GB in memory)")
    print("2. You need sufficient RAM/VRAM to load it")
    print("3. Generation will be slow even on good hardware")
    print("4. Consider using a smaller model if this is too heavy")
    print("5. The server must be running with this model loaded")
    print("\nPress Ctrl+C within 5 seconds to cancel...")

    # Initialize database
    db_manager.init_database()

    print("\n" + "=" * 60)
    print(f"Generating descriptions for model: {MODEL_NAME}")
    print(f"Using split files in: {constants.MODEL_BASE_PATH}")
    print(f"First split file: {FIRST_SPLIT_FILE}")
    print(f"Prompt version: {constants.PROMPT_VERSION}")
    print("=" * 60)

    if TEST_MODE:
        print("\n🧪 TEST MODE: Generating only first 5 creatures")
        print("Change TEST_MODE = False in code to generate all 254\n")

    # Optional: Test with just a few creatures first
    test = _creature_img_data.Creatures[:5] if TEST_MODE else _creature_img_data.Creatures
    generate_list(test, constants.CREATURE_TABLE, "creature")

    test = _skill_img_data.Skills[:5] if TEST_MODE else _skill_img_data.Skills
    generate_list(test, constants.SKILL_TABLE, "skill")

    test = _spell_img_data.Spells[:5] if TEST_MODE else _spell_img_data.Spells
    generate_list(test, constants.SPELL_TABLE, "spell")

    test = _item_img_data.Items[:5] if TEST_MODE else _item_img_data.Items
    generate_list(test, constants.ITEM_TABLE, "item")

    test = _object_img_data.Objects[:5] if TEST_MODE else _object_img_data.Objects
    generate_list(test, constants.OBJECT_TABLE, "object")

    test = _trainer_img_data.Trainers[:5] if TEST_MODE else _trainer_img_data.Trainers
    generate_list(test, constants.TRAINER_TABLE, "trainer")


if __name__ == "__main__":
    main()
