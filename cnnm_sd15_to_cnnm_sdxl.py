"""
Convertion of SD 1.5 Control net for normal maps to SDXL compatible architecture

Code by: Adam Handschuh
"""

import os
import torch
from diffusers import ControlNetModel
from diffusers.models.controlnet import ControlNetConfig
from transformers import logging as hf_logging

hf_logging.set_verbosity_error()  # Silence warnings for clean output


def load_controlnet_sd15(model_id: str) -> ControlNetModel:
    """Load the SD 1.5 ControlNet model."""
    print(f"Loading source ControlNet from: {model_id}")
    return ControlNetModel.from_pretrained(model_id, torch_dtype=torch.float16)


def create_controlnet_sdxl(config_id: str) -> ControlNetModel:
    """Initialize an empty ControlNet model using an SDXL-compatible config."""
    print(f"Loading SDXL ControlNet config from: {config_id}")
    config = ControlNetConfig.from_pretrained(config_id)
    return ControlNetModel(config).to(dtype=torch.float16)


def transfer_weights(
    src_model: ControlNetModel,
    target_model: ControlNetModel
) -> None:
    """Transfer weights from SD 1.5 ControlNet to SDXL ControlNet, where compatible."""
    src_state = src_model.state_dict()
    tgt_state = target_model.state_dict()
    new_state = {}

    matched, skipped = 0, 0

    for key, tgt_weight in tgt_state.items():
        src_weight = src_state.get(key)

        if src_weight is None:
            print(f"[!] Missing key in source: {key}")
            skipped += 1
            continue

        if src_weight.shape != tgt_weight.shape:
            print(f"[!] Shape mismatch for {key}: {src_weight.shape} → {tgt_weight.shape}")
            skipped += 1
            continue

        new_state[key] = src_weight
        matched += 1

    print(f"\nWeights transferred: {matched}")
    print(f"Skipped (incompatible/missing): {skipped}\n")

    target_model.load_state_dict(new_state, strict=False)


def save_model(model: ControlNetModel, save_path: str) -> None:
    """Save the converted model to disk."""
    print(f"Saving converted model to: {save_path}")
    os.makedirs(save_path, exist_ok=True)
    model.save_pretrained(save_path)


def convert_controlnet_normal_to_sdxl(
    source_model_id: str,
    target_config_id: str,
    save_path: str
) -> None:
    """Main conversion pipeline."""
    src_model = load_controlnet_sd15(source_model_id)
    tgt_model = create_controlnet_sdxl(target_config_id)
    transfer_weights(src_model, tgt_model)
    save_model(tgt_model, save_path)
    print("🎉 Conversion complete. You're SDXL-ready!")


if __name__ == "__main__":
    # ----- USER CONFIG -----
    SOURCE_MODEL_ID = "lllyasviel/sd-controlnet-normal"
    TARGET_CONFIG_ID = "diffusers/controlnet-canny-sdxl-1.0"
    SAVE_PATH = "./controlnet-normal-sdxl"
    # ------------------------

    convert_controlnet_normal_to_sdxl(SOURCE_MODEL_ID, TARGET_CONFIG_ID, SAVE_PATH)
