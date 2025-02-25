#include <ultra64.h>
#include "sm64.h"
#include "geo_commands.h"

#include "game/level_geo.h"
#include "game/geo_misc.h"
#include "game/camera.h"
#include "game/moving_texture.h"
#include "game/screen_transition.h"
#include "game/paintings.h"

#include "make_const_nonconst.h"

const GeoLayout td_shadow_geo[] = {
   GEO_SHADOW(5, 0xC8, 200),
   GEO_END(),
};

#include "levels/castle_courtyard/header.h"

#include "levels/castle_courtyard/geo.inc.c"

#include "levels/castle_courtyard/circle/geo.inc.c"