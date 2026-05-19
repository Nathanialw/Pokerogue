#!/usr/bin/env python3
"""
Creates a fixed-layout cartridge binary from the ELF file.
Respects all absolute addresses defined in the linker script.
"""
import sys
import subprocess
from pathlib import Path

from python.config import constants


def main():
    elf_path = Path("../build-cartridge/src/GameCartridge/GameCartridge")
    output_bin = Path("../build-cartridge/src/GameCartridge/cartridge.bin")
    # rom_size = 0x1000000  # 16MB
    rom_size = 0x0400000  # 4MB

    if not elf_path.exists():
        print(f"Error: ELF file not found: {elf_path}")
        sys.exit(1)

    # Create blank ROM filled with 0xFF (standard for flash cartridges)

    # Define sections and their exact addresses
    defines_pos = []
    defines_size = []

    sections = [
        #code
        ".core",
        ".splash",
        ".map",
        ".map_gen",
        ".battle",

        #   DATA
        # char sprite colors
        ".char_sprites_items",
        ".char_sprites_monsters",
        ".char_sprites_objects",
        ".char_sprites_biomes",

        # tilesets
        ".tileset_theme",
        ".tileset_creatures",

        # strings
        ".strings_menu_main",
        ".strings_menu_battle",
        ".strings_menu_options",
        ".strings_names_items",
        ".strings_names_creatures",
        ".strings_names_spells",
        ".strings_names_skills",
        ".strings_names_objects",
        ".strings_names_trainers",
        ".strings_descriptions_items",
        ".strings_descriptions_creatures",
        ".strings_descriptions_spells",
        ".strings_descriptions_skills",
        ".strings_descriptions_objects",
        ".strings_descriptions_trainers",
        ".strings_types",

        # sprites
        ".sprite_battler_layout_back",
        ".sprite_battler_layout_front",
        ".sprite_battler_back",
        ".sprite_battler_front",
        ".sprite_items_layout",
        ".sprite_creatures_layout",
        ".sprite_spells_layout",
        ".sprite_skills_layout",
        ".sprite_objects_layout",
        ".sprite_trainers_layout",
        ".sprite_items",
        ".sprite_creatures",
        ".sprite_spells",
        ".sprite_skills",
        ".sprite_objects",
        ".sprite_trainers",
        ".sprite_map_creature",
        ".sprite_map_item",
        ".sprite_map_object",
        ".sprite_map_trainer",
        ".sprite_map_tile",
        ".sprite_map_creature_metaData",
        ".sprite_map_item_metaData",
        ".sprite_map_object_metaData",
        ".sprite_map_trainer_metaData",
        ".sprite_map_tile_metaData",

        #fonts
        ".font_8x8",
        ".font_16x16",


        # game_data
        ".game_data_type_effects",
        ".game_data_creature_types",
        ".game_data_creature_stats",
        ".game_data_learnable_skills",
        ".game_data_level_up_skills",
        ".game_data_ability",
        ".game_data_spell",
        ".game_data_item",
        ".game_data_object",

        # colors
        ".colors_16",
        ".colors_256",

        # sounds
        ".sounds_music_data",
        ".sounds_music_notes",
        ".sounds_creatures",
        ".sounds_spells",
        ".sounds_skills",
        ".sounds_menus",
        ".sounds_notes",
    ]
    print("Building cartridge...")

    header_size = (len(sections) * 8) + 1
    rom = bytearray(b'\xFF' * (rom_size - (header_size - 4)))

    p = 0x0
    rom[p] = len(sections)
    p = p + 0x1

    addr = header_size
    # build header
    for section_name in sections:
        tmp_file = Path(f"{section_name}.tmp")

        # Extract section from ELF
        result = subprocess.run([
            "arm-none-eabi-objcopy",
            "--dump-section", f"{section_name}={tmp_file}",
            str(elf_path)
        ], capture_output=True)

        if result.returncode == 0 and tmp_file.exists():
            data = tmp_file.read_bytes()

            rom[p:0x4] = addr.to_bytes(4, byteorder='little')
            p = p + 0x4
            defines_pos.append(addr)

            l = len(data)
            defines_size.append(l)
            rom[p:0x4] = l.to_bytes(4, byteorder='little')
            p = p + 0x4

            addr = addr + l
            tmp_file.unlink()
        else:
            print(f"  No data for {section_name}")


    # Generate .ld file



    # build cartridge
    for section_name in sections:
        tmp_file = Path(f"{section_name}.tmp")

        # Extract section from ELF
        result = subprocess.run([
            "arm-none-eabi-objcopy",
            "--dump-section", f"{section_name}={tmp_file}",
            str(elf_path)
        ], capture_output=True)

        if result.returncode == 0 and tmp_file.exists():
            data = tmp_file.read_bytes()
            rom[p:p + len(data)] = data
            p = p + len(data)
            print(f"✓ Placed {section_name:<20} at 0x{p:06X}  ({len(data):,} bytes)")
            tmp_file.unlink()
        else:
            print(f"  No data for {section_name}")

    # Write final binary
    output_bin.write_bytes(rom)

    print(f"\nSuccess! Created {output_bin.name} ({rom_size//1024//1024} MB) ({rom_size//1024} KB) ({rom_size} B)")

    # write defines
    filename = f"{constants.INC_FOLDER}/memory_constants.inc"

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"#pragma once\n\n")
        f.write(f"//  Defines the layout of data in rom\n")
        f.write(f"///   {len(defines_pos)} entries MAX 255\n\n\n")
        for i in range(len(defines_pos)):
            f.write(f"#define {sections[i].upper().lstrip('.')}_POSITION {defines_pos[i]:#x} \n")
            f.write(f"#define {sections[i].upper().lstrip('.')}_SIZE {defines_size[i]:#x} \n")

        f.write("\n\n\n")
        f.write(f"///   {len(defines_pos)} entries MAX 255\n")
        f.write("\n")



if __name__ == "__main__":
    main()