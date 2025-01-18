#include "game/puppyprint.h"

#include "actors/group14.h"

static struct Object* kTDMap[10][10];

#define oTDWave oF4
#define oTDSelectedTower oF8

enum TowerTypes
{
    TOWER_FIRE,
    TOWER_WATER,
    TOWER_CRYSTAL,
    TOWER_AIR,
};

enum TDStates
{
    TD_INIT,
    TD_WAVE,
};

struct TowerBeh
{
    int model;
    const BehaviorScript* bhv;
    const char* name;
    const char* desc;
};

static struct TowerBeh kBasicTowerBehs[] = {
    { MODEL_FLYGUY       , bhvFireTower   , "Flame Tower"       , "Has exploding projectiles" }, // AOE traps
    { MODEL_MR_BLIZZARD  , bhvWaterTower  , "Freeze Tower"      , "Slows down targeted enemy" }, // Slowing down
    { MODEL_SNUFIT       , bhvCrystalTower, "Sharpshooter Tower", "Far range attacks" }, // Very far range, slow, high damage
    { MODEL_MONTY_MOLE   , bhvAirTower    , "Dirt Tower"        , "No attack cooldown" }, // Very fast, low damage
};

static struct TowerBeh kComboTowerBehs[4][4] = {
    [TOWER_FIRE] = {
        { MODEL_PIRANHA_PLANT, bhvFireTower   , "Inferno Tower", "Enemy burn damaging surrounding enemies" },
        { MODEL_BOO          , bhvSteamTower  , "Vapor Tower"  , "High damage on short circle range" },
        { MODEL_YOSHI        , bhvSpireTower  , "0.5A Tower"   , "Has 0.5% chance to instantly kill target" },
        { MODEL_BLARGG       , bhvInfernoTower, "Spin Tower"   , "Spawn rotating flame around tower" },
    },
    [TOWER_WATER] = {
        { MODEL_BOO         , bhvSteamTower    , "Vapor Tower"     , "High damage on short circle range" },
        { MODEL_PENGUIN     , bhvWaterTower    , "Permafrost Tower", "Permanently freezes enemy" },
        { MODEL_MANTA_RAY   , bhvShardTower    , "Spire Tower"     , "Hops on 5 enemies after attack" },
        { MODEL_ENEMY_LAKITU, bhvHurricaneTower, "Love Tower"      , "Throws projectile that permanently enemies" },
    },
    [TOWER_CRYSTAL] = {
        { MODEL_YOSHI       , bhvSpireTower  , "0.5A Tower"        , "Has 0.5% chance to instantly kill target" },
        { MODEL_MANTA_RAY   , bhvShardTower  , "Spire Tower"       , "Hops on 5 enemies after attack" },
        { MODEL_HEAVE_HO    , bhvCrystalTower, "Ultrasharp Tower"  , "Full field attack, flips enemies" },
        { MODEL_MR_I        , bhvMrI         , "Pupil Tower"       , "Shoot far attacks in 8 directions, no aim" },
    },
    [TOWER_AIR] = {
        { MODEL_BLARGG       , bhvInfernoTower  , "Spin Tower"   , "Spawn rotating flame around tower" },
        { MODEL_ENEMY_LAKITU , bhvHurricaneTower, "Love Tower"   , "Throws projectile that permanently enemies" },
        { MODEL_MR_I         , bhvMrI           , "Pupil Tower"  , "Shoot far attacks in 8 directions, no aim" },
        { MODEL_UKIKI        , bhvAirTower      , "Banana Tower" , "No attack cd + double projectiles" },
    },
};

struct Color
{
    u8 r, g, b, a;
};

static const struct Color kTowerColors[] = {
    [ TOWER_FIRE ]    = { 0xff, 0x47, 0x4c, 255 },
    [ TOWER_WATER ]   = { 0x73, 0xc2, 0xfb, 255 },
    [ TOWER_CRYSTAL ] = { 0xef, 0xde, 0x21, 255 },
    [ TOWER_AIR ]     = { 0x58, 0x2f, 0xe , 255 }, 
};

union TowerTypePacked
{
    struct
    {
        u8 buffed;
        u8 type0;
        u8 type1;
        u8 _pad; // must be 0!
    };
    u32 value;
};

enum EnemyTypes
{
    ENEMY_GOOMBA,
    ENEMY_KOOPA,
    ENEMY_BOBOMB,
    ENEMY_BULLY,
    ENEMY_BOWSER,
};

