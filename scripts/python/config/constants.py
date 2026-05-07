# constants.py
# Shared configuration
SERVER_URL = "http://127.0.0.1:8000/completion"
INC_FOLDER = "../src/GameCore/inc"

OUTPUT_TYPE_STRINGS = f"{INC_FOLDER}/text_types.inc"
PROMPT_VERSION = "v1.0"

# Server configuration
DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 8000
DEFAULT_CONTEXT_SIZE = 2048
DEFAULT_THREADS = 20
DEFAULT_GPU_LAYERS = 33
DEFAULT_FLASH_ATTN = True
DESIRED_COUNT = 20
MAX_HINT_ATTEMPTS = 10000

IMG_PROMPT = False

if IMG_PROMPT:
    MAX_CHARS = 400
    MIN_CHARS = 200
    MIN_WORDS = 20
    MAX_WORDS = 80
else:
    MAX_CHARS = 126
    MIN_CHARS = MAX_CHARS - (32 * (MAX_CHARS / 126))
    MIN_WORDS = MAX_CHARS / 8
    MAX_WORDS = MAX_CHARS / 6

DB_FILE = f"game_data.db"

MODEL_BASE_PATH = "/mnt/media/ai/text/llama.cpp_built/models/"

# Model paths (can be overridden)
QWEN_MODEL_PATH = "/mnt/media/ai/text/llama.cpp_built/models/ConicCat_Qwen3.5-27B-Writer-Q6_K.gguf"
LLAMA_MODEL_PATH = "/mnt/media/ai/text/llama.cpp_built/models/bartowski_Llama-3.1-SuperNova-Lite-GGUF_Llama-3.1-SuperNova-Lite-Q4_K_M.gguf"
OLMO_MODEL_PATH = "/mnt/media/ai/text/llama.cpp_built/models/OLMo-2-1124-13B-Instruct-Q6_K.gguf"
MISTRAL_MODEL_PATH = "/mnt/media/ai/text/llama.cpp_built/models/"

# Model names for database tracking
MODEL_NAMES = {
    "qwen": "ConicCat_Qwen3.5-27B-Writer-Q6_K.gguf",
    "llama": "Llama-3.1-SuperNova-Lite-Q4_K_M",
    "olmo": "OLMo-2-1124-13B-Instruct-Q6_K.gguf",
    "mistral": "mistralai_Mistral-Small-4-119B-2603-Q6_K"
}

CREATURE_TABLE = "creature_descriptions"
SKILL_TABLE = "skill_descriptions"
SPELL_TABLE = "spell_descriptions"
ITEM_TABLE = "item_descriptions"
OBJECT_TABLE = "object_descriptions"
TRAINER_TABLE = "trainer_descriptions"
