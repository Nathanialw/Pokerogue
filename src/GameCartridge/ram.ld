/*
1MB	    1,048,576	0x00100000	0x0FFFFF (1,048,575)
2MB	    2,097,152	0x00200000	0x1FFFFF
4MB	    4,194,304	0x00400000	0x3FFFFF
8MB	    8,388,608	0x00800000	0x7FFFFF
16MB	16,777,216	0x01000000	0xFFFFFF (16,777,215)
16MB	16,777,216	0x02000000	0xFFFFFF
32MB	16,777,216	0x04000000	0xFFFFFF
64MB	16,777,216	0x08000000	0xFFFFFF
128MB	16,777,216	0x10000000	0xFFFFFF
256MB	16,777,216	0x20000000	0xFFFFFF
*/

/*16MB is used right now 16777216*/
MEMORY
{
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 141K
}

SECTIONS
{
    /* Start at address 0 */
    . = 0x000000;

/* Battle / Map Overlays */
    .battle 0x20000000 : AT(0x000000)
    {
        KEEP(*(.battle))
        . = ALIGN(4);
    } > RAM

    .map 0x20000000 : AT(0x020000)
    {
        KEEP(*(.map))
        . = ALIGN(4);
    } > RAM

    .title 0x20000000 : AT(0x040000)
    {
        KEEP(*(.title_entry))   /* must come first */
        KEEP(*(.title))
        . = ALIGN(4);
    } > RAM


    /* Overlay symbols */
    __overlay_start__ = 0x20000000;
    __battle_lma      = LOADADDR(.battle);
    __map_lma         = LOADADDR(.map);
    __title_lma       = LOADADDR(.title);
    __battle_size     = SIZEOF(.battle);
    __map_size        = SIZEOF(.map);
    __title_size      = SIZEOF(.title);

    /* ========================================== */
    /*     FORCE ABSOLUTE POSITIONS FROM HERE     */
    /* ========================================== */

    . = 0x400000;

    .strings_items      0x400000 : { KEEP(*(.strings_items))      . = ALIGN(4); }
    .strings_creatures  0x410000 : { KEEP(*(.strings_creatures))  . = ALIGN(4); }
    .strings_spells     0x420000 : { KEEP(*(.strings_spells))     . = ALIGN(4); }
    .strings_attacks    0x430000 : { KEEP(*(.strings_attacks))    . = ALIGN(4); }
    .strings_trainers   0x440000 : { KEEP(*(.strings_trainers))   . = ALIGN(4); }
    .strings_objects    0x450000 : { KEEP(*(.strings_objects))    . = ALIGN(4); }

    . = 0x500000;

    .sprites_items      0x500000 : { KEEP(*(.sprites_items))      . = ALIGN(4); }
    .sprites_creatures  0x510000 : { KEEP(*(.sprites_creatures))  . = ALIGN(4); }
    .sprites_spells     0x520000 : { KEEP(*(.sprites_spells))     . = ALIGN(4); }
    .sprites_attacks    0x530000 : { KEEP(*(.sprites_attacks))    . = ALIGN(4); }
    .sprites_trainers   0x540000 : { KEEP(*(.sprites_trainers))   . = ALIGN(4); }

    /* This is the critical one */
    . = 0x550000;
    .sprites_objects    0x550000 :
    {
        KEEP(*(.sprites_objects))
        KEEP(*(.sprites_objects*))     /* catch variations */
        . = ALIGN(4);
    }

    __rom_end = .;
    end = .;
}