#define TD_BUSY ((void*) 1)
#define TD_CAN_SELL(o) ((int)o < 0)

void bhv_td_init()
{
    for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
    {
        kTDMap[i][j] = NULL;
    }

    kTDMap[0][0] = TD_BUSY;
    for (int i = 3; i < 10; i++)
        kTDMap[i][0] = TD_BUSY;

    kTDMap[0][1] = TD_BUSY;
    for (int i = 3; i < 10; i++)
        kTDMap[i][1] = TD_BUSY;

    kTDMap[0][2] = TD_BUSY;
    kTDMap[9][2] = TD_BUSY;

    kTDMap[0][3] = TD_BUSY;
    for (int i = 3; i < 7; i++)
        kTDMap[i][3] = TD_BUSY;
    kTDMap[9][3] = TD_BUSY;

    kTDMap[0][4] = TD_BUSY;
    kTDMap[3][4] = TD_BUSY;
    kTDMap[6][4] = TD_BUSY;
    kTDMap[9][4] = TD_BUSY;

    for (int i = 0; i < 4; i++)
        kTDMap[i][5] = TD_BUSY;
    kTDMap[6][5] = TD_BUSY;
    kTDMap[9][5] = TD_BUSY;

    kTDMap[6][6] = TD_BUSY;
    kTDMap[9][6] = TD_BUSY;

    for (int i = 0; i < 7; i++)
        kTDMap[i][7] = TD_BUSY;
    kTDMap[9][7] = TD_BUSY;

    kTDMap[0][8] = TD_BUSY;
    kTDMap[9][8] = TD_BUSY;

    for (int i = 0; i < 10; i++)
        kTDMap[i][9] = TD_BUSY;

    gMarioStates->numCoins = 50;

    o->oTDSelectedTower = -1;
}

static void handle_tower_style_selection()
{
    if (gPlayer1Controller->buttonPressed & U_JPAD)
    {
        o->oTDSelectedTower = TOWER_FIRE;
    }
    else if (gPlayer1Controller->buttonPressed & D_JPAD)
    {
        o->oTDSelectedTower = TOWER_WATER;
    }
    else if (gPlayer1Controller->buttonPressed & L_JPAD)
    {
        o->oTDSelectedTower = TOWER_CRYSTAL;
    }
    else if (gPlayer1Controller->buttonPressed & R_JPAD)
    {
        o->oTDSelectedTower = TOWER_AIR;
    }
}

