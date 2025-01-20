#include "game/puppyprint.h"

#include "actors/group14.h"

static struct Object* kTDMap[10][10];

#define oTDWave oF4
#define oTDSelectedTower oF8
#define oTDWaveLeftEnemies oFC
// cast to SpawnedStatePacked
#define oTDWaveSpawningState o100
// array of u8
#define oTDWaveSpawningAmountSpawned o104
#define oTDWaveTotalEnemyCount o108

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
    TD_WAVE_SPAWNING,
    TD_WAVE_WAITING,
    TD_WAIT_NEXT_WAVE,
};

struct TowerBeh
{
    int model;
    const BehaviorScript* bhv;
    const char* name;
    const char* desc;
};

struct Color
{
    u8 r, g, b, a;
};

static const struct Color kTowerColors[] = {
    [ TOWER_FIRE ]    = { 0xff, 0x47, 0x4c, 255 },
    [ TOWER_WATER ]   = { 0x73, 0xc2, 0xfb, 255 },
    [ TOWER_CRYSTAL ] = { 0xef, 0xde, 0x21, 255 },
    [ TOWER_AIR ]     = { 0x58*2, 0x2f*2, 0xe*2, 255 }, 
};

const struct TowerBeh kInitTower = { MODEL_CANNON_BARREL, bhvTower, "Tower", "Shoots projectiles at enemies" };

static struct TowerBeh kBasicTowerBehs[] = {
    { MODEL_FLYGUY       , bhvFireTower   , "Flame Tower"       , "Spawn fire trap on hit" },
    { MODEL_MR_BLIZZARD  , bhvWaterTower  , "Freeze Tower"      , "Slows down targeted enemy" },
    { MODEL_SNUFIT       , bhvCrystalTower, "Sharpshooter Tower", "Shoots furthest enemy" },
    { MODEL_MONTY_MOLE   , bhvAirTower    , "Dirt Tower"        , "Very fast but weak attacks" },
};

static struct TowerBeh kComboTowerBehs[4][4] = {
    [TOWER_FIRE] = {
        { MODEL_PIRANHA_PLANT, bhvFireTower   , "Inferno Tower", "Enemy catches on fire" },
        { MODEL_THWOMP       , bhvSteamTower  , "URGH Tower"   , "High damage on short circle range" },
        { MODEL_YOSHI        , bhvSpireTower  , "0.5A Tower"   , "Has 0.5% chance to instantly kill target" },
        { MODEL_BLARGG       , bhvInfernoTower, "Spin Tower"   , "Spawn rotating flame around tower" },
    },
    [TOWER_WATER] = {
        { MODEL_THWOMP      , bhvSteamTower    , "URGH Tower"      , "High damage on around tower" },
        { MODEL_PENGUIN     , bhvWaterTower    , "Permafrost Tower", "Slows downs enemies around tower" },
        { MODEL_MANTA_RAY   , bhvShardTower    , "LINK Tower"      , "Hops on 3 far enemies after attack" },
        { MODEL_ENEMY_LAKITU, bhvHurricaneTower, "Love Tower"      , "Throws projectile that follow enemies" },
    },
    [TOWER_CRYSTAL] = {
        { MODEL_YOSHI       , bhvSpireTower  , "0.5A Tower"        , "Has 0.5% chance to instantly kill target" },
        { MODEL_MANTA_RAY   , bhvShardTower  , "LINK Tower"        , "Hops on 3 far enemies after attack" },
        { MODEL_HEAVE_HO    , bhvCrystalTower, "Flip Tower"        , "Full field attack, flips enemies" },
        { MODEL_MR_I        , bhvPrismTower  , "iTower"            , "Shoots slow linear projectiles" },
    },
    [TOWER_AIR] = {
        { MODEL_BLARGG       , bhvInfernoTower  , "Spin Tower"   , "Spawn rotating flame around tower" },
        { MODEL_ENEMY_LAKITU , bhvHurricaneTower, "Love Tower"   , "Throws projectile that follow enemies" },
        { MODEL_MR_I         , bhvPrismTower    , "iTower"       , "Shoots slow linear projectiles" },
        { MODEL_UKIKI        , bhvAirTower      , "Banana Tower" , "Even faster attacks" },
    },
};

#define TOWER_DEFAULT_RANGE 600.f
#define TOWER_DEFAULT_DAMAGE 25
#define TOWER_DEFAULT_BULLET_SPEED 50.f
#define TOWER_DEFAULT_ATTACK_CD 10

union TowerTypePacked
{
    struct
    {
        // -1 for basic tower, 0 for single upgarde, 1 for double upgrade
        s8 level;
        u8 type0;
        u8 type1;
        u8 totalCost;
    };
    u32 value;
};

