#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "macros.h"
#include "surface_terrains.h"
#include "types.h"
#include "make_const_nonconst.h"

ALIGNED8 const Texture movie_image224[] = {
#include "levels/castle_grounds/image224.rgba16.inc.c"
};
