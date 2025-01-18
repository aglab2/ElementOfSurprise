#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "macros.h"
#include "surface_terrains.h"
#include "types.h"
#include "make_const_nonconst.h"

ALIGNED8 const Texture movie_image80[] = {
#include "levels/castle_grounds/image80.rgba16.inc.c"
};
