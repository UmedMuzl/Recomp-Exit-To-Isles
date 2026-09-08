#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"

#define GLOBAL_PROPERTIES_PAUSED 2
#define ISLES_EXIT 0
#define LEVEL_INDEX_ISLES 7

extern u32 global_properties_bitfield;
extern OSContPad D_global_asm_807ECD58; // controller hold state
extern Maps current_map;
extern u8 game_mode;
extern u8 game_mode_copy;
extern s32 D_global_asm_807FBB68;
extern s8 D_global_asm_80755330;   // prevent_transition
extern u8 D_global_asm_807445E0[]; // level index per map

extern void func_global_asm_805FF378(Maps nextMap, s32 nextExit); // initiateTransition
extern void func_global_asm_805FFFC8(void);                        // resetMap
extern s32 func_global_asm_805FEF74(Maps map);                     // isLobby
extern void func_global_asm_807124B8(Maps newMap, s32 cutsceneIndex, u8 newGameMode);
extern u8 isFlagSet(s16 flagIndex, u8 flagType);
extern void setFlag(s16 flagIndex, u8 newValue, u8 flagType);

static s32 isIslesMap(Maps map) {
    switch (map) {
        case MAP_DK_ISLES_OVERWORLD:
        case MAP_FAIRY_ISLAND:
        case MAP_KLUMSY:
        case MAP_DK_ISLES_SNIDES_ROOM:
        case MAP_TRAINING_GROUNDS:
        case MAP_DK_HOUSE:
            return 1;
        default:
            return 0;
    }
}

// WTI rules
static s32 exitToIslesAllowed(void) {
    // Not before the escape from Training Grounds
    if (!isFlagSet(PERMFLAG_CUTSCENE_ISLES_FTCS, FLAG_TYPE_PERMANENT)) {
        return 0;
    }
    if (func_global_asm_805FEF74(current_map)) {
        return 1; // level lobbies
    }
    if (D_global_asm_807445E0[current_map] < LEVEL_INDEX_ISLES) {
        return 1; // main maps, sub-maps, bosses, races
    }
    if (current_map == MAP_HELM) {
        // Hideout Helm only once the Blast-o-Matic is off.
        return isFlagSet(PERMFLAG_PROGRESS_HELM_SHUTDOWN, FLAG_TYPE_PERMANENT) != 0;
    }
    return isIslesMap(current_map);
}

static s32 exitToIslesRequested(void) {
    if (!(global_properties_bitfield & GLOBAL_PROPERTIES_PAUSED)) {
        return 0;
    }
    if (!(D_global_asm_807ECD58.button & L_TRIG)) {
        return 0;
    }
    return exitToIslesAllowed();
}

RECOMP_PATCH void func_global_asm_80712F10(Maps *map, s32 *exit) {
    if (*map != current_map && exitToIslesRequested()) {
        func_global_asm_805FFFC8();
        *map = MAP_DK_ISLES_OVERWORLD;
        *exit = ISLES_EXIT;
    }
    if ((game_mode_copy == GAME_MODE_MYSTERY_MENU_MINIGAME)
        && (((D_global_asm_807FBB68 & 2) == 0)
            || (current_map == MAP_KROOL_FIGHT_CHUNKY_PHASE)
            || (*map == MAP_DK_ISLES_OVERWORLD))) {
        *map = MAP_MAIN_MENU;
        *exit = 0;
        game_mode = GAME_MODE_MAIN_MENU;
        return;
    }
    if ((game_mode_copy == GAME_MODE_SNIDES_BONUS_GAME) && (current_map != *map)) {
        *map = MAP_SNIDES_HQ;
        *exit = 0;
        game_mode = GAME_MODE_ADVENTURE;
        return;
    }
    if (D_global_asm_80755330 != 0) {
        *map = -1;
    }
}

RECOMP_PATCH void func_global_asm_807127B4(void) {
    func_global_asm_805FFFC8();
    if (exitToIslesRequested()) {
        func_global_asm_805FF378(MAP_DK_ISLES_OVERWORLD, ISLES_EXIT);
        return;
    }
    setFlag(PERMFLAG_PROGRESS_HAS_QUIT_GAME, TRUE, FLAG_TYPE_PERMANENT);
    func_global_asm_807124B8(MAP_HELM_LEVEL_INTROS_GAME_OVER, 0x17, GAME_MODE_QUIT_GAME);
}
