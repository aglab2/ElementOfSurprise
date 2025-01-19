#include "src/game/envfx_snow.h"

extern Gfx *geo_hp(s32 callContext, struct GraphNode *node, Mat4 mtx);
const GeoLayout hp_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
        GEO_ASM(0, geo_hp),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
