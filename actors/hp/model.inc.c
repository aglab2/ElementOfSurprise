#if 0
Vtx hp_hp_mesh_layer_1_vtx_0[4] = {
	{{ {0, 310, 231}, 0, {496, 614}, {127, 0, 0, 255} }},
	{{ {0, 238, 231}, 0, {496, 614}, {127, 0, 0, 255} }},
	{{ {0, 238, -66}, 0, {496, 462}, {127, 0, 0, 255} }},
	{{ {0, 310, -66}, 0, {496, 462}, {127, 0, 0, 255} }},
};

Vtx hp_hp_mesh_layer_1_vtx_1[4] = {
	{{ {0, 310, -66}, 0, {496, 462}, {127, 0, 0, 255} }},
	{{ {0, 238, -66}, 0, {496, 462}, {127, 0, 0, 255} }},
	{{ {0, 238, -247}, 0, {496, 369}, {127, 0, 0, 255} }},
	{{ {0, 310, -247}, 0, {496, 369}, {127, 0, 0, 255} }},
};
#endif

Gfx hp_enemy_enter[] = {
	gsDPPipeSync(),
    gsSPClearGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADING_SMOOTH),
	gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, 0, 0, 0, 1, 0, 0, 0, PRIMITIVE, 0, 0, 0, 1),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx hp_enemy_exit[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
    gsSPSetGeometryMode(G_ZBUFFER | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPEndDisplayList(),
};

#if 0
Gfx hp_hp_mesh_layer_1[] = {
	// set prim color
	gsSPVertex(hp_hp_mesh_layer_1_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	// set prim color
	gsSPVertex(hp_hp_mesh_layer_1_vtx_1 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
};
#endif
