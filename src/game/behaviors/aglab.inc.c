#include "game/puppyprint.h"
#include "game/emutest.h"

#define TEXTURES_COUNTS 3
static ALIGNED32 u8 sTextures[TEXTURES_COUNTS][0x25800 + 32];
u8* sMovieTexture = NULL;

#define PIC_DECLARE(name) extern u8 _cc_##name##_yay0SegmentRomStart[]; extern u8 _cc_##name##_yay0SegmentRomEnd[];

PIC_DECLARE(img1)
PIC_DECLARE(img2)
PIC_DECLARE(img3)
PIC_DECLARE(img4)
PIC_DECLARE(img5)
PIC_DECLARE(img6)
PIC_DECLARE(img7)
PIC_DECLARE(img8)
PIC_DECLARE(img9)
PIC_DECLARE(img10)
PIC_DECLARE(img11)
PIC_DECLARE(img12)
PIC_DECLARE(img13)
PIC_DECLARE(img14)
PIC_DECLARE(img15)
PIC_DECLARE(img16)
PIC_DECLARE(img17)
PIC_DECLARE(img18)
PIC_DECLARE(img19)
PIC_DECLARE(img20)
PIC_DECLARE(img21)
PIC_DECLARE(img22)
PIC_DECLARE(img23)
PIC_DECLARE(img24)
PIC_DECLARE(img25)
PIC_DECLARE(img26)
PIC_DECLARE(img27)
PIC_DECLARE(img28)
PIC_DECLARE(img29)
PIC_DECLARE(img30)
PIC_DECLARE(img31)
PIC_DECLARE(img32)
PIC_DECLARE(img33)
PIC_DECLARE(img34)
PIC_DECLARE(img35)
PIC_DECLARE(img36)
PIC_DECLARE(img37)
PIC_DECLARE(img38)
PIC_DECLARE(img39)
PIC_DECLARE(img40)
PIC_DECLARE(img41)
PIC_DECLARE(img42)
PIC_DECLARE(img43)
PIC_DECLARE(img44)
PIC_DECLARE(img45)
PIC_DECLARE(img46)
PIC_DECLARE(img47)
PIC_DECLARE(img48)
PIC_DECLARE(img49)
PIC_DECLARE(img50)
PIC_DECLARE(img51)
PIC_DECLARE(img52)
PIC_DECLARE(img53)
PIC_DECLARE(img54)
PIC_DECLARE(img55)
PIC_DECLARE(img56)
PIC_DECLARE(img57)
PIC_DECLARE(img58)
PIC_DECLARE(img59)
PIC_DECLARE(img60)
PIC_DECLARE(img61)
PIC_DECLARE(img62)
PIC_DECLARE(img63)
PIC_DECLARE(img64)
PIC_DECLARE(img65)
PIC_DECLARE(img66)
PIC_DECLARE(img67)
PIC_DECLARE(img68)
PIC_DECLARE(img69)
PIC_DECLARE(img70)
PIC_DECLARE(img71)
PIC_DECLARE(img72)
PIC_DECLARE(img73)
PIC_DECLARE(img74)
PIC_DECLARE(img75)
PIC_DECLARE(img76)
PIC_DECLARE(img77)
PIC_DECLARE(img78)
PIC_DECLARE(img79)
PIC_DECLARE(img80)
PIC_DECLARE(img81)
PIC_DECLARE(img82)
PIC_DECLARE(img83)
PIC_DECLARE(img84)
PIC_DECLARE(img85)
PIC_DECLARE(img86)
PIC_DECLARE(img87)
PIC_DECLARE(img88)
PIC_DECLARE(img89)
PIC_DECLARE(img90)
PIC_DECLARE(img91)
PIC_DECLARE(img92)
PIC_DECLARE(img93)
PIC_DECLARE(img94)
PIC_DECLARE(img95)
PIC_DECLARE(img96)
PIC_DECLARE(img97)
PIC_DECLARE(img98)
PIC_DECLARE(img99)
PIC_DECLARE(img100)
PIC_DECLARE(img101)
PIC_DECLARE(img102)
PIC_DECLARE(img103)
PIC_DECLARE(img104)
PIC_DECLARE(img105)
PIC_DECLARE(img106)
PIC_DECLARE(img107)
PIC_DECLARE(img108)
PIC_DECLARE(img109)
PIC_DECLARE(img110)
PIC_DECLARE(img111)
PIC_DECLARE(img112)
PIC_DECLARE(img113)
PIC_DECLARE(img114)
PIC_DECLARE(img115)
PIC_DECLARE(img116)
PIC_DECLARE(img117)
PIC_DECLARE(img118)
PIC_DECLARE(img119)
PIC_DECLARE(img120)
PIC_DECLARE(img121)
PIC_DECLARE(img122)
PIC_DECLARE(img123)
PIC_DECLARE(img124)
PIC_DECLARE(img125)
PIC_DECLARE(img126)
PIC_DECLARE(img127)
PIC_DECLARE(img128)
PIC_DECLARE(img129)
PIC_DECLARE(img130)
PIC_DECLARE(img131)
PIC_DECLARE(img132)
PIC_DECLARE(img133)
PIC_DECLARE(img134)
PIC_DECLARE(img135)
PIC_DECLARE(img136)
PIC_DECLARE(img137)
PIC_DECLARE(img138)
PIC_DECLARE(img139)
PIC_DECLARE(img140)
PIC_DECLARE(img141)
PIC_DECLARE(img142)
PIC_DECLARE(img143)
PIC_DECLARE(img144)
PIC_DECLARE(img145)
PIC_DECLARE(img146)
PIC_DECLARE(img147)
PIC_DECLARE(img148)
PIC_DECLARE(img149)
PIC_DECLARE(img150)
PIC_DECLARE(img151)
PIC_DECLARE(img152)
PIC_DECLARE(img153)
PIC_DECLARE(img154)
PIC_DECLARE(img155)
PIC_DECLARE(img156)
PIC_DECLARE(img157)
PIC_DECLARE(img158)
PIC_DECLARE(img159)
PIC_DECLARE(img160)
PIC_DECLARE(img161)
PIC_DECLARE(img162)
PIC_DECLARE(img163)
PIC_DECLARE(img164)
PIC_DECLARE(img165)
PIC_DECLARE(img166)
PIC_DECLARE(img167)
PIC_DECLARE(img168)
PIC_DECLARE(img169)
PIC_DECLARE(img170)
PIC_DECLARE(img171)
PIC_DECLARE(img172)
PIC_DECLARE(img173)
PIC_DECLARE(img174)
PIC_DECLARE(img175)
PIC_DECLARE(img176)
PIC_DECLARE(img177)
PIC_DECLARE(img178)
PIC_DECLARE(img179)
PIC_DECLARE(img180)
PIC_DECLARE(img181)
PIC_DECLARE(img182)
PIC_DECLARE(img183)
PIC_DECLARE(img184)
PIC_DECLARE(img185)
PIC_DECLARE(img186)
PIC_DECLARE(img187)
PIC_DECLARE(img188)
PIC_DECLARE(img189)
PIC_DECLARE(img190)
PIC_DECLARE(img191)
PIC_DECLARE(img192)
PIC_DECLARE(img193)
PIC_DECLARE(img194)
PIC_DECLARE(img195)
PIC_DECLARE(img196)
PIC_DECLARE(img197)
PIC_DECLARE(img198)
PIC_DECLARE(img199)
PIC_DECLARE(img200)
PIC_DECLARE(img201)
PIC_DECLARE(img202)
PIC_DECLARE(img203)
PIC_DECLARE(img204)
PIC_DECLARE(img205)
PIC_DECLARE(img206)
PIC_DECLARE(img207)
PIC_DECLARE(img208)
PIC_DECLARE(img209)
PIC_DECLARE(img210)
PIC_DECLARE(img211)
PIC_DECLARE(img212)
PIC_DECLARE(img213)
PIC_DECLARE(img214)
PIC_DECLARE(img215)
PIC_DECLARE(img216)
PIC_DECLARE(img217)
PIC_DECLARE(img218)
PIC_DECLARE(img219)
PIC_DECLARE(img220)
PIC_DECLARE(img221)
PIC_DECLARE(img222)
PIC_DECLARE(img223)
PIC_DECLARE(img224)
PIC_DECLARE(img225)
PIC_DECLARE(img226)
PIC_DECLARE(img227)
PIC_DECLARE(img228)
PIC_DECLARE(img229)
PIC_DECLARE(img230)
PIC_DECLARE(img231)
PIC_DECLARE(img232)
PIC_DECLARE(img233)
PIC_DECLARE(img234)
PIC_DECLARE(img235)
PIC_DECLARE(img236)
PIC_DECLARE(img237)
PIC_DECLARE(img238)
PIC_DECLARE(img239)
PIC_DECLARE(img240)
PIC_DECLARE(img241)
PIC_DECLARE(img242)
PIC_DECLARE(img243)
PIC_DECLARE(img244)
PIC_DECLARE(img245)
PIC_DECLARE(img246)
PIC_DECLARE(img247)
PIC_DECLARE(img248)
PIC_DECLARE(img249)
PIC_DECLARE(img250)
PIC_DECLARE(img251)
PIC_DECLARE(img252)
PIC_DECLARE(img253)
PIC_DECLARE(img254)
PIC_DECLARE(img255)
PIC_DECLARE(img256)
PIC_DECLARE(img257)
PIC_DECLARE(img258)
PIC_DECLARE(img259)
PIC_DECLARE(img260)
PIC_DECLARE(img261)
PIC_DECLARE(img262)
PIC_DECLARE(img263)
PIC_DECLARE(img264)
PIC_DECLARE(img265)
PIC_DECLARE(img266)
PIC_DECLARE(img267)
PIC_DECLARE(img268)
PIC_DECLARE(img269)
PIC_DECLARE(img270)
PIC_DECLARE(img271)
PIC_DECLARE(img272)
PIC_DECLARE(img273)
PIC_DECLARE(img274)
PIC_DECLARE(img275)
PIC_DECLARE(img276)
PIC_DECLARE(img277)
PIC_DECLARE(img278)
PIC_DECLARE(img279)
PIC_DECLARE(img280)
PIC_DECLARE(img281)
PIC_DECLARE(img282)
PIC_DECLARE(img283)
PIC_DECLARE(img284)
PIC_DECLARE(img285)
PIC_DECLARE(img286)
PIC_DECLARE(img287)
PIC_DECLARE(img288)
PIC_DECLARE(img289)
PIC_DECLARE(img290)
PIC_DECLARE(img291)
PIC_DECLARE(img292)
PIC_DECLARE(img293)
PIC_DECLARE(img294)
PIC_DECLARE(img295)
PIC_DECLARE(img296)
PIC_DECLARE(img297)
PIC_DECLARE(img298)
PIC_DECLARE(img299)
PIC_DECLARE(img300)
PIC_DECLARE(img301)
PIC_DECLARE(img302)
PIC_DECLARE(img303)
PIC_DECLARE(img304)
PIC_DECLARE(img305)
PIC_DECLARE(img306)
PIC_DECLARE(img307)
PIC_DECLARE(img308)
PIC_DECLARE(img309)
PIC_DECLARE(img310)
PIC_DECLARE(img311)
PIC_DECLARE(img312)
PIC_DECLARE(img313)
PIC_DECLARE(img314)
PIC_DECLARE(img315)
PIC_DECLARE(img316)
PIC_DECLARE(img317)
PIC_DECLARE(img318)
PIC_DECLARE(img319)
PIC_DECLARE(img320)
PIC_DECLARE(img321)
PIC_DECLARE(img322)
PIC_DECLARE(img323)
PIC_DECLARE(img324)
PIC_DECLARE(img325)
PIC_DECLARE(img326)
PIC_DECLARE(img327)
PIC_DECLARE(img328)
PIC_DECLARE(img329)
PIC_DECLARE(img330)
PIC_DECLARE(img331)
PIC_DECLARE(img332)
PIC_DECLARE(img333)

