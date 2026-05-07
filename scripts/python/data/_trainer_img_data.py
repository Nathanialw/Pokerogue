# Generated from trainer_img_prompts table
# Do not edit manually

Trainers = [
]

TrainersDict = [
]

BASE_PROMPT = (
    # f"photo-realistic hi fidelity detailed, "
    f"Illustrated with contrasting vibrant colours, "
    "fantasy trainer, full body visible, centered composition, isolated creature, "
    "single subject, no cropping, highly detailed dark fantasy illustration, sharp focus, "
)

LIGHTING_STYLES = [
    # "dramatic rim lighting",
    # "moody low-key lighting, subtle fog",
    # "ancient parchment illustration style, inked linework",
    "torchlit dungeon lighting",
    # "volumetric light shafts",
    "soft studio lighting",

    "high contrast lighting, crisp silhouette",
    # "grimdark concept art, gritty texture",
    # "moonlit night lighting",
    # "overcast diffuse lighting",
]

# View angles - expanded with back-side angles
VIEW_ANGLES = [
    # "",  # front/full face (implied)
    "back view",
    "strict side profile view, 90 degree lateral pose, full side silhouette",
    "three-quarter side view, strong 45 degree angle over-the-shoulder",
    # "rear three-quarter view, mostly back but slight side visible",
    # "direct rear view, seen from straight behind, full back, over-the-shoulder",
    "looking back over shoulder, body in profile but head turned",
]

# Generate all combinations
VARIANTS = [
    f"{angle + ', ' if angle else ''}{style}, neutral background"
    for style in LIGHTING_STYLES
    for angle in VIEW_ANGLES
]

NEGATIVE_PROMPT = (
    "deformed iris, deformed pupils, semi-realistic, cgi, 3d, render, sketch, cartoon, drawing, anime), text, cropped, out of frame, worst quality, low quality, jpeg artifacts, ugly, duplicate, morbid, mutilated, extra fingers, mutated hands, poorly drawn hands, poorly drawn face, mutation, deformed, blurry, dehydrated, bad anatomy, bad proportions, extra limbs, cloned face, disfigured, gross proportions, malformed limbs, missing arms, missing legs, extra arms, extra legs, fused fingers, too many fingers, long neck, brown men, black men, asian men"
)
        