#include "src/game/envfx_snow.h"

const GeoLayout circle_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_RANGE, circle_circle_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