static void handle_tower_spawning()
{
    const int dist = 127.f;
    const int marioX = (int) gMarioStates->pos[0] + dist * 10;
    const int marioZ = (int) gMarioStates->pos[2] + dist * 10;

    int i = marioX / dist / 2;
    int j = marioZ / dist / 2;
 
#if 1
    o->oPosX = i * dist * 2 - dist * 9;
    o->oPosY = 0;
    o->oPosZ = j * dist * 2 - dist * 9;
#else
    o->oPosX = gMarioStates->pos[0];
    o->oPosY = gMarioStates->pos[1];
    o->oPosZ = gMarioStates->pos[2];
#endif

    struct Object** pslot = &kTDMap[i][j];

#define slot (*pslot)
    if (slot == TD_BUSY)
    {
        cur_obj_hide();
        return;
    }
    else
    {
        cur_obj_unhide();
    }

    if (-1 == o->oTDSelectedTower)
        return;

    if (slot)
    {

    }
    else
    {
        struct Color color;
        color.r = kTowerColors[o->oTDSelectedTower].r;
        color.g = kTowerColors[o->oTDSelectedTower].g;
        color.b = kTowerColors[o->oTDSelectedTower].b;

        char line[100];
        sprintf(line, "A to place <COL_%02X%02X%02XFF>%s", color.r, color.g, color.b, kBasicTowerBehs[o->oTDSelectedTower].name);

    
        print_small_text_buffered(160, 205, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
        print_set_envcolour(color.r, color.g, color.b, color.a);
        print_small_text_buffered(160, 215, kBasicTowerBehs[o->oTDSelectedTower].desc, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
        print_set_envcolour(255, 255, 255, 255);

        if (gPlayer1Controller->buttonPressed & A_BUTTON)
        {
            if (gMarioStates->numCoins >= 10)
            {
                gMarioStates->numCoins -= 10;
                slot = spawn_object(o, kBasicTowerBehs[o->oTDSelectedTower].model, kBasicTowerBehs[o->oTDSelectedTower].bhv);
                union TowerTypePacked* packed = (union TowerTypePacked*) &slot->oBehParams2ndByte;
                packed->buffed = 0;
                packed->type0 = o->oTDSelectedTower;
                packed->type1 = 0;
                packed->_pad = 0;

                slot->oBehParams2ndByte = o->oTDSelectedTower;
            }
        }
    }
#undef slot
}

static void set_green_color_if(int on)
{
    if (!on)
        print_set_envcolour(255, 255, 255, 255);
    else
        print_set_envcolour(136, 231, 136, 255);
}

static void render_tower_labels()
{
    set_green_color_if(o->oTDSelectedTower == TOWER_FIRE);
    print_small_text_buffered(260, 10, "Fire", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
    set_green_color_if(o->oTDSelectedTower == TOWER_WATER);
    print_small_text_buffered(260, 50, "Water", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
    set_green_color_if(o->oTDSelectedTower == TOWER_CRYSTAL);
    print_small_text_buffered(240, 30, "Light", PRINT_TEXT_ALIGN_RIGHT, PRINT_ALL, FONT_OUTLINE);
    set_green_color_if(o->oTDSelectedTower == TOWER_AIR);
    print_small_text_buffered(280, 30, "Earth", PRINT_TEXT_ALIGN_LEFT, PRINT_ALL, FONT_OUTLINE);

    print_set_envcolour(255, 255, 255, 255);
}

void bhv_td_loop()
{
    gMarioStates->pos[1] = 200.f;
    gMarioStates->action = ACT_DEBUG_FREE_MOVE;
    gCamera->cutscene = CUTSCENE_TD;

    handle_tower_style_selection();
    handle_tower_spawning();

    render_tower_labels();

    switch (o->oAction)
    {
        case TD_INIT:
            if (-1 == o->oTDSelectedTower && gPlayer1Controller->buttonPressed & L_TRIG)
                o->oAction = TD_WAVE;

            break;
        case TD_WAVE:
    }
}

void bhv_td_enemy_loop()
{
    enum EnemyTypes enemyType = (enum EnemyTypes) o->oBehParams2ndByte;
    switch (enemyType)
    {
        case ENEMY_GOOMBA:
            cur_obj_init_animation_with_accel_and_sound(GOOMBA_ANIM_DEFAULT, 1.f);
            break;
        case ENEMY_KOOPA:
            break;
        case ENEMY_BOBOMB:
            break;
        case ENEMY_BULLY:
            break;
        case ENEMY_BOWSER:
            break;
    }
}

void bhv_fire_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (!packed->buffed)
    {
        obj_scale(o, 2.f);
        o->oAnimations = flyguy_seg8_anims_08011A64;
        s32 animIndex = FLY_GUY_ANIM_FLYING;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        o->oAnimations = piranha_plant_seg6_anims_0601C31C;
        s32 animIndex = PIRANHA_PLANT_ANIM_BITE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_water_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (!packed->buffed)
    {
        o->oAnimations = snowman_seg5_anims_0500D118;
        s32 animIndex = MR_BLIZZARD_ANIM_SPAWN_SNOWBALL;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        o->oAnimations = penguin_seg5_anims_05008B74;
        s32 animIndex = PENGUIN_ANIM_IDLE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_crystal_tower_init()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (!packed->buffed)
        o->oPosY += 100.f;
}

void bhv_crystal_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (!packed->buffed)
    {
        obj_scale(o, 2.f + sins(o->oTimer * 0x456) / 10.f);
        o->oSnufitRecoil = 0;
        o->oSnufitBodyScale = 1000;
    }
    else
    {
        o->oAnimations = heave_ho_seg5_anims_0501534C;
        s32 animIndex = HEAVE_HO_ANIM_MOVING;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_air_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (!packed->buffed)
    {
        obj_scale(o, 2.f);
        o->oAnimations = monty_mole_seg5_anims_05007248;
        s32 animIndex = MONTY_MOLE_ANIM_BEGIN_JUMP_INTO_HOLE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        o->oAnimations = ukiki_seg5_anims_05015784;
        s32 animIndex = UKIKI_ANIM_RUN;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}
