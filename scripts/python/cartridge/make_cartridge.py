#!/usr/bin/env python3
"""
Creates a fixed-layout cartridge binary from the ELF file.
Respects all absolute addresses defined in the linker script.
"""

import sys
import subprocess
from pathlib import Path

def main():
    elf_path = Path("../../../build-cartridge/src/GameCartridge/GameCartridge")
    output_bin = Path("../../../build-cartridge/src/GameCartridge/cartridge.bin")
    # rom_size = 0x1000000  # 16MB
    rom_size = 0x0400000  # 16MB

    if not elf_path.exists():
        print(f"Error: ELF file not found: {elf_path}")
        sys.exit(1)

    # Create blank ROM filled with 0xFF (standard for flash cartridges)
    rom = bytearray(b'\xFF' * rom_size)

    # Define sections and their exact addresses
    sections = [
        #code
        ".title",
        ".map",
        ".battle",
        ".strings_items",
        ".strings_creatures",
        ".strings_spells",
        ".strings_attacks",
        ".strings_trainers",
        ".strings_objects",
        ".sprites_items",
        ".sprites_creatures",
        ".sprites_spells",
        ".sprites_attacks",
        ".sprites_trainers",
        ".sprites_objects",
    ]
    print("Building cartridge...")

    p = 0x0
    rom[p] = len(sections)
    p = p + 0x1

    addr = (len(sections) * 8) + 1
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
            print(f"{addr} {addr.to_bytes(4, byteorder='little')}")

            l = len(data)
            print(f"{l} {l.to_bytes(4, byteorder='little')}")
            rom[p:0x4] = l.to_bytes(4, byteorder='little')
            p = p + 0x4

            addr = addr + l
            tmp_file.unlink()
        else:
            print(f"  No data for {section_name}")

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
    print(f"\nSuccess! Created {output_bin.name} ({rom_size//1024//1024} MB)")
    print(f"   sprites_objects should be at offset 0x550000")

if __name__ == "__main__":
    main()