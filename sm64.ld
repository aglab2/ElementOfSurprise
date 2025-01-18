OUTPUT_ARCH (mips)

#define LINKER /* Removes externs from preprocessed script */
#define TARGET_N64
#include "segments.h"
#include "config.h"
#undef LINKER

#define BEGIN_SEG(name, addr) \
    _##name##SegmentStart = ADDR(.name); \
    _##name##SegmentRomStart = __romPos; \
    .name addr : AT(__romPos)

#define END_SEG(name) \
    _##name##SegmentEnd = ADDR(.name) + SIZEOF(.name); \
    _##name##SegmentRomEnd = __romPos + SIZEOF(.name); \
    __romPos += SIZEOF(.name);

#define BEGIN_NOLOAD(name) \
    _##name##SegmentBssStart = ADDR(.name.noload); \
    .name.noload (NOLOAD) :

#define END_NOLOAD(name) \
    _##name##SegmentBssEnd = ADDR(.name.noload) + SIZEOF(.name.noload); \
    _##name##SegmentBssSize = SIZEOF(.name.noload);

#define YAY0_SEG(name, segAddr) \
   BEGIN_SEG(name##_yay0, segAddr) \
   { \
      KEEP(BUILD_DIR/bin/name.szp.o(.data*)); \
      KEEP(BUILD_DIR/bin/name.szp.o(.rodata*)); \
      . = ALIGN(0x10); \
   } \
   END_SEG(name##_yay0) \
   _##name##_mio0SegmentRomStart = _##name##_yay0SegmentRomStart; \
   _##name##_mio0SegmentRomEnd   = _##name##_yay0SegmentRomEnd;

#define STANDARD_LEVEL(name) \
   BEGIN_SEG(name##_segment_7, 0x07000000) \
   { \
      KEEP(BUILD_DIR/levels/name/leveldata.szp.o(.data*)); \
      . = ALIGN(0x10); \
   } \
   END_SEG(name##_segment_7) \
   BEGIN_SEG(name, 0x19000000) \
   { \
      KEEP(BUILD_DIR/levels/name/script.o(.data*)); \
      KEEP(BUILD_DIR/levels/name/script.o(.text*)); \
      KEEP(BUILD_DIR/levels/name/script.o(.rodata*)); \
      KEEP(BUILD_DIR/levels/name/geo.o(.data*)); \
      KEEP(BUILD_DIR/levels/name/geo.o(.text*)); \
      KEEP(BUILD_DIR/levels/name/geo.o(.rodata*)); \
   } \
   END_SEG(name) \
   BEGIN_NOLOAD(name) \
   { \
      KEEP(BUILD_DIR/levels/name/script.o(.bss*)); \
      KEEP(BUILD_DIR/levels/name/geo.o(.bss*)); \
   } \
   END_NOLOAD(name)

#define STANDARD_OBJECTS(name, segAddr, geoAddr) \
   BEGIN_SEG(name##_yay0, segAddr) \
   { \
      KEEP(BUILD_DIR/actors/name.szp.o(.data*)); \
      . = ALIGN(0x10); \
   } \
   END_SEG(name##_yay0) \
   BEGIN_SEG(name##_geo, geoAddr) \
   { \
      KEEP(BUILD_DIR/actors/name##_geo.o(.data*)); \
      KEEP(BUILD_DIR/actors/name##_geo.o(.rodata*)); \
      KEEP(BUILD_DIR/actors/name##_geo.o(.text*)); \
   } \
   END_SEG(name##_geo) \
   BEGIN_NOLOAD(name##_geo) \
   { \
      KEEP(BUILD_DIR/actors/name##_geo.o(.bss*)); \
   } \
   END_NOLOAD(name##_geo) \
   _##name##_mio0SegmentRomStart = _##name##_yay0SegmentRomStart; \
   _##name##_mio0SegmentRomEnd = _##name##_yay0SegmentRomEnd;

#define VANILLA_OBJECTS(name) \
   BEGIN_SEG(vo_##name##_yay0, 0x0e000000) \
   { \
      KEEP(BUILD_DIR/actors/vanilla_actors/name/data.szp.o(.data*)); \
      . = ALIGN(0x10); \
   } \
   END_SEG(vo_##name##_yay0) \
   BEGIN_SEG(vo_##name##_geo, 0x1a000000) \
   { \
      KEEP(BUILD_DIR/actors/vanilla_actors/name/geo.o(.data*)); \
      KEEP(BUILD_DIR/actors/vanilla_actors/name/geo.o(.rodata*)); \
      KEEP(BUILD_DIR/actors/vanilla_actors/name/geo.o(.text*)); \
   } \
   END_SEG(vo_##name##_geo) \
   BEGIN_NOLOAD(vo_##name##_geo) \
   { \
      KEEP(BUILD_DIR/actors/vanilla_actors/name/geo.o(.bss*)); \
   } \
   END_NOLOAD(vo_##name##_geo) \
   _vo_##name##_mio0SegmentRomStart = _vo_##name##_yay0SegmentRomStart; \
   _vo_##name##_mio0SegmentRomEnd = _vo_##name##_yay0SegmentRomEnd;

#define COMPRESSED_CONTENT(name) \
   BEGIN_SEG(cc_##name##_yay0, 0x0e000000) \
   { \
      KEEP(BUILD_DIR/compressed_content/name/data.szp.o(.data*)); \
      . = ALIGN(0x10); \
   } \
   END_SEG(cc_##name##_yay0) \
   _cc_##name##_mio0SegmentRomStart = _cc_##name##_yay0SegmentRomStart; \
   _cc_##name##_mio0SegmentRomEnd = _cc_##name##_yay0SegmentRomEnd;

SECTIONS
{
   __romPos = 0;

   BEGIN_SEG(boot, 0x04000000)
   {
      KEEP(BUILD_DIR/asm/rom_header.o(.text*));
      KEEP(BUILD_DIR/asm/ipl3.o(.text*));
   }
   END_SEG(boot)

   . = (RAM_START + 0x400);
#if MEMORY_FRAGMENTATION_NO_FRAGMENTATION == MEMORY_FRAGMENTATION_LEVEL
   BEGIN_NOLOAD(zbuffer)
   {
      BUILD_DIR/src/buffers/zbuffer.o(.bss*);
   }
   END_NOLOAD(zbuffer)
#endif

   BEGIN_NOLOAD(buffers)
   {
      BUILD_DIR/src/buffers/buffers.o(.bss*);
      BUILD_DIR/src/audio/globals_start.o(.bss*);
      BUILD_DIR/src/audio/synthesis.o(.bss*);
      BUILD_DIR/src/audio/heap.o(.bss*);
      BUILD_DIR/src/audio/load.o(.bss*);
      BUILD_DIR/src/audio/data.o(.bss*);
      BUILD_DIR/src/audio*.o(.bss*);
      . = ALIGN(0x10);
      BUILD_DIR/src/buffers/gfx_output_buffer.o(.bss*);
   }
   END_NOLOAD(buffers)

   . = _buffersSegmentBssEnd;

#ifdef HVQM
   BEGIN_NOLOAD(hvqmwork)
   {
      BUILD_DIR/src/buffers/hvqmwork.o(.bss*);
   }
   END_NOLOAD(hvqmwork)
   BEGIN_NOLOAD(adpcmbuf)
   {
      BUILD_DIR/src/buffers/adpcmbuf.o(.bss*);
   }
   END_NOLOAD(adpcmbuf)
   BEGIN_NOLOAD(hvqbuf)
   {
      BUILD_DIR/src/buffers/hvqbuf.o(.bss*);
   }
   END_NOLOAD(hvqbuf)

   . = _hvqbufSegmentBssEnd;
#endif

/* hardcoded symbols to satisfy preliminary link for map parser */
#ifndef DEBUG_MAP_STACKTRACE
      _mapDataSegmentRomStart = 0;
      gMapEntries   = 0;
      gMapEntrySize = 0;
      gMapStrings   = 0;
#endif

   BEGIN_SEG(main, .) SUBALIGN(16)
   {
      KEEP(BUILD_DIR/asm/entry.o(.text*));
      KEEP(BUILD_DIR/asm/n64_assert.o(.text*));
      KEEP(BUILD_DIR/asm/setgp.o(.text*));
#ifdef EEP
      KEEP(BUILD_DIR/asm/vc_bin.o(.text*));
#endif
      KEEP(BUILD_DIR/asm/emux.o(.text*));
      KEEP(BUILD_DIR/asm/pj64_get_count_factor_asm.o(.text*));
      KEEP(BUILD_DIR/asm/round.o(.text*));
      KEEP(BUILD_DIR/asm/fcr31.o(.text*));

      BUILD_DIR/src/boot*.o(.text*);
      BUILD_DIR/src/hvqm*.o(.text*);
      BUILD_DIR/src/usb*.o(.text*);
      BUILD_DIR/src/audio*.o(.text*);
#ifdef S2DEX_TEXT_ENGINE
      BUILD_DIR/src/s2d_engine*.o(.text*);
#endif
#ifdef LIBPL
      BUILD_DIR/lib/libpl*.o(.text*);
#endif
      *ULTRALIB.a:*.o(.text*);
      *libnustd.a:*.o(.text*);
      *libgcc.a:*.o(.text*);
      */libz.a:*.o(.text*);
      */libhvqm2.a:*.o(.text*);
      lib/rspboot.o(.text*);
#include "rsptext.inc.ld"
      BUILD_DIR/lib/aspMain.o(.text*);
      lib/PR/audio/n_aspMain.o(.text*);
      lib/PR/hvqm/hvqm2sp1.o(.text*);
      _mainSegmentTextEnd = .;

      /* data */
      BUILD_DIR/asm/n64_assert.o(.data*);
      BUILD_DIR/src/boot*.o(.data*);
      BUILD_DIR/src/audio*.o(.data*);
#ifdef S2DEX_TEXT_ENGINE
      BUILD_DIR/src/s2d_engine*.o(.data*);
#endif
#ifdef LIBPL
      BUILD_DIR/lib/libpl*.o(.data*);
#endif
      *ULTRALIB.a:*.o(.data*);
      */libhvqm2.a:*.o(.data*);
      */libz.a:*.o(.data*);
#include "rspdata.inc.ld"
      BUILD_DIR/lib/aspMain.o(.data*);
      lib/PR/audio/n_aspMain.o(.data*);
      lib/PR/hvqm/hvqm2sp1.o(.data*);

      /* rodata */
      BUILD_DIR/src/boot*.o(.rodata*);
      BUILD_DIR/src/usb*.o(.rodata*);
      BUILD_DIR/src/audio*.o(.rodata*);
#ifdef S2DEX_TEXT_ENGINE
      BUILD_DIR/src/s2d_engine*.o(.rodata*);
#endif
#ifdef LIBPL
      BUILD_DIR/lib/libpl*.o(.rodata*);
#endif
      *ULTRALIB.a:*.o(.rodata*);
      *libgcc.a:*.o(.rodata*);
      */libz.a:*.o(.rodata*);

      lib/PR/hvqm/hvqm2sp1.o(.rodata*);
   }
   END_SEG(main)
   BEGIN_NOLOAD(main)
   {
      BUILD_DIR/src/boot*.o(.bss*);
      BUILD_DIR/src/hvqm*.o(.bss*);
      BUILD_DIR/src/usb*.o(.bss*);
      BUILD_DIR/src/audio*.o(.bss*);
#ifdef S2DEX_TEXT_ENGINE
      BUILD_DIR/src/s2d_engine*.o(.bss*);
#endif
#ifdef LIBPL
      BUILD_DIR/lib/libpl*.o(.bss*);
#endif
      *ULTRALIB.a:*.o(COMMON);
      *ULTRALIB.a:*.o(.scommon);
      *ULTRALIB.a:*.o(.bss*);
      */libhvqm2.a:*.o(.bss*);
      */libz.a:*.o(.bss*);
      . = ALIGN(0x8);
   }
   END_NOLOAD(main)

   . = _mainSegmentBssEnd;

   BEGIN_SEG(engine, .)
   {
      BUILD_DIR/src/game*.o(.text*);
      BUILD_DIR/src/game/behavior_actions.o(.text*);
      BUILD_DIR/src/game/obj_behaviors_2.o(.text*);
      BUILD_DIR/src/game/obj_behaviors.o(.text*);
      BUILD_DIR/src/game/object_helpers.o(.text*);
      BUILD_DIR/src/game/rendering_graph_node.o(.text*);
      BUILD_DIR/src/engine/math_util.o(.text*);
      BUILD_DIR/src/engine/surface_collision.o(.text*);
      BUILD_DIR/src/engine/surface_load.o(.text*);
      BUILD_DIR/src/engine/graph_node.o(.text*);
      BUILD_DIR/src/engine*.o(.text*);
      BUILD_DIR/src/hacktice*.o(.text*);
      _engineSegmentTextEnd = .;
      /* data */
      BUILD_DIR/src/game*.o(.data*);
      BUILD_DIR/src/engine*.o(.data*);
      BUILD_DIR/src/usb*.o(.data*);
      BUILD_DIR/src/hacktice*.o(.data*);
      /* rodata */
      BUILD_DIR/src/game*.o(.rodata*);
      BUILD_DIR/src/engine*.o(.rodata*);
      BUILD_DIR/src/usb*.o(.rodata*);
      BUILD_DIR/src/hacktice*.o(.rodata*);
      . = ALIGN(0x10);
   }
   END_SEG(engine)
   BEGIN_NOLOAD(engine)
   {
      BUILD_DIR/src/game/crash_screen.o(.bss*);
      BUILD_DIR/src/game/game_init.o(.bss*);
      BUILD_DIR/src/game/sound_init.o(.bss*);
      _hackticeStateDataStart0 = .;
      BUILD_DIR/src/game*.o(.bss*);
      _hackticeStateDataEnd0 = .;
      BUILD_DIR/src/engine*.o(.bss*);
      BUILD_DIR/src/hacktice*.o(.bss*);
      . = ALIGN(0x40);
   }
   END_NOLOAD(engine)

   _gp = . + 0x8000;
   BEGIN_SEG(sdata, .)
   {
      BUILD_DIR/src/game*.o(.sdata*);
      BUILD_DIR/src/audio*.o(.sdata*);
      BUILD_DIR/src/engine*.o(.sdata*);
      BUILD_DIR/src/usb*.o(.sdata*);
      BUILD_DIR/src/menu*.o(.sdata*);
      BUILD_DIR/src/hacktice*.o(.sdata*);
   }
   END_SEG(sdata)
   BEGIN_NOLOAD(sbss)
   {
      BUILD_DIR/src/game/crash_screen.o(.sdata*);
      BUILD_DIR/src/game/game_init.o(.sdata*);
      BUILD_DIR/src/game/sound_init.o(.sdata*);
      _hackticeStateDataStart1 = .;
      BUILD_DIR/src/game*.o(.sbss*);
      _hackticeStateDataEnd1 = .;
      BUILD_DIR/src/game*.o(.sbss*);
      BUILD_DIR/src/audio*.o(.sbss*);
      BUILD_DIR/src/engine*.o(.sbss*);
      BUILD_DIR/src/usb*.o(.sbss*);
      BUILD_DIR/src/menu*.o(.sbss*);
      BUILD_DIR/src/hacktice*.o(.sbss*);
      . = ALIGN(0x40);
   }
   END_NOLOAD(sbss)

   _poolStart = .;

   . = 0x80500000;
   BEGIN_NOLOAD(framebuffer0)
   {
      BUILD_DIR/src/buffers/framebuffer0.o(.bss*);
   }
   END_NOLOAD(framebuffer0)

   . = 0x80600000;
   BEGIN_NOLOAD(framebuffer1)
   {
      BUILD_DIR/src/buffers/framebuffer1.o(.bss*);
   }
   END_NOLOAD(framebuffer1)

   . = 0x80700000;
   BEGIN_NOLOAD(framebuffer2)
   {
      BUILD_DIR/src/buffers/framebuffer2.o(.bss*);
   }
   END_NOLOAD(framebuffer2)

   __expansionRamStart = (RAM_START + (RAM_1MB * 4));
#ifndef USE_EXT_RAM
   #ifndef PRELIMINARY
         ASSERT((. <= __expansionRamStart), "Error: RDRAM expanded into Expansion RAM, despite Expansion RAM not being defined.")
   #endif
#endif

   BEGIN_SEG(entry, 0x10000000)
   {
      BUILD_DIR/levels/entry.o(.data*);
   }
   END_SEG(entry)

   /* load YAY0 and level data */
   YAY0_SEG(segment2, 0x02000000)

#ifdef MULTILANG
   YAY0_SEG(translation_en, 0x1B000000)
#ifdef ENABLE_FRENCH
   YAY0_SEG(translation_fr, 0x1B000000)
#endif
#ifdef ENABLE_GERMAN
   YAY0_SEG(translation_de, 0x1B000000)
#endif
#ifdef ENABLE_JAPANESE
   YAY0_SEG(translation_jp, 0x1B000000)
#endif
#ifdef ENABLE_SPANISH
   YAY0_SEG(translation_es, 0x1B000000)
#endif
#endif

   /* mario actor group */
   STANDARD_OBJECTS(group0,  0x04000000, 0x17000000)

   /* load the other actor groups */
   STANDARD_OBJECTS(group1,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group2,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group3,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group4,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group5,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group6,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group7,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group8,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group9,  0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group10, 0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group11, 0x05000000, 0x0C000000)
   STANDARD_OBJECTS(group12, 0x06000000, 0x0D000000)
   STANDARD_OBJECTS(group13, 0x06000000, 0x0D000000)
   STANDARD_OBJECTS(group14, 0x06000000, 0x0D000000)
   STANDARD_OBJECTS(group15, 0x06000000, 0x0D000000)
   STANDARD_OBJECTS(group16, 0x06000000, 0x0D000000)
   STANDARD_OBJECTS(group17, 0x06000000, 0x0D000000)

   /* load the common actor groups */
   STANDARD_OBJECTS(common0, 0x08000000, 0x0F000000)
   STANDARD_OBJECTS(common1, 0x03000000, 0x16000000)

   VANILLA_OBJECTS(bob)
   VANILLA_OBJECTS(wf)
   VANILLA_OBJECTS(jrb)
   VANILLA_OBJECTS(ccm)
   VANILLA_OBJECTS(bbh)
   VANILLA_OBJECTS(hmc)
   VANILLA_OBJECTS(lll)
   VANILLA_OBJECTS(ssl)
   VANILLA_OBJECTS(ddd)
   VANILLA_OBJECTS(sl)
   VANILLA_OBJECTS(wdw)
   VANILLA_OBJECTS(ttm)
   VANILLA_OBJECTS(thi)
   VANILLA_OBJECTS(ttc)
   VANILLA_OBJECTS(rr)
   VANILLA_OBJECTS(bitdw)
   VANILLA_OBJECTS(bitfs)
   VANILLA_OBJECTS(bits)
   VANILLA_OBJECTS(vcutm)
   VANILLA_OBJECTS(castle_inside)
   VANILLA_OBJECTS(castle_grounds)

COMPRESSED_CONTENT(img1)
COMPRESSED_CONTENT(img2)
COMPRESSED_CONTENT(img3)
COMPRESSED_CONTENT(img4)
COMPRESSED_CONTENT(img5)
COMPRESSED_CONTENT(img6)
COMPRESSED_CONTENT(img7)
COMPRESSED_CONTENT(img8)
COMPRESSED_CONTENT(img9)
COMPRESSED_CONTENT(img10)
COMPRESSED_CONTENT(img11)
COMPRESSED_CONTENT(img12)
COMPRESSED_CONTENT(img13)
COMPRESSED_CONTENT(img14)
COMPRESSED_CONTENT(img15)
COMPRESSED_CONTENT(img16)
COMPRESSED_CONTENT(img17)
COMPRESSED_CONTENT(img18)
COMPRESSED_CONTENT(img19)
COMPRESSED_CONTENT(img20)
COMPRESSED_CONTENT(img21)
COMPRESSED_CONTENT(img22)
COMPRESSED_CONTENT(img23)
COMPRESSED_CONTENT(img24)
COMPRESSED_CONTENT(img25)
COMPRESSED_CONTENT(img26)
COMPRESSED_CONTENT(img27)
COMPRESSED_CONTENT(img28)
COMPRESSED_CONTENT(img29)
COMPRESSED_CONTENT(img30)
COMPRESSED_CONTENT(img31)
COMPRESSED_CONTENT(img32)
COMPRESSED_CONTENT(img33)
COMPRESSED_CONTENT(img34)
COMPRESSED_CONTENT(img35)
COMPRESSED_CONTENT(img36)
COMPRESSED_CONTENT(img37)
COMPRESSED_CONTENT(img38)
COMPRESSED_CONTENT(img39)
COMPRESSED_CONTENT(img40)
COMPRESSED_CONTENT(img41)
COMPRESSED_CONTENT(img42)
COMPRESSED_CONTENT(img43)
COMPRESSED_CONTENT(img44)
COMPRESSED_CONTENT(img45)
COMPRESSED_CONTENT(img46)
COMPRESSED_CONTENT(img47)
COMPRESSED_CONTENT(img48)
COMPRESSED_CONTENT(img49)
COMPRESSED_CONTENT(img50)
COMPRESSED_CONTENT(img51)
COMPRESSED_CONTENT(img52)
COMPRESSED_CONTENT(img53)
COMPRESSED_CONTENT(img54)
COMPRESSED_CONTENT(img55)
COMPRESSED_CONTENT(img56)
COMPRESSED_CONTENT(img57)
COMPRESSED_CONTENT(img58)
COMPRESSED_CONTENT(img59)
COMPRESSED_CONTENT(img60)
COMPRESSED_CONTENT(img61)
COMPRESSED_CONTENT(img62)
COMPRESSED_CONTENT(img63)
COMPRESSED_CONTENT(img64)
COMPRESSED_CONTENT(img65)
COMPRESSED_CONTENT(img66)
COMPRESSED_CONTENT(img67)
COMPRESSED_CONTENT(img68)
COMPRESSED_CONTENT(img69)
COMPRESSED_CONTENT(img70)
COMPRESSED_CONTENT(img71)
COMPRESSED_CONTENT(img72)
COMPRESSED_CONTENT(img73)
COMPRESSED_CONTENT(img74)
COMPRESSED_CONTENT(img75)
COMPRESSED_CONTENT(img76)
COMPRESSED_CONTENT(img77)
COMPRESSED_CONTENT(img78)
COMPRESSED_CONTENT(img79)
COMPRESSED_CONTENT(img80)
COMPRESSED_CONTENT(img81)
COMPRESSED_CONTENT(img82)
COMPRESSED_CONTENT(img83)
COMPRESSED_CONTENT(img84)
COMPRESSED_CONTENT(img85)
COMPRESSED_CONTENT(img86)
COMPRESSED_CONTENT(img87)
COMPRESSED_CONTENT(img88)
COMPRESSED_CONTENT(img89)
COMPRESSED_CONTENT(img90)
COMPRESSED_CONTENT(img91)
COMPRESSED_CONTENT(img92)
COMPRESSED_CONTENT(img93)
COMPRESSED_CONTENT(img94)
COMPRESSED_CONTENT(img95)
COMPRESSED_CONTENT(img96)
COMPRESSED_CONTENT(img97)
COMPRESSED_CONTENT(img98)
COMPRESSED_CONTENT(img99)
COMPRESSED_CONTENT(img100)
COMPRESSED_CONTENT(img101)
COMPRESSED_CONTENT(img102)
COMPRESSED_CONTENT(img103)
COMPRESSED_CONTENT(img104)
COMPRESSED_CONTENT(img105)
COMPRESSED_CONTENT(img106)
COMPRESSED_CONTENT(img107)
COMPRESSED_CONTENT(img108)
COMPRESSED_CONTENT(img109)
COMPRESSED_CONTENT(img110)
COMPRESSED_CONTENT(img111)
COMPRESSED_CONTENT(img112)
COMPRESSED_CONTENT(img113)
COMPRESSED_CONTENT(img114)
COMPRESSED_CONTENT(img115)
COMPRESSED_CONTENT(img116)
COMPRESSED_CONTENT(img117)
COMPRESSED_CONTENT(img118)
COMPRESSED_CONTENT(img119)
COMPRESSED_CONTENT(img120)
COMPRESSED_CONTENT(img121)
COMPRESSED_CONTENT(img122)
COMPRESSED_CONTENT(img123)
COMPRESSED_CONTENT(img124)
COMPRESSED_CONTENT(img125)
COMPRESSED_CONTENT(img126)
COMPRESSED_CONTENT(img127)
COMPRESSED_CONTENT(img128)
COMPRESSED_CONTENT(img129)
COMPRESSED_CONTENT(img130)
COMPRESSED_CONTENT(img131)
COMPRESSED_CONTENT(img132)
COMPRESSED_CONTENT(img133)
COMPRESSED_CONTENT(img134)
COMPRESSED_CONTENT(img135)
COMPRESSED_CONTENT(img136)
COMPRESSED_CONTENT(img137)
COMPRESSED_CONTENT(img138)
COMPRESSED_CONTENT(img139)
COMPRESSED_CONTENT(img140)
COMPRESSED_CONTENT(img141)
COMPRESSED_CONTENT(img142)
COMPRESSED_CONTENT(img143)
COMPRESSED_CONTENT(img144)
COMPRESSED_CONTENT(img145)
COMPRESSED_CONTENT(img146)
COMPRESSED_CONTENT(img147)
COMPRESSED_CONTENT(img148)
COMPRESSED_CONTENT(img149)
COMPRESSED_CONTENT(img150)
COMPRESSED_CONTENT(img151)
COMPRESSED_CONTENT(img152)
COMPRESSED_CONTENT(img153)
COMPRESSED_CONTENT(img154)
COMPRESSED_CONTENT(img155)
COMPRESSED_CONTENT(img156)
COMPRESSED_CONTENT(img157)
COMPRESSED_CONTENT(img158)
COMPRESSED_CONTENT(img159)
COMPRESSED_CONTENT(img160)
COMPRESSED_CONTENT(img161)
COMPRESSED_CONTENT(img162)
COMPRESSED_CONTENT(img163)
COMPRESSED_CONTENT(img164)
COMPRESSED_CONTENT(img165)
COMPRESSED_CONTENT(img166)
COMPRESSED_CONTENT(img167)
COMPRESSED_CONTENT(img168)
COMPRESSED_CONTENT(img169)
COMPRESSED_CONTENT(img170)
COMPRESSED_CONTENT(img171)
COMPRESSED_CONTENT(img172)
COMPRESSED_CONTENT(img173)
COMPRESSED_CONTENT(img174)
COMPRESSED_CONTENT(img175)
COMPRESSED_CONTENT(img176)
COMPRESSED_CONTENT(img177)
COMPRESSED_CONTENT(img178)
COMPRESSED_CONTENT(img179)
COMPRESSED_CONTENT(img180)
COMPRESSED_CONTENT(img181)
COMPRESSED_CONTENT(img182)
COMPRESSED_CONTENT(img183)
COMPRESSED_CONTENT(img184)
COMPRESSED_CONTENT(img185)
COMPRESSED_CONTENT(img186)
COMPRESSED_CONTENT(img187)
COMPRESSED_CONTENT(img188)
COMPRESSED_CONTENT(img189)
COMPRESSED_CONTENT(img190)
COMPRESSED_CONTENT(img191)
COMPRESSED_CONTENT(img192)
COMPRESSED_CONTENT(img193)
COMPRESSED_CONTENT(img194)
COMPRESSED_CONTENT(img195)
COMPRESSED_CONTENT(img196)
COMPRESSED_CONTENT(img197)
COMPRESSED_CONTENT(img198)
COMPRESSED_CONTENT(img199)
COMPRESSED_CONTENT(img200)
COMPRESSED_CONTENT(img201)
COMPRESSED_CONTENT(img202)
COMPRESSED_CONTENT(img203)
COMPRESSED_CONTENT(img204)
COMPRESSED_CONTENT(img205)
COMPRESSED_CONTENT(img206)
COMPRESSED_CONTENT(img207)
COMPRESSED_CONTENT(img208)
COMPRESSED_CONTENT(img209)
COMPRESSED_CONTENT(img210)
COMPRESSED_CONTENT(img211)
COMPRESSED_CONTENT(img212)
COMPRESSED_CONTENT(img213)
COMPRESSED_CONTENT(img214)
COMPRESSED_CONTENT(img215)
COMPRESSED_CONTENT(img216)
COMPRESSED_CONTENT(img217)
COMPRESSED_CONTENT(img218)
COMPRESSED_CONTENT(img219)
COMPRESSED_CONTENT(img220)
COMPRESSED_CONTENT(img221)
COMPRESSED_CONTENT(img222)
COMPRESSED_CONTENT(img223)
COMPRESSED_CONTENT(img224)
COMPRESSED_CONTENT(img225)
COMPRESSED_CONTENT(img226)
COMPRESSED_CONTENT(img227)
COMPRESSED_CONTENT(img228)
COMPRESSED_CONTENT(img229)
COMPRESSED_CONTENT(img230)
COMPRESSED_CONTENT(img231)
COMPRESSED_CONTENT(img232)
COMPRESSED_CONTENT(img233)
COMPRESSED_CONTENT(img234)
COMPRESSED_CONTENT(img235)
COMPRESSED_CONTENT(img236)
COMPRESSED_CONTENT(img237)
COMPRESSED_CONTENT(img238)
COMPRESSED_CONTENT(img239)
COMPRESSED_CONTENT(img240)
COMPRESSED_CONTENT(img241)
COMPRESSED_CONTENT(img242)
COMPRESSED_CONTENT(img243)
COMPRESSED_CONTENT(img244)
COMPRESSED_CONTENT(img245)
COMPRESSED_CONTENT(img246)
COMPRESSED_CONTENT(img247)
COMPRESSED_CONTENT(img248)
COMPRESSED_CONTENT(img249)
COMPRESSED_CONTENT(img250)
COMPRESSED_CONTENT(img251)
COMPRESSED_CONTENT(img252)
COMPRESSED_CONTENT(img253)
COMPRESSED_CONTENT(img254)
COMPRESSED_CONTENT(img255)
COMPRESSED_CONTENT(img256)
COMPRESSED_CONTENT(img257)
COMPRESSED_CONTENT(img258)
COMPRESSED_CONTENT(img259)
COMPRESSED_CONTENT(img260)
COMPRESSED_CONTENT(img261)
COMPRESSED_CONTENT(img262)
COMPRESSED_CONTENT(img263)
COMPRESSED_CONTENT(img264)
COMPRESSED_CONTENT(img265)
COMPRESSED_CONTENT(img266)
COMPRESSED_CONTENT(img267)
COMPRESSED_CONTENT(img268)
COMPRESSED_CONTENT(img269)
COMPRESSED_CONTENT(img270)
COMPRESSED_CONTENT(img271)
COMPRESSED_CONTENT(img272)
COMPRESSED_CONTENT(img273)
COMPRESSED_CONTENT(img274)
COMPRESSED_CONTENT(img275)
COMPRESSED_CONTENT(img276)
COMPRESSED_CONTENT(img277)
COMPRESSED_CONTENT(img278)
COMPRESSED_CONTENT(img279)
COMPRESSED_CONTENT(img280)
COMPRESSED_CONTENT(img281)
COMPRESSED_CONTENT(img282)
COMPRESSED_CONTENT(img283)
COMPRESSED_CONTENT(img284)
COMPRESSED_CONTENT(img285)
COMPRESSED_CONTENT(img286)
COMPRESSED_CONTENT(img287)
COMPRESSED_CONTENT(img288)
COMPRESSED_CONTENT(img289)
COMPRESSED_CONTENT(img290)
COMPRESSED_CONTENT(img291)
COMPRESSED_CONTENT(img292)
COMPRESSED_CONTENT(img293)
COMPRESSED_CONTENT(img294)
COMPRESSED_CONTENT(img295)
COMPRESSED_CONTENT(img296)
COMPRESSED_CONTENT(img297)
COMPRESSED_CONTENT(img298)
COMPRESSED_CONTENT(img299)
COMPRESSED_CONTENT(img300)
COMPRESSED_CONTENT(img301)
COMPRESSED_CONTENT(img302)
COMPRESSED_CONTENT(img303)
COMPRESSED_CONTENT(img304)
COMPRESSED_CONTENT(img305)
COMPRESSED_CONTENT(img306)
COMPRESSED_CONTENT(img307)
COMPRESSED_CONTENT(img308)
COMPRESSED_CONTENT(img309)
COMPRESSED_CONTENT(img310)
COMPRESSED_CONTENT(img311)
COMPRESSED_CONTENT(img312)
COMPRESSED_CONTENT(img313)
COMPRESSED_CONTENT(img314)
COMPRESSED_CONTENT(img315)
COMPRESSED_CONTENT(img316)
COMPRESSED_CONTENT(img317)
COMPRESSED_CONTENT(img318)
COMPRESSED_CONTENT(img319)
COMPRESSED_CONTENT(img320)
COMPRESSED_CONTENT(img321)
COMPRESSED_CONTENT(img322)
COMPRESSED_CONTENT(img323)
COMPRESSED_CONTENT(img324)
COMPRESSED_CONTENT(img325)
COMPRESSED_CONTENT(img326)
COMPRESSED_CONTENT(img327)
COMPRESSED_CONTENT(img328)
COMPRESSED_CONTENT(img329)
COMPRESSED_CONTENT(img330)
COMPRESSED_CONTENT(img331)
COMPRESSED_CONTENT(img332)
COMPRESSED_CONTENT(img333)

   /* use segmented addressing for behaviors */
   BEGIN_SEG(behavior, 0x13000000)
   {
      KEEP(BUILD_DIR/data/behavior_data.o(.data*));
      KEEP(BUILD_DIR/data/behavior_data.o(.rodata*));
      KEEP(BUILD_DIR/data/behavior_data.o(.text*));
   }
   END_SEG(behavior)
   BEGIN_NOLOAD(behavior)
   {
      KEEP(BUILD_DIR/data/behavior_data.o(.bss*));
   }
   END_NOLOAD(behavior)


   /* 0x8016F000 21D7D0-255EC0 [386F0] */
   BEGIN_SEG(goddard, (RAM_END - GODDARD_SIZE))
   {
      KEEP(BUILD_DIR/src/menu*.o(.text*));
      _goddardSegmentTextEnd = .;
      KEEP(BUILD_DIR/src/menu*.o(.data*));
      KEEP(BUILD_DIR/src/menu*.o(.rodata*));
#ifdef KEEP_MARIO_HEAD
      KEEP(BUILD_DIR/libgoddard.a:*.o(.text*));
      KEEP(BUILD_DIR/libgoddard.a:gd_main.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:draw_objects.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:objects.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:particles.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:dynlist_proc.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:debug_utils.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:joints.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:shape_helper.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:renderer.o(.data*));
      KEEP(BUILD_DIR/libgoddard.a:gd_main.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:gd_memory.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:draw_objects.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:objects.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:skin_movement.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:particles.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:dynlist_proc.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:old_menu.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:debug_utils.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:joints.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:skin.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:gd_math.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:shape_helper.o(.rodata*));
      KEEP(BUILD_DIR/libgoddard.a:renderer.o(.rodata*));
#endif
      . = ALIGN(16);
   }
   END_SEG(goddard)
   BEGIN_NOLOAD(goddard)
   {
      KEEP(BUILD_DIR/src/menu*.o(.bss*));
#ifdef KEEP_MARIO_HEAD
      KEEP(BUILD_DIR/libgoddard.a:*.o(.bss*));
#endif
      . = ALIGN(16);
   }
   END_NOLOAD(goddard)

#ifdef PRELIMINARY
      GODDARD_SIZE = SIZEOF(.goddard) + SIZEOF(.goddard.noload);
#else
#endif

   /* 0x268020 0x268020-0 [0] */
   BEGIN_SEG(intro, 0x14000000)
   {
      KEEP(BUILD_DIR/levels/intro/script.o(.data));
      KEEP(BUILD_DIR/levels/intro/script.o(.rodata*));
      KEEP(BUILD_DIR/levels/intro/script.o(.text*));
      KEEP(BUILD_DIR/levels/intro/geo.o(.data*));
   }
   END_SEG(intro)
   BEGIN_NOLOAD(intro)
   {
      KEEP(BUILD_DIR/levels/intro/script.o(.bss*));
   }
   END_NOLOAD(intro)
   BEGIN_SEG(intro_segment_7, 0x07000000)
   {
      KEEP(BUILD_DIR/levels/intro/leveldata.szp.o(.data*));
      . = ALIGN(0x10);
   }
   END_SEG(intro_segment_7)

   YAY0_SEG(debug_level_select, 0x07000000)
   YAY0_SEG(title_screen_bg,    0x0A000000)

#ifdef KEEP_MARIO_HEAD
   BEGIN_SEG(gd_dynlists, 0x04000000)
   {
      BUILD_DIR/libgoddard.a:dynlist_test_cube.o(.data*);
      BUILD_DIR/libgoddard.a:dynlist_unused.o(.data*);
      BUILD_DIR/libgoddard.a:dynlist_mario_face.o(.data*);
      BUILD_DIR/libgoddard.a:dynlists_mario_eyes.o(.data*);
      BUILD_DIR/libgoddard.a:dynlists_mario_eyebrows_mustache.o(.data*);
      BUILD_DIR/libgoddard.a:dynlist_mario_master.o(.data*);
      BUILD_DIR/libgoddard.a:anim_mario_mustache_right.o(.data*);
      BUILD_DIR/libgoddard.a:anim_mario_mustache_left.o(.data*);
      BUILD_DIR/libgoddard.a:anim_mario_lips_1.o(.data*);
      BUILD_DIR/libgoddard.a:anim_mario_lips_2.o(.data*);
      BUILD_DIR/libgoddard.a:anim_mario_eyebrows_1.o(.data*);
      BUILD_DIR/libgoddard.a:anim_group_1.o(.data*);
      BUILD_DIR/libgoddard.a:anim_group_2.o(.data*);
      BUILD_DIR/libgoddard.a:dynlist_test_cube.o(.rodata*);
      BUILD_DIR/libgoddard.a:dynlist_unused.o(.rodata*);
      BUILD_DIR/libgoddard.a:*.o(.data*);
      BUILD_DIR/libgoddard.a:*.o(.rodata*);
   }
   END_SEG(gd_dynlists)
#endif

   gMainLevels = __romPos;

   BEGIN_SEG(menu, 0x14000000)
   {
      KEEP(BUILD_DIR/levels/menu/script.o(.data*));
      KEEP(BUILD_DIR/levels/menu/geo.o(.data*));
   }
   END_SEG(menu)
   BEGIN_SEG(menu_segment_7, 0x07000000)
   {
      KEEP(BUILD_DIR/levels/menu/leveldata.szp.o(.data*));
      . = ALIGN(0x10);
   }
   END_SEG(menu_segment_7)

   BEGIN_SEG(scripts, 0x15000000)
   {
      BUILD_DIR/levels/scripts.o(.data*);
   }
   END_SEG(scripts)

   YAY0_SEG(water_skybox,       0x0A000000)
   YAY0_SEG(ccm_skybox,         0x0A000000)
   YAY0_SEG(clouds_skybox,      0x0A000000)
   YAY0_SEG(bitfs_skybox,       0x0A000000)
   YAY0_SEG(wdw_skybox,         0x0A000000)
   YAY0_SEG(cloud_floor_skybox, 0x0A000000)
   YAY0_SEG(ssl_skybox,         0x0A000000)
   YAY0_SEG(bbh_skybox,         0x0A000000)
   YAY0_SEG(bidw_skybox,        0x0A000000)
   YAY0_SEG(bits_skybox,        0x0A000000)

   // Texture bins
   YAY0_SEG(fire,     0x09000000)
   YAY0_SEG(spooky,   0x09000000)
   YAY0_SEG(generic,  0x09000000)
   YAY0_SEG(water,    0x09000000)
   YAY0_SEG(sky,      0x09000000)
   YAY0_SEG(snow,     0x09000000)
   YAY0_SEG(cave,     0x09000000)
   YAY0_SEG(machine,  0x09000000)
   YAY0_SEG(mountain, 0x09000000)
   YAY0_SEG(grass,    0x09000000)
   YAY0_SEG(outside,  0x09000000)
   YAY0_SEG(inside,   0x09000000)
   YAY0_SEG(effect,   0x0B000000)

#define STUB_LEVEL(_0, _1, _2, _3, _4, _5, _6, _7, _8)
#define DEFINE_LEVEL(_0, _1, _2, folder, _4, _5, _6, _7, _8, _9, _10) STANDARD_LEVEL(folder)

   #include "levels/level_defines.h"

#undef STUB_LEVEL
#undef DEFINE_LEVEL

   /* 4E9FA0-? [?] */
   BEGIN_SEG(assets, __romPos)
   {
      KEEP(BUILD_DIR/assets/mario_anim_data.o(.data*));
      KEEP(BUILD_DIR/assets/mario_anim_data.o(.rodata*));
      KEEP(BUILD_DIR/assets/demo_data.o(.data*));
      KEEP(BUILD_DIR/sound/sound_data.o(.data*));
   }
   END_SEG(assets)
#ifdef HVQM
   BEGIN_SEG(capcom, __romPos) SUBALIGN(2)
   {
      KEEP(BUILD_DIR/data/capcom.o(.data*));
   }
	END_SEG(capcom)
#endif

#ifdef DEBUG_MAP_STACKTRACE
   BEGIN_SEG(mapData, (RAM_END - 0x00100000)) {
      KEEP(BUILD_DIR/asm/debug/map.o(.data*));
   }
   END_SEG(mapData)
#endif
   /* DWARF debug sections.
      Symbols in the DWARF debugging sections are relative to the beginning
      of the section so we begin them at 0.  */
   /* DWARF 1 */
   .debug          0 : { *(.debug) }
   .line           0 : { *(.line) }
   /* GNU DWARF 1 extensions */
   .debug_srcinfo  0 : { *(.debug_srcinfo) }
   .debug_sfnames  0 : { *(.debug_sfnames) }
   /* DWARF 1.1 and DWARF 2 */
   .debug_aranges  0 : { *(.debug_aranges) }
   .debug_pubnames 0 : { *(.debug_pubnames) }
   /* DWARF 2 */
   .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
   .debug_abbrev   0 : { *(.debug_abbrev) }
   .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
   .debug_frame    0 : { *(.debug_frame) }
   .debug_str      0 : { *(.debug_str) }
   .debug_loc      0 : { *(.debug_loc) }
   .debug_macinfo  0 : { *(.debug_macinfo) }
   /* SGI/MIPS DWARF 2 extensions */
   .debug_weaknames 0 : { *(.debug_weaknames) }
   .debug_funcnames 0 : { *(.debug_funcnames) }
   .debug_typenames 0 : { *(.debug_typenames) }
   .debug_varnames  0 : { *(.debug_varnames) }
   /* DWARF 3 */
   .debug_pubtypes 0 : { *(.debug_pubtypes) }
   .debug_ranges   0 : { *(.debug_ranges) }
   /* DWARF Extension.  */
   .debug_macro    0 : { *(.debug_macro) }
   .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
   /* Discard everything not specifically mentioned above. */
   /DISCARD/ :
   {
      *(*);
   }
}