union SpawnedStatePacked
{
    struct
    {
        s8 currEnemyGroup;
        u8 totalLeftToSpawn;
        u8 spawnedInCycle;
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
    ENEMY_TOAD,
};

static const u8 kEnemyModels[] = {
    [ ENEMY_GOOMBA ] = MODEL_GOOMBA,
    [ ENEMY_KOOPA ]  = MODEL_KOOPA_WITHOUT_SHELL,
    [ ENEMY_BOBOMB ] = MODEL_BLACK_BOBOMB,
    [ ENEMY_BULLY ]  = MODEL_BULLY,
    [ ENEMY_BOWSER ] = MODEL_BOWSER,
    [ ENEMY_TOAD ]   = MODEL_TOAD,
};

static const f32 kSpeeds[] = { 
    [ ENEMY_GOOMBA ] = 20.f,
    [ ENEMY_KOOPA ]  = 40.f,
    [ ENEMY_BOBOMB ] = 25.f,
    [ ENEMY_BULLY ]  = 10.f,
    [ ENEMY_BOWSER ] = 4.f,
    [ ENEMY_TOAD ]   = 30.f,
};

static const u16 kHealths[] = {
    [ ENEMY_GOOMBA ] = 25,
    [ ENEMY_KOOPA ]  = 10,
    [ ENEMY_BOBOMB ] = 30,
    [ ENEMY_BULLY ]  = 80,
    [ ENEMY_BOWSER ] = 1000,
    [ ENEMY_TOAD ]   = 40,
};

static const u8 kCoinRewards[] = {
    [ ENEMY_GOOMBA ] = 1,
    [ ENEMY_KOOPA ]  = 2,
    [ ENEMY_BOBOMB ] = 2,
    [ ENEMY_BULLY ]  = 5,
    [ ENEMY_BOWSER ] = 50,
    [ ENEMY_TOAD ]   = 0,
};

static const char* kNamesMult[] = {
    "Goombas",
    "Koopas",
    "Bobombs",
    "Bullies",
    "Bowser",
    "The True Fiend",
};

#define TD_BUSY ((void*) 1)
#define TD_CAN_SELL(o) ((int)o < 0)

void bhv_td_init()
{
    o->oTDWave = 1;

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

    gHudDisplay.coins = gMarioStates->numCoins = 20;

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

static void prompt_to_spawn_tower(struct Object** pslot, int upgrade, union TowerTypePacked towerType)
{
#define slot (*pslot)
    print_set_envcolour(255, 255, 255, 255);

    int cost = towerType.totalCost;
    if (gMarioStates->numCoins < cost)
    {
        char line[100];
        sprintf(line, "Need %d coins %s", cost, upgrade ? "for next upgrade" : "to place tower");
        print_small_text_buffered(160, 205, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
        return;
    }

    const struct TowerBeh* towerBeh = NULL;
    struct Color color;
    if (towerType.level < 0)
    {
        towerBeh = &kInitTower;
        color.r = 0xaf;
        color.g = 0xaf;
        color.b = 0xaf;
        color.a = 0xff;
    }
    if (towerType.level == 0)
    {
        towerBeh = &kBasicTowerBehs[towerType.type0];
        color = kTowerColors[towerType.type0];
    }
    if (towerType.level > 0)
    {
        towerBeh = &kComboTowerBehs[towerType.type0][towerType.type1];
        color.r = ((int) kTowerColors[towerType.type0].r + kTowerColors[towerType.type1].r) / 2;
        color.g = ((int) kTowerColors[towerType.type0].g + kTowerColors[towerType.type1].g) / 2;
        color.b = ((int) kTowerColors[towerType.type0].b + kTowerColors[towerType.type1].b) / 2;
        color.a = 0xff;
    }

    char line[100];
    sprintf(line, "A to %s <COL_%02X%02X%02XFF>%s<COL_FFFFFFFF> for %d coins", upgrade ? "upgrade" : "place", color.r, color.g, color.b, towerBeh->name, cost);

    print_set_envcolour(255, 255, 255, 255);
    print_small_text_buffered(160, 205, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
    print_set_envcolour(color.r, color.g, color.b, 255);
    print_small_text_buffered(160, 215, towerBeh->desc, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
    print_set_envcolour(255, 255, 255, 255);

    if (gPlayer1Controller->buttonPressed & A_BUTTON)
    {
        int prevCost = 0;
        if (slot)
        {
            union TowerTypePacked* packed = (union TowerTypePacked*) &slot->oBehParams2ndByte;
            prevCost = packed->totalCost;
            slot->activeFlags = 0;
        }

        gMarioStates->numCoins -= cost;
        gHudDisplay.coins -= cost;
        if (gHudDisplay.coins < 0)
            gHudDisplay.coins = 0;

        slot = spawn_object(o, towerBeh->model, towerBeh->bhv);
        union TowerTypePacked* packed = (union TowerTypePacked*) &slot->oBehParams2ndByte;
        *packed = towerType;
        packed->totalCost += prevCost;
    }
#undef slot
}

static int in_tutorial()
{
    if (1 == o->oTDWave)
        return 1;
    if (2 == o->oTDWave)
        return 0 == o->oAction;

    return 0;
}

static int prompt_to_despawn_tower(struct Object** pslot)
{
    if (in_tutorial())
        return 0;

    print_set_envcolour(255, 255, 255, 255);

#define slot (*pslot)
    union TowerTypePacked* packed = (union TowerTypePacked*) &slot->oBehParams2ndByte;
    int cost = packed->totalCost * 8 / 10;
    
    char line[100];
    sprintf(line, "B to destroy tower, returns %d coins", cost);
    print_small_text_buffered(160, 185, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);

    if (gPlayer1Controller->buttonPressed & B_BUTTON)
    {
        gMarioStates->numCoins += cost;
        slot->activeFlags = 0;
        slot = NULL;
        return 1;
    }
    else
    {
        return 0;
    }
#undef slot
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

    if (slot)
    {
        if (0 == prompt_to_despawn_tower(pslot) && -1 != o->oTDSelectedTower)
        {
            union TowerTypePacked* packed = (union TowerTypePacked*) &slot->oBehParams2ndByte;
            if (packed->level == -1)
                prompt_to_spawn_tower(pslot, 1, (union TowerTypePacked) { .level = 0, .type0 = o->oTDSelectedTower                        , .totalCost = 20 });
            if (packed->level == 0)
                prompt_to_spawn_tower(pslot, 1, (union TowerTypePacked) { .level = 1, .type0 = packed->type0, .type1 = o->oTDSelectedTower, .totalCost = 50 });
        }
    }
    else
    {
        int canSpawnSimpleTower = 1;
        if (in_tutorial())
        {
            canSpawnSimpleTower = 0;
            if (1 == o->oTDWave && 0 == o->oAction)
                canSpawnSimpleTower = 1;
        }

        if (canSpawnSimpleTower)
            prompt_to_spawn_tower(pslot, 0, (union TowerTypePacked) { .level = -1, .totalCost = 10 });
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

struct WaveEnemyGroup
{
    u8 enemy;
    u8 count;
    u8 cycleSwitch;
};

struct WaveDesc
{
    struct WaveEnemyGroup enemies[4];
};



static const struct WaveDesc kWaveDescs[] = {
    { (struct WaveEnemyGroup){ ENEMY_GOOMBA, 20, 0 }, },
    { (struct WaveEnemyGroup){ ENEMY_GOOMBA, 40, 0 }, },
    { (struct WaveEnemyGroup){ ENEMY_GOOMBA, 20, 2 }, (struct WaveEnemyGroup){ ENEMY_KOOPA, 10, 1 } },
    { (struct WaveEnemyGroup){ ENEMY_KOOPA, 50, 0 } },
    { (struct WaveEnemyGroup){ ENEMY_KOOPA, 20, 1 }, (struct WaveEnemyGroup){ ENEMY_BULLY, 10, 1 } },

    { (struct WaveEnemyGroup){ ENEMY_BOWSER, 1, 0 } },

    { (struct WaveEnemyGroup){ ENEMY_BOBOMB, 20, 2 }, (struct WaveEnemyGroup){ ENEMY_KOOPA, 10, 1 } },
    { (struct WaveEnemyGroup){ ENEMY_BULLY, 15, 0 } },
    { (struct WaveEnemyGroup){ ENEMY_GOOMBA, 20, 1 } , (struct WaveEnemyGroup){ ENEMY_BOBOMB, 20, 1 } },

    { (struct WaveEnemyGroup){ ENEMY_BOWSER, 1, 0 } },
    { (struct WaveEnemyGroup){ ENEMY_TOAD, 1, 0 } },
};

static void handle_wave_spawning()
{
    print_set_envcolour(255, 255, 255, 255);
    char line[100];
    switch (o->oAction)
    {
        case TD_INIT:
            int canSpawnNextWave = 1;
            if (o->oTDWave < 3)
            {
                if (0 != gMarioStates->numCoins)
                    canSpawnNextWave = 0;
            }

            if (canSpawnNextWave)
            {
                sprintf(line, "Press L to spawn wave %d", o->oTDWave);
                print_small_text_buffered(160, 10, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);

                const struct WaveDesc* waveDesc = &kWaveDescs[o->oTDWave - 1];
                int lineOff = 0;
                for (int i = 0; i < 4; i++)
                {
                    const struct WaveEnemyGroup* waveGroup = &waveDesc->enemies[i];
                    if (waveGroup->count == 0)
                        break;

                    if (0 == lineOff)
                    {
                        lineOff += sprintf(line + lineOff, "%d %s", waveGroup->count, kNamesMult[waveGroup->enemy]);
                    }
                    else
                    {
                        lineOff += sprintf(line + lineOff, " & %d %s", waveGroup->count, kNamesMult[waveGroup->enemy]);
                    }
                }
                print_small_text_buffered(160, 22, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);

                if (gPlayer1Controller->buttonPressed & L_TRIG)
                {
                    o->oAction = TD_WAVE_SPAWNING;

                    const struct WaveDesc* waveDesc = &kWaveDescs[o->oTDWave - 1];
                    int totalEnemies = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        totalEnemies += waveDesc->enemies[i].count;
                    }

                    union SpawnedStatePacked* spawnedState = (union SpawnedStatePacked*) &o->oTDWaveSpawningState;
                    spawnedState->value = 0;
                    spawnedState->totalLeftToSpawn = totalEnemies;
                    o->oTDWaveLeftEnemies = totalEnemies;
                    o->oTDWaveTotalEnemyCount = totalEnemies;
                    o->oTDWaveSpawningAmountSpawned = 0;
                }
            }
            else
            {
                if (1 == o->oTDWave)
                {
                    print_small_text_buffered(160, 10, "Welcome to Element TD!", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
                    print_small_text_buffered(160, 22, "Place 2 towers using A button", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
                }
                if (2 == o->oTDWave)
                {
                    if (-1 == o->oTDSelectedTower)
                    {
                        print_small_text_buffered(160, 10, "Select an Element using dpad", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
                    }
                    else
                    {
                        print_small_text_buffered(160, 10, "Upgrade tower using Element", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
                    }
                }
            }

            break;
        case TD_WAVE_SPAWNING:
            sprintf(line, "Wave %d, Left %d/%d", o->oTDWave, o->oTDWaveLeftEnemies, o->oTDWaveTotalEnemyCount);
            print_small_text_buffered(160, 10, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
            if (1 == o->oTDWave)
            {
                print_set_envcolour(255, 200, 200, 255);
                print_small_text_buffered(160, 22, "Use C Buttons to speed up", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
            }

            if (0 == (o->oTimer % 8))
            {
                union SpawnedStatePacked* spawnedState = (union SpawnedStatePacked*) &o->oTDWaveSpawningState;
                u8* amountSpawnedAll = (u8*) &o->oTDWaveSpawningAmountSpawned;
                const struct WaveDesc* waveDesc = &kWaveDescs[o->oTDWave - 1];

                // perform spawning for the current group
                {
                    const struct WaveEnemyGroup* waveGroup = &waveDesc->enemies[spawnedState->currEnemyGroup];
                    enum EnemyTypes enemyType = waveGroup->enemy;

                    struct Object* enemy = spawn_object(o, kEnemyModels[enemyType], bhvTdEnemy);
                    enemy->oBehParams2ndByte = enemyType;
                    enemy->oForwardVel = kSpeeds[enemy->oBehParams2ndByte];
                    enemy->oDamageOrCoinValue = enemy->oHealth = kHealths[enemy->oBehParams2ndByte] * (0.5f + 0.35f * (o->oTDWave * o->oTDWave));
                    enemy->oPosX = -1143;
                    enemy->oPosY = -200;
                    enemy->oPosZ = -1600;

                    u8* amountSpawned = &amountSpawnedAll[spawnedState->currEnemyGroup];
                    (*amountSpawned)++;
                    spawnedState->totalLeftToSpawn--;
                    spawnedState->spawnedInCycle++;
                }

                if (0 != spawnedState->totalLeftToSpawn)
                {
                    // switch to next group by searching the first group that has not spawned all enemies
                    for (int i = 0; i < 4; i++)
                    {
                        int group = spawnedState->currEnemyGroup;
                        u8* amountSpawned = &amountSpawnedAll[group];
                        const struct WaveEnemyGroup* waveGroup = &waveDesc->enemies[group];
                        if (*amountSpawned == waveGroup->count || spawnedState->spawnedInCycle == waveGroup->cycleSwitch)
                        {
                            spawnedState->currEnemyGroup++;
                            spawnedState->currEnemyGroup &= 3;
                            spawnedState->spawnedInCycle = 0;
                        }
                    }
                }
                else
                {
                    o->oAction = TD_WAVE_WAITING;
                }
            }

            break;

        case TD_WAVE_WAITING:
            sprintf(line, "Wave %d, Left %d/%d", o->oTDWave, o->oTDWaveLeftEnemies, o->oTDWaveTotalEnemyCount);
            print_small_text_buffered(160, 10, line, PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
            if (1 == o->oTDWave)
            {
                print_set_envcolour(255, 200, 200, 255);
                print_small_text_buffered(160, 22, "Use C Buttons to speed up", PRINT_TEXT_ALIGN_CENTRE, PRINT_ALL, FONT_OUTLINE);
            }

            if (0 == o->oTDWaveLeftEnemies)
            {
                o->oAction = TD_INIT;
                o->oTDWave++;
            }
            break;

        case TD_WAIT_NEXT_WAVE:
            break;
    }
}

void bhv_td_loop()
{
    gMarioStates->pos[1] = 200.f;
    gMarioStates->action = ACT_DEBUG_FREE_MOVE;
    gCamera->cutscene = CUTSCENE_TD;

    handle_tower_style_selection();
    handle_tower_spawning();
    handle_wave_spawning();

    if (o->oTDWave > 1)
    {
        render_tower_labels();
    }
}

struct TdTrajPoint
{
    s16 x, z;
};

struct TdTrajPointF
{
    f32 x, z;
};

static const struct TdTrajPoint kTdEnemyPath[] =
{
    { -1143, -1600 },
    { -1143, 127 },
    { -381, 127 },
    { -381, -381 },
    { 381, -381 },
    { 381, 635 },
    { -1143, 635 },
    { -1143, 1143 },
    { 1143, 1143 },
    { 1143, -889 },
    { -381, -889 },
    { -381, -1800 },
};

#define oTdEnemyProgress oF4
#define oTdEnemySpeedDebuff oF8

static void td_enemy_advance()
{
    if (o->oPosZ < -1610.f && o->activeFlags)
    {
        o->activeFlags = 0;
        o->parentObj->oTDWaveLeftEnemies--;
        gMarioStates->health -= 0x100;
        return;
    }

    const struct TdTrajPoint* curPoint   = &kTdEnemyPath[o->oTdEnemyProgress];
    const struct TdTrajPoint* nextPoint  = &kTdEnemyPath[o->oTdEnemyProgress + 1];
    const struct TdTrajPoint* nextPoint2 = &kTdEnemyPath[o->oTdEnemyProgress + 2];
    f32* posToChange = NULL;
    f32* nextPosToChange = NULL;
    const s16* pointToCompare = NULL;
    s16 nextDirectionPositive = 0;
    f32 vel;
    if (curPoint->x == nextPoint->x)
    {
        posToChange = &o->oPosZ;
        nextPosToChange = &o->oPosX;
        pointToCompare = &nextPoint->z;
        nextDirectionPositive = nextPoint->x > nextPoint2->x;
        vel = o->oForwardVel - o->oTdEnemySpeedDebuff;
        o->oFaceAngleYaw = 0x0;
        if (vel < 0)
            vel = 0;

        if (curPoint->z > nextPoint->z)
        {
            vel = -vel;
            o->oFaceAngleYaw += 0x8000;
        }
    }
    else
    {
        posToChange = &o->oPosX;
        nextPosToChange = &o->oPosZ;
        pointToCompare = &nextPoint->x;
        nextDirectionPositive = nextPoint->z > nextPoint2->z;
        vel = o->oForwardVel - o->oTdEnemySpeedDebuff;
        o->oFaceAngleYaw = 0x4000;
        if (vel < 0)
            vel = 0;

        if (curPoint->x > nextPoint->x)
        {
            vel = -vel;
            o->oFaceAngleYaw += 0x8000;
        }
    }

    *posToChange += vel;
    if (vel > 0)
    {
        if (*posToChange > *pointToCompare)
        {
            o->oTdEnemyProgress++;
            f32 left = *posToChange - *pointToCompare;
            *nextPosToChange += left * (nextDirectionPositive ? 1 : -1);
        }
    }
    else
    {
        if (*posToChange < *pointToCompare)
        {
            o->oTdEnemyProgress++;
            f32 left = *pointToCompare - *posToChange;
            *nextPosToChange += left * (nextDirectionPositive ? 1 : -1);
        }
    }
}

void bhv_td_enemy_init()
{
    struct Object* hp = spawn_object(o, MODEL_HP, bhvTdHealthBar);
    o->oOpacity = 255;
    if (ENEMY_BOWSER == o->oBehParams2ndByte)
        obj_scale_xyz(hp, 2.f, 2.f, 6.f);
}

void bhv_td_enemy_loop()
{
    if (o->oTdEnemySpeedDebuff)
        o->oTdEnemySpeedDebuff--;

    td_enemy_advance();
    enum EnemyTypes enemyType = (enum EnemyTypes) o->oBehParams2ndByte;
    switch (enemyType)
    {
        case ENEMY_GOOMBA:
            o->oAnimations = (void*) goomba_seg8_anims_0801DA4C;
            cur_obj_init_animation_with_accel_and_sound(GOOMBA_ANIM_DEFAULT, 1.f);
            break;
        case ENEMY_KOOPA:
            o->oAnimations = (void*) koopa_seg6_anims_06011364;
            cur_obj_init_animation_with_accel_and_sound(KOOPA_ANIM_WALK, 1.f);
            break;
        case ENEMY_BOBOMB:
            o->oAnimations = (void*) bobomb_seg8_anims_0802396C;
            cur_obj_init_animation_with_accel_and_sound(BOBOMB_ANIM_WALKING, 1.f);
            break;
        case ENEMY_BULLY:
            o->oAnimations = (void*) bully_seg5_anims_0500470C;
            cur_obj_init_animation_with_accel_and_sound(0, 1.f);
            break;
        case ENEMY_BOWSER:
            o->oAnimations = (void*) bowser_seg6_anims_06057690;
            cur_obj_init_animation_with_accel_and_sound(13, 1.f);
            break;
        case ENEMY_TOAD:
            o->oAnimations = (void*) toad_seg6_anims_0600FB58;
            cur_obj_init_animation_with_accel_and_sound(TOAD_ANIM_WEST_WALKING, 1.f);
            break;
    }
}

#define oTdBulletEnemy oObjF4
#define oTdBulletSpeedDebuff oF8
#define oTdBulletRelookupRange oFloatFC
#define oTdBulletReferencedEnemies oObj100

// F4 and F8 are booked by snufit code
#define oTdTowerCooldown oFC

#define BULLET_SPAWN_FLAME_LINGER 1
#define BULLET_SPAWN_FLAME_LINGER2 2
#define BULLET_FLIP 3
#define BULLET_INSTA_KILL 4
#define BULLET_JUMP1 5
#define BULLET_JUMP2 6

static struct Object* shoot_closest_enemy(int model, f32 modelScale, int dmg, f32 range, f32 bulletVel, int cd)
{
    if (o->oTdTowerCooldown) 
    {
        o->oTdTowerCooldown--;
        return NULL;
    }

    f32 dist;
    struct Object* enemy = cur_obj_find_nearest_object_with_behavior(bhvTdEnemy, &dist);
    if (!enemy)
        return NULL;

    s32 angleTowards = obj_angle_to_object(o, enemy);
    cur_obj_rotate_yaw_toward(angleTowards, 0x800);

    if (dist > range)
        return NULL;

    o->oFaceAngleYaw = angleTowards;

    struct Object* bullet = spawn_object(o, model, bhvTdBullet);
    bullet->oBehParams2ndByte = dmg;
    bullet->oForwardVel = bulletVel;
    bullet->oTdBulletEnemy = enemy;
    bullet->oTdBulletRelookupRange = range / 1.2f;
    bullet->oBehParams = 0;
    obj_scale(bullet, modelScale);
    o->oTdTowerCooldown = cd;

    return bullet;
}

extern struct Object *cur_obj_find_furthest_object_with_behavior(const BehaviorScript *behavior, f32 *dist);
static struct Object* shoot_furthest_enemy(int model, f32 modelScale, int dmg, f32 bulletVel, int cd)
{
    if (o->oTdTowerCooldown) 
    {
        o->oTdTowerCooldown--;
        return NULL;
    }

    f32 dist;
    struct Object* enemy = cur_obj_find_furthest_object_with_behavior(bhvTdEnemy, &dist);
    if (!enemy)
        return NULL;

    s32 angleTowards = obj_angle_to_object(o, enemy);
    cur_obj_rotate_yaw_toward(angleTowards, 0x800);

    o->oFaceAngleYaw = angleTowards;

    struct Object* bullet = spawn_object(o, model, bhvTdBullet);
    bullet->oBehParams2ndByte = dmg;
    bullet->oForwardVel = bulletVel;
    bullet->oTdBulletEnemy = enemy;
    bullet->oTdBulletRelookupRange = 3000.f; // chose somewhat arbitrary
    obj_scale(bullet, modelScale);
    o->oTdTowerCooldown = cd;

    return bullet;
}

static void deal_enemy_damage(struct Object* enemy, int dmg)
{
    if (!enemy->activeFlags)
        return;

    enemy->oHealth -= dmg;
    if (enemy->oHealth <= 0)
    {
        enemy->activeFlags = 0;
        enemy->parentObj->oTDWaveLeftEnemies--;
        gMarioStates->numCoins += kCoinRewards[enemy->oBehParams2ndByte];
    }
}

static struct Object *cur_obj_find_nearest_object_with_behavior_excluding_prev_hit(const BehaviorScript *behavior, f32 *dist)
{
    struct Object** prevHitEnemies = &o->oTdBulletReferencedEnemies;
    uintptr_t *behaviorAddr = segmented_to_virtual(behavior);
    struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
    struct Object *obj = (struct Object *) listHead->next;
    struct Object *closestObj = NULL;
    f32 minDist = 0x20000;

    while (obj != (struct Object *) listHead) {
        if (obj->behavior == behaviorAddr
            && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED
            && obj != prevHitEnemies[0]
            && obj != prevHitEnemies[1]
        ) {
            f32 objDist = dist_between_objects(o, obj);
            if (objDist < minDist) {
                closestObj = obj;
                minDist = objDist;
            }
        }

        obj = (struct Object *) obj->header.next;
    }

    *dist = minDist;
    return closestObj;
}


void bhv_td_bullet_loop()
{
    struct Object** prevHitEnemies = &o->oTdBulletReferencedEnemies;
    if (o->oTdBulletEnemy == NULL)
    {
        // refind the enemy, within reasonable range
        f32 d;
        o->oTdBulletEnemy = cur_obj_find_nearest_object_with_behavior_excluding_prev_hit(bhvTdEnemy, &d);
        if (!o->oTdBulletEnemy)
        {
            o->activeFlags = 0;
            return;
        }

        if (d > o->oTdBulletRelookupRange)
        {
            o->activeFlags = 0;
            return;
        }
    }

    Vec3f d;
    vec3_diff(d, &o->oPosVec, &o->oTdBulletEnemy->oPosVec);
    f32 dmag = vec3_mag(d);
    if (dmag < o->oForwardVel + 5.f)
    {
        int activeFlagsBackup = o->activeFlags;
        o->activeFlags = 0;

        deal_enemy_damage(o->oTdBulletEnemy, o->oBehParams2ndByte);
        if (o->oTdBulletSpeedDebuff)
        {
            o->oTdBulletEnemy->oTdEnemySpeedDebuff = o->oTdBulletEnemy->oForwardVel * o->oTdBulletSpeedDebuff / 50;
        }

        switch (o->oBehParams)
        {
            case BULLET_SPAWN_FLAME_LINGER:
            case BULLET_SPAWN_FLAME_LINGER2:
            {
                struct Object* flame = spawn_object(o, MODEL_BLUE_FLAME, bhvTdFlameLinger);
                flame->oPosX = o->oTdBulletEnemy->oPosX;
                flame->oPosY = o->oTdBulletEnemy->oPosY;
                flame->oPosZ = o->oTdBulletEnemy->oPosZ;
                // this is required to properly manage oTDWaveLeftEnemies
                flame->parentObj = o->oTdBulletEnemy->parentObj;
                if (BULLET_SPAWN_FLAME_LINGER2 == o->oBehParams)
                    flame->oTdBulletEnemy = o->oTdBulletEnemy;
                else
                    flame->oTdBulletEnemy = NULL;
            }
            break;

            case BULLET_INSTA_KILL:
            {
                if (random_u16() < (0x10000 * 0.05f))
                {
                    deal_enemy_damage(o->oTdBulletEnemy, 100000);
                }
            }
            break;

            case BULLET_JUMP1:
            case BULLET_JUMP2:
                prevHitEnemies[o->oBehParams - BULLET_JUMP1] = o->oTdBulletEnemy;
                f32 d;
                o->oTdBulletEnemy = cur_obj_find_nearest_object_with_behavior_excluding_prev_hit(bhvTdEnemy, &d);
                if (o->oTdBulletEnemy)
                {
                    o->activeFlags = activeFlagsBackup;
                    o->oBehParams++;
                    o->oBehParams2ndByte /= 2;
                }
                break;
        }
    }
    else
    {
        d[0] /= dmag;
        d[1] /= dmag;
        d[2] /= dmag;
        o->oPosX -= d[0] * o->oForwardVel;
        o->oPosY -= d[1] * o->oForwardVel;
        o->oPosZ -= d[2] * o->oForwardVel;
    }
}

void bhv_tower_init()
{
    o->oFaceAnglePitch = 0x3000;
    obj_scale(o, 0.7f);
}

void bhv_tower_loop()
{
    shoot_closest_enemy(MODEL_WATER_BOMB, 0.7f, TOWER_DEFAULT_DAMAGE, TOWER_DEFAULT_RANGE, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD);
}

void bhv_fire_tower_init()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        obj_scale(o, 2.f);
        o->oAnimations = (void*) flyguy_seg8_anims_08011A64;
        struct Animation **animations = o->oAnimations;
        s32 animIndex = FLY_GUY_ANIM_FLYING;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        o->oAnimations = (void*) piranha_plant_seg6_anims_0601C31C;
        s32 animIndex = PIRANHA_PLANT_ANIM_BITE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_fire_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        struct Object* bullet = shoot_closest_enemy(MODEL_RED_FLAME, 5.f, TOWER_DEFAULT_DAMAGE / 0.8f, TOWER_DEFAULT_RANGE, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD);
        if (bullet)
            bullet->oBehParams = BULLET_SPAWN_FLAME_LINGER;
    }
    else
    {
        struct Object* bullet = shoot_closest_enemy(MODEL_RED_FLAME, 8.f, TOWER_DEFAULT_DAMAGE * 2, TOWER_DEFAULT_RANGE, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD);
        if (bullet)
            bullet->oBehParams = BULLET_SPAWN_FLAME_LINGER2;
    }
}

void bhv_water_tower_init()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        o->oAnimations = (void*) snowman_seg5_anims_0500D118;
        s32 animIndex = MR_BLIZZARD_ANIM_SPAWN_SNOWBALL;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        obj_scale(o, 1.7f);
        o->oAnimations = (void*) penguin_seg5_anims_05008B74;
        s32 animIndex = PENGUIN_ANIM_IDLE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_water_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        struct Object* bullet = shoot_closest_enemy(MODEL_WHITE_PARTICLE, 2.f, TOWER_DEFAULT_DAMAGE * 1.2f, TOWER_DEFAULT_RANGE * 1.2f, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD / 1.2f);
        if (bullet)
            bullet->oTdBulletSpeedDebuff = 49;
    }
    else
    {
        struct Object* bullet = shoot_closest_enemy(MODEL_WHITE_PARTICLE, 2.5f, TOWER_DEFAULT_DAMAGE * 1.2f, TOWER_DEFAULT_RANGE * 1.2f, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD / 1.2f);
        if (bullet)
        {
            bullet->oTdBulletSpeedDebuff = 49;
            {
                uintptr_t *behaviorAddr = segmented_to_virtual(bhvTdEnemy);
                struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
                struct Object *obj = (struct Object *) listHead->next;

                while (obj != (struct Object *) listHead) {
                    if (obj->behavior == behaviorAddr
                        && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED
                    ) {
                        f32 objDist = dist_between_objects(o, obj);
                        if (objDist < 600.f)
                        {
                            obj->oTdEnemySpeedDebuff = obj->oForwardVel * 0.5f;
                        }
                    }

                    obj = (struct Object *) obj->header.next;
                }
            }
        }
    }
}

void bhv_crystal_tower_init()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        o->oSnufitRecoil = 0;
        o->oSnufitBodyScale = 1000;
        o->oPosY += 100.f;
    }
    else
    {
        obj_scale(o, 1.5f);
        o->oAnimations = (void*) heave_ho_seg5_anims_0501534C;
        s32 animIndex = HEAVE_HO_ANIM_MOVING;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);

        o->oHomeX = o->oPosX;
        o->oHomeY = o->oPosY;
        o->oHomeZ = o->oPosZ;
    }

    o->parentObj = NULL;
}

void bhv_crystal_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        obj_scale(o, 2.f + sins(o->oTimer * 0x456) / 10.f);
        shoot_furthest_enemy(MODEL_BOWLING_BALL, 0.7f, TOWER_DEFAULT_DAMAGE, TOWER_DEFAULT_BULLET_SPEED * 2, TOWER_DEFAULT_ATTACK_CD);
    }
    else
    {
        if (15 == o->oTimer)
        {
            f32 dist;
            struct Animation **animations = o->oAnimations;
            struct Object* enemy = cur_obj_find_furthest_object_with_behavior(bhvTdEnemy, &dist);
            if (!enemy)
            {
                o->oTimer = 14;
                vec3_copy(&o->oPosVec, &o->oHomeVec);
                geo_obj_init_animation(&o->header.gfx, &animations[HEAVE_HO_ANIM_MOVING]);
            }
            else
            {
                o->oTimer = -1;
                geo_obj_init_animation(&o->header.gfx, &animations[HEAVE_HO_ANIM_THROW]);
                o->parentObj = enemy;
                o->oPosX = enemy->oPosX;
                o->oPosY = enemy->oPosY;
                o->oPosZ = enemy->oPosZ;
            }
        }
        else
        {
            if (o->parentObj)
            {
                if (o->oTimer < 5) {
                    vec3_copy(&o->parentObj->oPosVec, &o->oPosVec);
                } else {
                    o->parentObj->oPosY = o->oPosY;
                }

                o->parentObj->oPosY += sins(o->oTimer * 0x8000 / 14) * 500.f;
                deal_enemy_damage(o->parentObj, 5);
            }
        }
    }
}

void bhv_air_tower_init()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        obj_scale(o, 2.f);
        o->oAnimations = (void*) monty_mole_seg5_anims_05007248;
        s32 animIndex = MONTY_MOLE_ANIM_BEGIN_JUMP_INTO_HOLE;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        obj_scale(o, 1.5f);
        o->oAnimations = (void*) ukiki_seg5_anims_05015784;
        s32 animIndex = UKIKI_ANIM_RUN;
        struct Animation **animations = o->oAnimations;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
}

void bhv_air_tower_loop()
{
    union TowerTypePacked* packed = (union TowerTypePacked*) &o->oBehParams2ndByte;
    if (0 == packed->level)
    {
        shoot_closest_enemy(MODEL_DIRT, 0.7f, TOWER_DEFAULT_DAMAGE, TOWER_DEFAULT_RANGE, TOWER_DEFAULT_BULLET_SPEED * 2.f, TOWER_DEFAULT_ATTACK_CD / 2);
    }
    else
    {
        shoot_closest_enemy(MODEL_DIRT, 0.7f, TOWER_DEFAULT_DAMAGE, TOWER_DEFAULT_RANGE, TOWER_DEFAULT_BULLET_SPEED * 4.f, TOWER_DEFAULT_ATTACK_CD / 4);
    }
}

static void deal_damage_around(f32 range, int dmg)
{
    uintptr_t *behaviorAddr = segmented_to_virtual(bhvTdEnemy);
    struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
    struct Object *obj = (struct Object *) listHead->next;

    while (obj != (struct Object *) listHead) {
        if (obj->behavior == behaviorAddr
            && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED
        ) {
            f32 objDist = dist_between_objects(o, obj);
            if (objDist < range)
            {
                deal_enemy_damage(obj, dmg);
            }
        }

        obj = (struct Object *) obj->header.next;
    }
}

void bhv_steam_tower_init()
{
    o->oOpacity = 255;
}

void bhv_steam_tower_loop()
{
    obj_scale(o, 0.9f + sins(o->oTimer * 0x456) / 10.f);
    deal_damage_around(400.f, 3);
}

void bhv_spire_tower_init()
{
    obj_scale(o, 1.2f);
    o->oAnimations = (void*) yoshi_seg5_anims_05024100;
    struct Animation **animations = o->oAnimations;
    s32 animIndex = YOSHI_ANIM_IDLE;
    geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
}

void bhv_spire_tower_loop()
{
    struct Object* bullet = shoot_closest_enemy(MODEL_YOSHI_EGG, 0.3f, TOWER_DEFAULT_DAMAGE / 25, TOWER_DEFAULT_RANGE * 2, TOWER_DEFAULT_BULLET_SPEED, TOWER_DEFAULT_ATTACK_CD / 2);
    if (bullet)
        bullet->oBehParams = BULLET_INSTA_KILL;
}

void bhv_td_flame_loop()
{
    deal_damage_around(300.f, 6);
}

void bhv_inferno_tower_init()
{
    obj_scale(o, 1.2f);
    o->oAnimations = (void*) blargg_seg5_anims_0500616C;
    struct Animation **animations = o->oAnimations;
    s32 animIndex = 0;
    geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);

    o->parentObj = spawn_object(o, MODEL_RED_FLAME, bhvTdFlame);
    o->parentObj->oTdBulletEnemy = NULL;
    obj_scale(o->parentObj, 5.f);
    obj_scale(o, 0.5f);
}

void bhv_inferno_tower_loop()
{
    o->parentObj->oPosX = o->oPosX + sins(o->oTimer * 0x756) * 250.f;
    o->parentObj->oPosY = o->oPosY;
    o->parentObj->oPosZ = o->oPosZ + coss(o->oTimer * 0x756) * 250.f;
}

void bhv_shard_tower_init()
{
    obj_scale(o, 0.7f);
    o->oAnimations = (void*) manta_seg5_anims_05008EB4;
    struct Animation **animations = o->oAnimations;
    s32 animIndex = 0;
    geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
}

void bhv_shard_tower_loop()
{
    struct Object* bullet = shoot_furthest_enemy(MODEL_WATER_SPLASH, 0.7f, TOWER_DEFAULT_DAMAGE * 2, TOWER_DEFAULT_BULLET_SPEED * 2, TOWER_DEFAULT_ATTACK_CD);
    if (bullet)
        bullet->oBehParams = BULLET_JUMP1;
}

void bhv_hurricane_tower_init()
{
    obj_scale(o, 1.2f);
    o->oAnimations = (void*) lakitu_enemy_seg5_anims_050144D4;
    struct Animation **animations = o->oAnimations;
    s32 animIndex = ENEMY_LAKITU_ANIM_SPAWN;
    geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    o->parentObj = NULL;
}

void bhv_hurricane_tower_loop()
{
    if (!o->parentObj)
    {
        f32 d;
        struct Object* enemy = cur_obj_find_nearest_object_with_behavior(bhvTdEnemy, &d);
        if (d < TOWER_DEFAULT_RANGE * 3.f)
        {
            o->parentObj = spawn_object(o, MODEL_SPINY_BALL, bhvTdSpiny);
            o->oFaceAngleYaw = o->parentObj->oFaceAngleYaw = obj_angle_to_object(o, enemy);
            o->parentObj->oForwardVel = 60.f;
        }
    }
    else
    {
        if (0 == o->activeFlags)
        {
            // Handle the case where the tower is destroyed cleanly
            o->parentObj->activeFlags = 0;
            o->parentObj = NULL;
        }
    }
}

void bhv_td_spiny_loop()
{
    f32 dist;
    struct Object* enemy = cur_obj_find_nearest_object_with_behavior(bhvTdEnemy, &dist);
    if (!enemy)
    {
        o->parentObj->parentObj = NULL;
        o->activeFlags = 0;
        return;
    }

    if (o->oAction && dist > 200.f)
    {
        o->parentObj->parentObj = NULL;
        o->activeFlags = 0;
        return;
    }

    Vec3f d;
    vec3_diff(d, &o->oPosVec, &enemy->oPosVec);
    f32 dmag = vec3_mag(d);
    if (dmag < o->oForwardVel + 5.f)
    {
        o->oAction = 1;
        cur_obj_set_model(MODEL_SPINY);
        o->oForwardVel = 5.f;
        o->oAnimations = (void*) spiny_seg5_anims_05016EAC;
        struct Animation **animations = o->oAnimations;
        s32 animIndex = SPINY_ANIM_DEFAULT;
        geo_obj_init_animation(&o->header.gfx, &animations[animIndex]);
    }
    else
    {
        d[0] /= dmag;
        d[1] /= dmag;
        d[2] /= dmag;
        o->oPosX -= d[0] * o->oForwardVel;
        o->oPosY -= d[1] * o->oForwardVel;
        o->oPosZ -= d[2] * o->oForwardVel;
    }

    deal_damage_around(200.f, 10);
}

void bhv_prism_tower_init()
{
    o->oPosY += 100.f;
    o->parentObj = NULL;
}

void bhv_prism_tower_loop()
{
    if (!o->parentObj)
    {
        f32 d;
        struct Object* enemy = cur_obj_find_nearest_object_with_behavior(bhvTdEnemy, &d);
        if (d < TOWER_DEFAULT_RANGE * 1.5f)
        {
            o->parentObj = spawn_object(o, MODEL_PURPLE_MARBLE, bhvTdFlame);
            o->parentObj->oTdBulletEnemy = NULL;
            o->oFaceAngleYaw = o->parentObj->oFaceAngleYaw = obj_angle_to_object(o, enemy);
            o->parentObj->oPosY -= 100.f;
            obj_scale(o->parentObj, 5.f);
        }
    }
    else
    {
        o->parentObj->oPosX += sins(o->parentObj->oFaceAngleYaw) * 30.f;
        o->parentObj->oPosZ += coss(o->parentObj->oFaceAngleYaw) * 30.f;
        if (absf(o->parentObj->oPosX) > 1500.f || absf(o->parentObj->oPosZ) > 1500.f)
        {
            o->parentObj->activeFlags = 0;
            o->parentObj = NULL;
        }
    }

    o->oFaceAngleYaw += 0x100;
    o->oMoveAngleYaw = o->oFaceAngleYaw;
}

void bhv_td_flame_linger_loop()
{
    f32 mult = BULLET_SPAWN_FLAME_LINGER2 == o->oBehParams ? 2.f : 1.f;
    obj_scale(o, mult * (60 - o->oTimer) / 8.f);
    if (o->oTimer == 59)
    {
        o->activeFlags = 0;
    }
    f32 range = 10.f * ((60 - o->oTimer) / 8.f);

    if (o->oTdBulletEnemy)
    {
        if (!o->oTdBulletEnemy->activeFlags)
            o->oTdBulletEnemy = NULL;
    }
    if (o->oTdBulletEnemy)
    {
        o->oPosX = o->oTdBulletEnemy->oPosX;
        o->oPosY = o->oTdBulletEnemy->oPosY;
        o->oPosZ = o->oTdBulletEnemy->oPosZ;
    }

    deal_damage_around(range * mult, 2 * mult * mult);
}

Gfx *geo_hp(s32 callContext, struct GraphNode *node, Mat4 mtx)
{
    // rawdog the hud rendering
    struct GraphNodeGenerated *graphNode = (struct GraphNodeGenerated *) node;
    Gfx *dlist = NULL;

    if (callContext == GEO_CONTEXT_RENDER) {
        Vtx *verts = alloc_display_list(6 * sizeof(Vtx));
        dlist = alloc_display_list(12 * sizeof(Gfx));
        Gfx* dplistp = dlist;

        struct Object *objectGraphNode = (struct Object *) gCurGraphNodeObject;

        const s16 left = -247 / 2;
        const s16 right = 247 / 2;
        int relHp = objectGraphNode->oHealth * (right - left) / objectGraphNode->oDamageOrCoinValue;
        s16 mid = relHp + left;

        verts[0].v.ob[0] = 0;
        verts[0].v.ob[1] = 310 / 2;
        verts[0].v.ob[2] = right / 4;

        verts[1].v.ob[0] = 0;
        verts[1].v.ob[1] = 238 / 2;
        verts[1].v.ob[2] = right / 4;

        verts[2].v.ob[0] = 0;
        verts[2].v.ob[1] = 238 / 2;
        verts[2].v.ob[2] = mid / 4;

        verts[3].v.ob[0] = 0;
        verts[3].v.ob[1] = 310 / 2;
        verts[3].v.ob[2] = mid / 4;

        verts[4].v.ob[0] = 0;
        verts[4].v.ob[1] = 238 / 2;
        verts[4].v.ob[2] = left / 4;

        verts[5].v.ob[0] = 0;
        verts[5].v.ob[1] = 310 / 2;
        verts[5].v.ob[2] = left / 4;

	    gSPVertex(dplistp++, PHYSICAL_TO_VIRTUAL(verts), 6, 0);

        gDPSetPrimColor(dplistp++, 0, 0, 0, 0, 0, 255);
	    gSP2Triangles(dplistp++, 0, 2, 1, 0, 0, 3, 2, 0);

        gDPSetPrimColor(dplistp++, 0, 0, 255 - relHp, relHp, 0, 255);
	    gSP2Triangles(dplistp++, 0+4, 2, 1+4, 0, 0+4, 2, 3, 0);

        gSPEndDisplayList(dplistp);

        SET_GRAPH_NODE_LAYER(graphNode->fnNode.node.flags, LAYER_HP);
    }
    return dlist;
}

void bhv_td_healthbar_init()
{
    o->oFaceAngleYaw = 0x4000;
    o->oFaceAngleRoll = -0x4000;
    o->oDamageOrCoinValue = o->parentObj->oDamageOrCoinValue;
}

void bhv_td_healthbar_loop()
{
    if (0 == o->parentObj->activeFlags)
    {
        o->activeFlags = 0;
        return;
    }

    o->oHealth = o->parentObj->oHealth;

    vec3_copy(&o->oPosVec, &o->parentObj->oPosVec);
}

static void clear_bullet_enemy_unallocs(const BehaviorScript* bhv)
{
    uintptr_t *behaviorAddr = segmented_to_virtual(bhv);
    struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
    struct Object *obj = (struct Object *) listHead->next;

    while (obj != (struct Object *) listHead) {
        if (obj->behavior == behaviorAddr && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) {
            if (obj->oTdBulletEnemy && obj->oTdBulletEnemy->activeFlags == 0)
            {
                obj->oTdBulletEnemy = NULL;
            }
        }

        obj = (struct Object *) obj->header.next;
    }
}

void td_patch_unallocs()
{
    // do not reference enemies from hp bars
    {
        uintptr_t *behaviorAddr = segmented_to_virtual(bhvTdHealthBar);
        struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
        struct Object *obj = (struct Object *) listHead->next;

        while (obj != (struct Object *) listHead) {
            if (obj->behavior == behaviorAddr && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) {
                if (obj->parentObj->activeFlags == 0)
                {
                    obj->activeFlags = 0;
                }
            }

            obj = (struct Object *) obj->header.next;
        }
    }

    // delete bullets referencing dead enemies
    clear_bullet_enemy_unallocs(bhvTdBullet);
    clear_bullet_enemy_unallocs(bhvTdFlame);
    clear_bullet_enemy_unallocs(bhvTdFlameLinger);
    
    // same thing for crystal tower
    {
        uintptr_t *behaviorAddr = segmented_to_virtual(bhvCrystalTower);
        struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)];
        struct Object *obj = (struct Object *) listHead->next;

        while (obj != (struct Object *) listHead) {
            if (obj->behavior == behaviorAddr && obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) {
                if (obj->parentObj && obj->parentObj->activeFlags == 0)
                {
                    obj->parentObj = NULL;
                }
            }

            obj = (struct Object *) obj->header.next;
        }
    }
}