struct MovieFrame
{
    u8* start;
    u8* end;
};

const struct MovieFrame sMovieFrames[] = {
    { _cc_img1_yay0SegmentRomStart, _cc_img1_yay0SegmentRomEnd },
    { _cc_img2_yay0SegmentRomStart, _cc_img2_yay0SegmentRomEnd },
    { _cc_img3_yay0SegmentRomStart, _cc_img3_yay0SegmentRomEnd },
    { _cc_img4_yay0SegmentRomStart, _cc_img4_yay0SegmentRomEnd },
    { _cc_img5_yay0SegmentRomStart, _cc_img5_yay0SegmentRomEnd },
    { _cc_img6_yay0SegmentRomStart, _cc_img6_yay0SegmentRomEnd },
    { _cc_img7_yay0SegmentRomStart, _cc_img7_yay0SegmentRomEnd },
    { _cc_img8_yay0SegmentRomStart, _cc_img8_yay0SegmentRomEnd },
    { _cc_img9_yay0SegmentRomStart, _cc_img9_yay0SegmentRomEnd },
    { _cc_img10_yay0SegmentRomStart, _cc_img10_yay0SegmentRomEnd },
    { _cc_img11_yay0SegmentRomStart, _cc_img11_yay0SegmentRomEnd },
    { _cc_img12_yay0SegmentRomStart, _cc_img12_yay0SegmentRomEnd },
    { _cc_img13_yay0SegmentRomStart, _cc_img13_yay0SegmentRomEnd },
    { _cc_img14_yay0SegmentRomStart, _cc_img14_yay0SegmentRomEnd },
    { _cc_img15_yay0SegmentRomStart, _cc_img15_yay0SegmentRomEnd },
    { _cc_img16_yay0SegmentRomStart, _cc_img16_yay0SegmentRomEnd },
    { _cc_img17_yay0SegmentRomStart, _cc_img17_yay0SegmentRomEnd },
    { _cc_img18_yay0SegmentRomStart, _cc_img18_yay0SegmentRomEnd },
    { _cc_img19_yay0SegmentRomStart, _cc_img19_yay0SegmentRomEnd },
    { _cc_img20_yay0SegmentRomStart, _cc_img20_yay0SegmentRomEnd },
    { _cc_img21_yay0SegmentRomStart, _cc_img21_yay0SegmentRomEnd },
    { _cc_img22_yay0SegmentRomStart, _cc_img22_yay0SegmentRomEnd },
    { _cc_img23_yay0SegmentRomStart, _cc_img23_yay0SegmentRomEnd },
    { _cc_img24_yay0SegmentRomStart, _cc_img24_yay0SegmentRomEnd },
    { _cc_img25_yay0SegmentRomStart, _cc_img25_yay0SegmentRomEnd },
    { _cc_img26_yay0SegmentRomStart, _cc_img26_yay0SegmentRomEnd },
    { _cc_img27_yay0SegmentRomStart, _cc_img27_yay0SegmentRomEnd },
    { _cc_img28_yay0SegmentRomStart, _cc_img28_yay0SegmentRomEnd },
    { _cc_img29_yay0SegmentRomStart, _cc_img29_yay0SegmentRomEnd },
    { _cc_img30_yay0SegmentRomStart, _cc_img30_yay0SegmentRomEnd },
    { _cc_img31_yay0SegmentRomStart, _cc_img31_yay0SegmentRomEnd },
    { _cc_img32_yay0SegmentRomStart, _cc_img32_yay0SegmentRomEnd },
    { _cc_img33_yay0SegmentRomStart, _cc_img33_yay0SegmentRomEnd },
    { _cc_img34_yay0SegmentRomStart, _cc_img34_yay0SegmentRomEnd },
    { _cc_img35_yay0SegmentRomStart, _cc_img35_yay0SegmentRomEnd },
    { _cc_img36_yay0SegmentRomStart, _cc_img36_yay0SegmentRomEnd },
    { _cc_img37_yay0SegmentRomStart, _cc_img37_yay0SegmentRomEnd },
    { _cc_img38_yay0SegmentRomStart, _cc_img38_yay0SegmentRomEnd },
    { _cc_img39_yay0SegmentRomStart, _cc_img39_yay0SegmentRomEnd },
    { _cc_img40_yay0SegmentRomStart, _cc_img40_yay0SegmentRomEnd },
    { _cc_img41_yay0SegmentRomStart, _cc_img41_yay0SegmentRomEnd },
    { _cc_img42_yay0SegmentRomStart, _cc_img42_yay0SegmentRomEnd },
    { _cc_img43_yay0SegmentRomStart, _cc_img43_yay0SegmentRomEnd },
    { _cc_img44_yay0SegmentRomStart, _cc_img44_yay0SegmentRomEnd },
    { _cc_img45_yay0SegmentRomStart, _cc_img45_yay0SegmentRomEnd },
    { _cc_img46_yay0SegmentRomStart, _cc_img46_yay0SegmentRomEnd },
    { _cc_img47_yay0SegmentRomStart, _cc_img47_yay0SegmentRomEnd },
    { _cc_img48_yay0SegmentRomStart, _cc_img48_yay0SegmentRomEnd },
    { _cc_img49_yay0SegmentRomStart, _cc_img49_yay0SegmentRomEnd },
    { _cc_img50_yay0SegmentRomStart, _cc_img50_yay0SegmentRomEnd },
    { _cc_img51_yay0SegmentRomStart, _cc_img51_yay0SegmentRomEnd },
    { _cc_img52_yay0SegmentRomStart, _cc_img52_yay0SegmentRomEnd },
    { _cc_img53_yay0SegmentRomStart, _cc_img53_yay0SegmentRomEnd },
    { _cc_img54_yay0SegmentRomStart, _cc_img54_yay0SegmentRomEnd },
    { _cc_img55_yay0SegmentRomStart, _cc_img55_yay0SegmentRomEnd },
    { _cc_img56_yay0SegmentRomStart, _cc_img56_yay0SegmentRomEnd },
    { _cc_img57_yay0SegmentRomStart, _cc_img57_yay0SegmentRomEnd },
    { _cc_img58_yay0SegmentRomStart, _cc_img58_yay0SegmentRomEnd },
    { _cc_img59_yay0SegmentRomStart, _cc_img59_yay0SegmentRomEnd },
    { _cc_img60_yay0SegmentRomStart, _cc_img60_yay0SegmentRomEnd },
    { _cc_img61_yay0SegmentRomStart, _cc_img61_yay0SegmentRomEnd },
    { _cc_img62_yay0SegmentRomStart, _cc_img62_yay0SegmentRomEnd },
    { _cc_img63_yay0SegmentRomStart, _cc_img63_yay0SegmentRomEnd },
    { _cc_img64_yay0SegmentRomStart, _cc_img64_yay0SegmentRomEnd },
    { _cc_img65_yay0SegmentRomStart, _cc_img65_yay0SegmentRomEnd },
    { _cc_img66_yay0SegmentRomStart, _cc_img66_yay0SegmentRomEnd },
    { _cc_img67_yay0SegmentRomStart, _cc_img67_yay0SegmentRomEnd },
    { _cc_img68_yay0SegmentRomStart, _cc_img68_yay0SegmentRomEnd },
    { _cc_img69_yay0SegmentRomStart, _cc_img69_yay0SegmentRomEnd },
    { _cc_img70_yay0SegmentRomStart, _cc_img70_yay0SegmentRomEnd },
    { _cc_img71_yay0SegmentRomStart, _cc_img71_yay0SegmentRomEnd },
    { _cc_img72_yay0SegmentRomStart, _cc_img72_yay0SegmentRomEnd },
    { _cc_img73_yay0SegmentRomStart, _cc_img73_yay0SegmentRomEnd },
    { _cc_img74_yay0SegmentRomStart, _cc_img74_yay0SegmentRomEnd },
    { _cc_img75_yay0SegmentRomStart, _cc_img75_yay0SegmentRomEnd },
    { _cc_img76_yay0SegmentRomStart, _cc_img76_yay0SegmentRomEnd },
    { _cc_img77_yay0SegmentRomStart, _cc_img77_yay0SegmentRomEnd },
    { _cc_img78_yay0SegmentRomStart, _cc_img78_yay0SegmentRomEnd },
    { _cc_img79_yay0SegmentRomStart, _cc_img79_yay0SegmentRomEnd },
    { _cc_img80_yay0SegmentRomStart, _cc_img80_yay0SegmentRomEnd },
    { _cc_img81_yay0SegmentRomStart, _cc_img81_yay0SegmentRomEnd },
    { _cc_img82_yay0SegmentRomStart, _cc_img82_yay0SegmentRomEnd },
    { _cc_img83_yay0SegmentRomStart, _cc_img83_yay0SegmentRomEnd },
    { _cc_img84_yay0SegmentRomStart, _cc_img84_yay0SegmentRomEnd },
    { _cc_img85_yay0SegmentRomStart, _cc_img85_yay0SegmentRomEnd },
    { _cc_img86_yay0SegmentRomStart, _cc_img86_yay0SegmentRomEnd },
    { _cc_img87_yay0SegmentRomStart, _cc_img87_yay0SegmentRomEnd },
    { _cc_img88_yay0SegmentRomStart, _cc_img88_yay0SegmentRomEnd },
    { _cc_img89_yay0SegmentRomStart, _cc_img89_yay0SegmentRomEnd },
    { _cc_img90_yay0SegmentRomStart, _cc_img90_yay0SegmentRomEnd },
    { _cc_img91_yay0SegmentRomStart, _cc_img91_yay0SegmentRomEnd },
    { _cc_img92_yay0SegmentRomStart, _cc_img92_yay0SegmentRomEnd },
    { _cc_img93_yay0SegmentRomStart, _cc_img93_yay0SegmentRomEnd },
    { _cc_img94_yay0SegmentRomStart, _cc_img94_yay0SegmentRomEnd },
    { _cc_img95_yay0SegmentRomStart, _cc_img95_yay0SegmentRomEnd },
    { _cc_img96_yay0SegmentRomStart, _cc_img96_yay0SegmentRomEnd },
    { _cc_img97_yay0SegmentRomStart, _cc_img97_yay0SegmentRomEnd },
    { _cc_img98_yay0SegmentRomStart, _cc_img98_yay0SegmentRomEnd },
    { _cc_img99_yay0SegmentRomStart, _cc_img99_yay0SegmentRomEnd },
    { _cc_img100_yay0SegmentRomStart, _cc_img100_yay0SegmentRomEnd },
    { _cc_img101_yay0SegmentRomStart, _cc_img101_yay0SegmentRomEnd },
    { _cc_img102_yay0SegmentRomStart, _cc_img102_yay0SegmentRomEnd },
    { _cc_img103_yay0SegmentRomStart, _cc_img103_yay0SegmentRomEnd },
    { _cc_img104_yay0SegmentRomStart, _cc_img104_yay0SegmentRomEnd },
    { _cc_img105_yay0SegmentRomStart, _cc_img105_yay0SegmentRomEnd },
    { _cc_img106_yay0SegmentRomStart, _cc_img106_yay0SegmentRomEnd },
    { _cc_img107_yay0SegmentRomStart, _cc_img107_yay0SegmentRomEnd },
    { _cc_img108_yay0SegmentRomStart, _cc_img108_yay0SegmentRomEnd },
    { _cc_img109_yay0SegmentRomStart, _cc_img109_yay0SegmentRomEnd },
    { _cc_img110_yay0SegmentRomStart, _cc_img110_yay0SegmentRomEnd },
    { _cc_img111_yay0SegmentRomStart, _cc_img111_yay0SegmentRomEnd },
    { _cc_img112_yay0SegmentRomStart, _cc_img112_yay0SegmentRomEnd },
    { _cc_img113_yay0SegmentRomStart, _cc_img113_yay0SegmentRomEnd },
    { _cc_img114_yay0SegmentRomStart, _cc_img114_yay0SegmentRomEnd },
    { _cc_img115_yay0SegmentRomStart, _cc_img115_yay0SegmentRomEnd },
    { _cc_img116_yay0SegmentRomStart, _cc_img116_yay0SegmentRomEnd },
    { _cc_img117_yay0SegmentRomStart, _cc_img117_yay0SegmentRomEnd },
    { _cc_img118_yay0SegmentRomStart, _cc_img118_yay0SegmentRomEnd },
    { _cc_img119_yay0SegmentRomStart, _cc_img119_yay0SegmentRomEnd },
    { _cc_img120_yay0SegmentRomStart, _cc_img120_yay0SegmentRomEnd },
    { _cc_img121_yay0SegmentRomStart, _cc_img121_yay0SegmentRomEnd },
    { _cc_img122_yay0SegmentRomStart, _cc_img122_yay0SegmentRomEnd },
    { _cc_img123_yay0SegmentRomStart, _cc_img123_yay0SegmentRomEnd },
    { _cc_img124_yay0SegmentRomStart, _cc_img124_yay0SegmentRomEnd },
    { _cc_img125_yay0SegmentRomStart, _cc_img125_yay0SegmentRomEnd },
    { _cc_img126_yay0SegmentRomStart, _cc_img126_yay0SegmentRomEnd },
    { _cc_img127_yay0SegmentRomStart, _cc_img127_yay0SegmentRomEnd },
    { _cc_img128_yay0SegmentRomStart, _cc_img128_yay0SegmentRomEnd },
    { _cc_img129_yay0SegmentRomStart, _cc_img129_yay0SegmentRomEnd },
    { _cc_img130_yay0SegmentRomStart, _cc_img130_yay0SegmentRomEnd },
    { _cc_img131_yay0SegmentRomStart, _cc_img131_yay0SegmentRomEnd },
    { _cc_img132_yay0SegmentRomStart, _cc_img132_yay0SegmentRomEnd },
    { _cc_img133_yay0SegmentRomStart, _cc_img133_yay0SegmentRomEnd },
    { _cc_img134_yay0SegmentRomStart, _cc_img134_yay0SegmentRomEnd },
    { _cc_img135_yay0SegmentRomStart, _cc_img135_yay0SegmentRomEnd },
    { _cc_img136_yay0SegmentRomStart, _cc_img136_yay0SegmentRomEnd },
    { _cc_img137_yay0SegmentRomStart, _cc_img137_yay0SegmentRomEnd },
    { _cc_img138_yay0SegmentRomStart, _cc_img138_yay0SegmentRomEnd },
    { _cc_img139_yay0SegmentRomStart, _cc_img139_yay0SegmentRomEnd },
    { _cc_img140_yay0SegmentRomStart, _cc_img140_yay0SegmentRomEnd },
    { _cc_img141_yay0SegmentRomStart, _cc_img141_yay0SegmentRomEnd },
    { _cc_img142_yay0SegmentRomStart, _cc_img142_yay0SegmentRomEnd },
    { _cc_img143_yay0SegmentRomStart, _cc_img143_yay0SegmentRomEnd },
    { _cc_img144_yay0SegmentRomStart, _cc_img144_yay0SegmentRomEnd },
    { _cc_img145_yay0SegmentRomStart, _cc_img145_yay0SegmentRomEnd },
    { _cc_img146_yay0SegmentRomStart, _cc_img146_yay0SegmentRomEnd },
    { _cc_img147_yay0SegmentRomStart, _cc_img147_yay0SegmentRomEnd },
    { _cc_img148_yay0SegmentRomStart, _cc_img148_yay0SegmentRomEnd },
    { _cc_img149_yay0SegmentRomStart, _cc_img149_yay0SegmentRomEnd },
    { _cc_img150_yay0SegmentRomStart, _cc_img150_yay0SegmentRomEnd },
    { _cc_img151_yay0SegmentRomStart, _cc_img151_yay0SegmentRomEnd },
    { _cc_img152_yay0SegmentRomStart, _cc_img152_yay0SegmentRomEnd },
    { _cc_img153_yay0SegmentRomStart, _cc_img153_yay0SegmentRomEnd },
    { _cc_img154_yay0SegmentRomStart, _cc_img154_yay0SegmentRomEnd },
    { _cc_img155_yay0SegmentRomStart, _cc_img155_yay0SegmentRomEnd },
    { _cc_img156_yay0SegmentRomStart, _cc_img156_yay0SegmentRomEnd },
    { _cc_img157_yay0SegmentRomStart, _cc_img157_yay0SegmentRomEnd },
    { _cc_img158_yay0SegmentRomStart, _cc_img158_yay0SegmentRomEnd },
    { _cc_img159_yay0SegmentRomStart, _cc_img159_yay0SegmentRomEnd },
    { _cc_img160_yay0SegmentRomStart, _cc_img160_yay0SegmentRomEnd },
    { _cc_img161_yay0SegmentRomStart, _cc_img161_yay0SegmentRomEnd },
    { _cc_img162_yay0SegmentRomStart, _cc_img162_yay0SegmentRomEnd },
    { _cc_img163_yay0SegmentRomStart, _cc_img163_yay0SegmentRomEnd },
    { _cc_img164_yay0SegmentRomStart, _cc_img164_yay0SegmentRomEnd },
    { _cc_img165_yay0SegmentRomStart, _cc_img165_yay0SegmentRomEnd },
    { _cc_img166_yay0SegmentRomStart, _cc_img166_yay0SegmentRomEnd },
    { _cc_img167_yay0SegmentRomStart, _cc_img167_yay0SegmentRomEnd },
    { _cc_img168_yay0SegmentRomStart, _cc_img168_yay0SegmentRomEnd },
    { _cc_img169_yay0SegmentRomStart, _cc_img169_yay0SegmentRomEnd },
    { _cc_img170_yay0SegmentRomStart, _cc_img170_yay0SegmentRomEnd },
    { _cc_img171_yay0SegmentRomStart, _cc_img171_yay0SegmentRomEnd },
    { _cc_img172_yay0SegmentRomStart, _cc_img172_yay0SegmentRomEnd },
    { _cc_img173_yay0SegmentRomStart, _cc_img173_yay0SegmentRomEnd },
    { _cc_img174_yay0SegmentRomStart, _cc_img174_yay0SegmentRomEnd },
    { _cc_img175_yay0SegmentRomStart, _cc_img175_yay0SegmentRomEnd },
    { _cc_img176_yay0SegmentRomStart, _cc_img176_yay0SegmentRomEnd },
    { _cc_img177_yay0SegmentRomStart, _cc_img177_yay0SegmentRomEnd },
    { _cc_img178_yay0SegmentRomStart, _cc_img178_yay0SegmentRomEnd },
    { _cc_img179_yay0SegmentRomStart, _cc_img179_yay0SegmentRomEnd },
    { _cc_img180_yay0SegmentRomStart, _cc_img180_yay0SegmentRomEnd },
    { _cc_img181_yay0SegmentRomStart, _cc_img181_yay0SegmentRomEnd },
    { _cc_img182_yay0SegmentRomStart, _cc_img182_yay0SegmentRomEnd },
    { _cc_img183_yay0SegmentRomStart, _cc_img183_yay0SegmentRomEnd },
    { _cc_img184_yay0SegmentRomStart, _cc_img184_yay0SegmentRomEnd },
    { _cc_img185_yay0SegmentRomStart, _cc_img185_yay0SegmentRomEnd },
    { _cc_img186_yay0SegmentRomStart, _cc_img186_yay0SegmentRomEnd },
    { _cc_img187_yay0SegmentRomStart, _cc_img187_yay0SegmentRomEnd },
    { _cc_img188_yay0SegmentRomStart, _cc_img188_yay0SegmentRomEnd },
    { _cc_img189_yay0SegmentRomStart, _cc_img189_yay0SegmentRomEnd },
    { _cc_img190_yay0SegmentRomStart, _cc_img190_yay0SegmentRomEnd },
    { _cc_img191_yay0SegmentRomStart, _cc_img191_yay0SegmentRomEnd },
    { _cc_img192_yay0SegmentRomStart, _cc_img192_yay0SegmentRomEnd },
    { _cc_img193_yay0SegmentRomStart, _cc_img193_yay0SegmentRomEnd },
    { _cc_img194_yay0SegmentRomStart, _cc_img194_yay0SegmentRomEnd },
    { _cc_img195_yay0SegmentRomStart, _cc_img195_yay0SegmentRomEnd },
    { _cc_img196_yay0SegmentRomStart, _cc_img196_yay0SegmentRomEnd },
    { _cc_img197_yay0SegmentRomStart, _cc_img197_yay0SegmentRomEnd },
    { _cc_img198_yay0SegmentRomStart, _cc_img198_yay0SegmentRomEnd },
    { _cc_img199_yay0SegmentRomStart, _cc_img199_yay0SegmentRomEnd },
    { _cc_img200_yay0SegmentRomStart, _cc_img200_yay0SegmentRomEnd },
    { _cc_img201_yay0SegmentRomStart, _cc_img201_yay0SegmentRomEnd },
    { _cc_img202_yay0SegmentRomStart, _cc_img202_yay0SegmentRomEnd },
    { _cc_img203_yay0SegmentRomStart, _cc_img203_yay0SegmentRomEnd },
    { _cc_img204_yay0SegmentRomStart, _cc_img204_yay0SegmentRomEnd },
    { _cc_img205_yay0SegmentRomStart, _cc_img205_yay0SegmentRomEnd },
    { _cc_img206_yay0SegmentRomStart, _cc_img206_yay0SegmentRomEnd },
    { _cc_img207_yay0SegmentRomStart, _cc_img207_yay0SegmentRomEnd },
    { _cc_img208_yay0SegmentRomStart, _cc_img208_yay0SegmentRomEnd },
    { _cc_img209_yay0SegmentRomStart, _cc_img209_yay0SegmentRomEnd },
    { _cc_img210_yay0SegmentRomStart, _cc_img210_yay0SegmentRomEnd },
    { _cc_img211_yay0SegmentRomStart, _cc_img211_yay0SegmentRomEnd },
    { _cc_img212_yay0SegmentRomStart, _cc_img212_yay0SegmentRomEnd },
    { _cc_img213_yay0SegmentRomStart, _cc_img213_yay0SegmentRomEnd },
    { _cc_img214_yay0SegmentRomStart, _cc_img214_yay0SegmentRomEnd },
    { _cc_img215_yay0SegmentRomStart, _cc_img215_yay0SegmentRomEnd },
    { _cc_img216_yay0SegmentRomStart, _cc_img216_yay0SegmentRomEnd },
    { _cc_img217_yay0SegmentRomStart, _cc_img217_yay0SegmentRomEnd },
    { _cc_img218_yay0SegmentRomStart, _cc_img218_yay0SegmentRomEnd },
    { _cc_img219_yay0SegmentRomStart, _cc_img219_yay0SegmentRomEnd },
    { _cc_img220_yay0SegmentRomStart, _cc_img220_yay0SegmentRomEnd },
    { _cc_img221_yay0SegmentRomStart, _cc_img221_yay0SegmentRomEnd },
    { _cc_img222_yay0SegmentRomStart, _cc_img222_yay0SegmentRomEnd },
    { _cc_img223_yay0SegmentRomStart, _cc_img223_yay0SegmentRomEnd },
    { _cc_img224_yay0SegmentRomStart, _cc_img224_yay0SegmentRomEnd },
    { _cc_img225_yay0SegmentRomStart, _cc_img225_yay0SegmentRomEnd },
    { _cc_img226_yay0SegmentRomStart, _cc_img226_yay0SegmentRomEnd },
    { _cc_img227_yay0SegmentRomStart, _cc_img227_yay0SegmentRomEnd },
    { _cc_img228_yay0SegmentRomStart, _cc_img228_yay0SegmentRomEnd },
    { _cc_img229_yay0SegmentRomStart, _cc_img229_yay0SegmentRomEnd },
    { _cc_img230_yay0SegmentRomStart, _cc_img230_yay0SegmentRomEnd },
    { _cc_img231_yay0SegmentRomStart, _cc_img231_yay0SegmentRomEnd },
    { _cc_img232_yay0SegmentRomStart, _cc_img232_yay0SegmentRomEnd },
    { _cc_img233_yay0SegmentRomStart, _cc_img233_yay0SegmentRomEnd },
    { _cc_img234_yay0SegmentRomStart, _cc_img234_yay0SegmentRomEnd },
    { _cc_img235_yay0SegmentRomStart, _cc_img235_yay0SegmentRomEnd },
    { _cc_img236_yay0SegmentRomStart, _cc_img236_yay0SegmentRomEnd },
    { _cc_img237_yay0SegmentRomStart, _cc_img237_yay0SegmentRomEnd },
    { _cc_img238_yay0SegmentRomStart, _cc_img238_yay0SegmentRomEnd },
    { _cc_img239_yay0SegmentRomStart, _cc_img239_yay0SegmentRomEnd },
    { _cc_img240_yay0SegmentRomStart, _cc_img240_yay0SegmentRomEnd },
    { _cc_img241_yay0SegmentRomStart, _cc_img241_yay0SegmentRomEnd },
    { _cc_img242_yay0SegmentRomStart, _cc_img242_yay0SegmentRomEnd },
    { _cc_img243_yay0SegmentRomStart, _cc_img243_yay0SegmentRomEnd },
    { _cc_img244_yay0SegmentRomStart, _cc_img244_yay0SegmentRomEnd },
    { _cc_img245_yay0SegmentRomStart, _cc_img245_yay0SegmentRomEnd },
    { _cc_img246_yay0SegmentRomStart, _cc_img246_yay0SegmentRomEnd },
    { _cc_img247_yay0SegmentRomStart, _cc_img247_yay0SegmentRomEnd },
    { _cc_img248_yay0SegmentRomStart, _cc_img248_yay0SegmentRomEnd },
    { _cc_img249_yay0SegmentRomStart, _cc_img249_yay0SegmentRomEnd },
    { _cc_img250_yay0SegmentRomStart, _cc_img250_yay0SegmentRomEnd },
    { _cc_img251_yay0SegmentRomStart, _cc_img251_yay0SegmentRomEnd },
    { _cc_img252_yay0SegmentRomStart, _cc_img252_yay0SegmentRomEnd },
    { _cc_img253_yay0SegmentRomStart, _cc_img253_yay0SegmentRomEnd },
    { _cc_img254_yay0SegmentRomStart, _cc_img254_yay0SegmentRomEnd },
    { _cc_img255_yay0SegmentRomStart, _cc_img255_yay0SegmentRomEnd },
    { _cc_img256_yay0SegmentRomStart, _cc_img256_yay0SegmentRomEnd },
    { _cc_img257_yay0SegmentRomStart, _cc_img257_yay0SegmentRomEnd },
    { _cc_img258_yay0SegmentRomStart, _cc_img258_yay0SegmentRomEnd },
    { _cc_img259_yay0SegmentRomStart, _cc_img259_yay0SegmentRomEnd },
    { _cc_img260_yay0SegmentRomStart, _cc_img260_yay0SegmentRomEnd },
    { _cc_img261_yay0SegmentRomStart, _cc_img261_yay0SegmentRomEnd },
    { _cc_img262_yay0SegmentRomStart, _cc_img262_yay0SegmentRomEnd },
    { _cc_img263_yay0SegmentRomStart, _cc_img263_yay0SegmentRomEnd },
    { _cc_img264_yay0SegmentRomStart, _cc_img264_yay0SegmentRomEnd },
    { _cc_img265_yay0SegmentRomStart, _cc_img265_yay0SegmentRomEnd },
    { _cc_img266_yay0SegmentRomStart, _cc_img266_yay0SegmentRomEnd },
    { _cc_img267_yay0SegmentRomStart, _cc_img267_yay0SegmentRomEnd },
    { _cc_img268_yay0SegmentRomStart, _cc_img268_yay0SegmentRomEnd },
    { _cc_img269_yay0SegmentRomStart, _cc_img269_yay0SegmentRomEnd },
    { _cc_img270_yay0SegmentRomStart, _cc_img270_yay0SegmentRomEnd },
    { _cc_img271_yay0SegmentRomStart, _cc_img271_yay0SegmentRomEnd },
    { _cc_img272_yay0SegmentRomStart, _cc_img272_yay0SegmentRomEnd },
    { _cc_img273_yay0SegmentRomStart, _cc_img273_yay0SegmentRomEnd },
    { _cc_img274_yay0SegmentRomStart, _cc_img274_yay0SegmentRomEnd },
    { _cc_img275_yay0SegmentRomStart, _cc_img275_yay0SegmentRomEnd },
    { _cc_img276_yay0SegmentRomStart, _cc_img276_yay0SegmentRomEnd },
    { _cc_img277_yay0SegmentRomStart, _cc_img277_yay0SegmentRomEnd },
    { _cc_img278_yay0SegmentRomStart, _cc_img278_yay0SegmentRomEnd },
    { _cc_img279_yay0SegmentRomStart, _cc_img279_yay0SegmentRomEnd },
    { _cc_img280_yay0SegmentRomStart, _cc_img280_yay0SegmentRomEnd },
    { _cc_img281_yay0SegmentRomStart, _cc_img281_yay0SegmentRomEnd },
    { _cc_img282_yay0SegmentRomStart, _cc_img282_yay0SegmentRomEnd },
    { _cc_img283_yay0SegmentRomStart, _cc_img283_yay0SegmentRomEnd },
    { _cc_img284_yay0SegmentRomStart, _cc_img284_yay0SegmentRomEnd },
    { _cc_img285_yay0SegmentRomStart, _cc_img285_yay0SegmentRomEnd },
    { _cc_img286_yay0SegmentRomStart, _cc_img286_yay0SegmentRomEnd },
    { _cc_img287_yay0SegmentRomStart, _cc_img287_yay0SegmentRomEnd },
    { _cc_img288_yay0SegmentRomStart, _cc_img288_yay0SegmentRomEnd },
    { _cc_img289_yay0SegmentRomStart, _cc_img289_yay0SegmentRomEnd },
    { _cc_img290_yay0SegmentRomStart, _cc_img290_yay0SegmentRomEnd },
    { _cc_img291_yay0SegmentRomStart, _cc_img291_yay0SegmentRomEnd },
    { _cc_img292_yay0SegmentRomStart, _cc_img292_yay0SegmentRomEnd },
    { _cc_img293_yay0SegmentRomStart, _cc_img293_yay0SegmentRomEnd },
    { _cc_img294_yay0SegmentRomStart, _cc_img294_yay0SegmentRomEnd },
    { _cc_img295_yay0SegmentRomStart, _cc_img295_yay0SegmentRomEnd },
    { _cc_img296_yay0SegmentRomStart, _cc_img296_yay0SegmentRomEnd },
    { _cc_img297_yay0SegmentRomStart, _cc_img297_yay0SegmentRomEnd },
    { _cc_img298_yay0SegmentRomStart, _cc_img298_yay0SegmentRomEnd },
    { _cc_img299_yay0SegmentRomStart, _cc_img299_yay0SegmentRomEnd },
    { _cc_img300_yay0SegmentRomStart, _cc_img300_yay0SegmentRomEnd },
    { _cc_img301_yay0SegmentRomStart, _cc_img301_yay0SegmentRomEnd },
    { _cc_img302_yay0SegmentRomStart, _cc_img302_yay0SegmentRomEnd },
    { _cc_img303_yay0SegmentRomStart, _cc_img303_yay0SegmentRomEnd },
    { _cc_img304_yay0SegmentRomStart, _cc_img304_yay0SegmentRomEnd },
    { _cc_img305_yay0SegmentRomStart, _cc_img305_yay0SegmentRomEnd },
    { _cc_img306_yay0SegmentRomStart, _cc_img306_yay0SegmentRomEnd },
    { _cc_img307_yay0SegmentRomStart, _cc_img307_yay0SegmentRomEnd },
    { _cc_img308_yay0SegmentRomStart, _cc_img308_yay0SegmentRomEnd },
    { _cc_img309_yay0SegmentRomStart, _cc_img309_yay0SegmentRomEnd },
    { _cc_img310_yay0SegmentRomStart, _cc_img310_yay0SegmentRomEnd },
    { _cc_img311_yay0SegmentRomStart, _cc_img311_yay0SegmentRomEnd },
    { _cc_img312_yay0SegmentRomStart, _cc_img312_yay0SegmentRomEnd },
    { _cc_img313_yay0SegmentRomStart, _cc_img313_yay0SegmentRomEnd },
    { _cc_img314_yay0SegmentRomStart, _cc_img314_yay0SegmentRomEnd },
    { _cc_img315_yay0SegmentRomStart, _cc_img315_yay0SegmentRomEnd },
    { _cc_img316_yay0SegmentRomStart, _cc_img316_yay0SegmentRomEnd },
    { _cc_img317_yay0SegmentRomStart, _cc_img317_yay0SegmentRomEnd },
    { _cc_img318_yay0SegmentRomStart, _cc_img318_yay0SegmentRomEnd },
    { _cc_img319_yay0SegmentRomStart, _cc_img319_yay0SegmentRomEnd },
    { _cc_img320_yay0SegmentRomStart, _cc_img320_yay0SegmentRomEnd },
    { _cc_img321_yay0SegmentRomStart, _cc_img321_yay0SegmentRomEnd },
    { _cc_img322_yay0SegmentRomStart, _cc_img322_yay0SegmentRomEnd },
    { _cc_img323_yay0SegmentRomStart, _cc_img323_yay0SegmentRomEnd },
    { _cc_img324_yay0SegmentRomStart, _cc_img324_yay0SegmentRomEnd },
    { _cc_img325_yay0SegmentRomStart, _cc_img325_yay0SegmentRomEnd },
    { _cc_img326_yay0SegmentRomStart, _cc_img326_yay0SegmentRomEnd },
    { _cc_img327_yay0SegmentRomStart, _cc_img327_yay0SegmentRomEnd },
    { _cc_img328_yay0SegmentRomStart, _cc_img328_yay0SegmentRomEnd },
    { _cc_img329_yay0SegmentRomStart, _cc_img329_yay0SegmentRomEnd },
    { _cc_img330_yay0SegmentRomStart, _cc_img330_yay0SegmentRomEnd },
    { _cc_img331_yay0SegmentRomStart, _cc_img331_yay0SegmentRomEnd },
    { _cc_img332_yay0SegmentRomStart, _cc_img332_yay0SegmentRomEnd },
    { _cc_img333_yay0SegmentRomStart, _cc_img333_yay0SegmentRomEnd },
};

