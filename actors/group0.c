#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#include "spindrift/model.inc.c"
#include "spindrift/anims/data.inc.c"
#include "spindrift/anims/table.inc.c"

#include "penguin/model.inc.c"
#include "penguin/anims/data.inc.c"
#include "penguin/anims/table.inc.c"
#include "penguin/collision.inc.c"

#include "snowman/model.inc.c"
#include "snowman/anims/data.inc.c"
#include "snowman/anims/table.inc.c"

#include "mr_i_eyeball/model.inc.c"

#include "mr_i_iris/model.inc.c"

#include "swoop/model.inc.c"
#include "swoop/anims/data.inc.c"
#include "swoop/anims/table.inc.c"

#include "snufit/model.inc.c"

#include "scuttlebug/model.inc.c"
#include "scuttlebug/anims/data.inc.c"
#include "scuttlebug/anims/table.inc.c"

#include "bowser/model.inc.c"
#include "bowser/anims/data.inc.c"
#include "bowser/anims/table.inc.c"

#include "bully/model.inc.c"
#include "bully/anims/data.inc.c"
#include "bully/anims/table.inc.c"

#include "blargg/model.inc.c"
#include "blargg/anims/data.inc.c"
#include "blargg/anims/table.inc.c"

#include "lakitu_enemy/model.inc.c"
#include "lakitu_enemy/anims/data.inc.c"
#include "lakitu_enemy/anims/table.inc.c"

#include "spiny_egg/model.inc.c"
#include "spiny_egg/anims/data.inc.c"
#include "spiny_egg/anims/table.inc.c"

#include "spiny/model.inc.c"
#include "spiny/anims/data.inc.c"
#include "spiny/anims/table.inc.c"

#include "manta/model.inc.c"
#include "manta/anims/data.inc.c"
#include "manta/anims/table.inc.c"

#include "yoshi/model.inc.c"
#include "yoshi/anims/data.inc.c"
#include "yoshi/anims/table.inc.c"

#include "heave_ho/model.inc.c"
#include "heave_ho/anims/data.inc.c"
#include "heave_ho/anims/table.inc.c"

#include "monty_mole_hole/model.inc.c"
#include "monty_mole/model.inc.c"

#include "smoke/model.inc.c"
#include "ukiki/model.inc.c"

#include "fwoosh/model.inc.c"

#include "water_bubble/model.inc.c"

#include "yoshi_egg/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

#include "hp/model.inc.c"