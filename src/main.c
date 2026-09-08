#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"


#define GLOBAL_PROPERTIES_PAUSED 2
#define ISLES_EXIT 0

extern u32 global_properties_bitfield;
extern OSContPad D_global_asm_807ECD58;
extern u16 D_global_asm_80744734[];
extern void func_global_asm_805FF378(Maps nextMap, s32 nextExit); // initiateTransition
extern void func_global_asm_805FFFC8(void);                        // resetMap
extern void func_global_asm_807124B8(Maps newMap, s32 cutsceneIndex, u8 newGameMode);
extern void setFlag(s16 flagIndex, u8 newValue, u8 flagType);

static s32 exitToIslesRequested(void) {
    if (!(global_properties_bitfield & GLOBAL_PROPERTIES_PAUSED)) {
        return 0;
    }
    return (D_global_asm_807ECD58.button & L_TRIG) != 0;
}

RECOMP_PATCH void func_global_asm_80600044(s32 levelIndex) {
    func_global_asm_805FFFC8();
    if (exitToIslesRequested()) {
        func_global_asm_805FF378(MAP_DK_ISLES_OVERWORLD, ISLES_EXIT);
    } else {
        func_global_asm_805FF378((Maps)D_global_asm_80744734[levelIndex], 1);
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