extern void load_decompress(const u8* srcStart, const u8* srcEnd, u8* dst);

extern s32 gTatums;
void bhv_movie_loop()
{
    print_text_fmt_int(20, 40, "%x", (int) &(o->oTimer));
    if (0 == o->oAction)
    {
        if (o->oDistanceToMario < 100.f)
        {
            o->oAction = 1;
            play_sound(SOUND_MENU_THANK_YOU_PLAYING_MY_GAME, gGlobalSoundSource);
            gMarioStates->action = 0;
            sMovieTexture = sTextures[0];
            gTatums = 0;
        }
    }
    else if (1 == o->oAction)
    {
        int presentFrame = o->oTimer % TEXTURES_COUNTS;
        int renderFrame = (o->oTimer + 1) % TEXTURES_COUNTS;

        f32 realTime = gTatums / 16.f;
        int movieFrame = realTime * 24.f / 30.f;

        u8* out = sTextures[renderFrame];
        if (movieFrame < sizeof(sMovieFrames) / sizeof(*sMovieFrames))
        {
            load_decompress(sMovieFrames[movieFrame].start, sMovieFrames[movieFrame].end, out);
        }
        else
        {
            bzero(out, 320 * 240 * 2);
        }
        sMovieTexture = sTextures[presentFrame];

        if (gTatums > 7000)
        {
            gMarioStates->usedObj = o;
            o->oBehParams2ndByte = 0xa;
            o->oBehParams = 0xa << 16;
            level_trigger_warp(gMarioStates, WARP_OP_TELEPORT);
            o->oAction = 2;
        }
    }
    else
    {
        if (o->oTimer == 19)
        {
            sMovieTexture = 0;
        }
    }
}

extern Gfx movie_movie_mesh[];
void render_movie()
{
    if (sMovieTexture)
    {
        create_dl_ortho_matrix();

        create_dl_translation_matrix(MENU_MTX_PUSH, 160, 120, 0);
        gSPLoadGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
        gDPSetCombineLERP(gDisplayListHead++, 0, 0, 0, TEXEL0, 0, 0, 0, 1, 0, 0, 0, TEXEL0, 0, 0, 0, 1);
        gSPSetOtherMode(gDisplayListHead++, G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE);
        gSPSetOtherMode(gDisplayListHead++, G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL);
        gSPTexture(gDisplayListHead++, 65535, 65535, 0, 0, 1);
        gDPLoadSync(gDisplayListHead++);
        gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, sMovieTexture);
        gSPDisplayList(gDisplayListHead++, movie_movie_mesh);

        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }
}
