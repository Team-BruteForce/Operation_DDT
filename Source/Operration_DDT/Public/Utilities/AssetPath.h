#pragma once

#include "CoreMinimal.h"

namespace AssetPaths
{
    constexpr const TCHAR* ENEMY_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );
    constexpr const TCHAR* ENEMY_ANIM = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );
    
    

    // player
    constexpr const TCHAR* PLAYER_MESH = TEXT( "/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'");

    //boss
    constexpr const TCHAR* Boss_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Paladin_Anim_Set/Animation/Skeleton/SK_Mannequin.SK_Mannequin'" );
    constexpr const TCHAR* Boss_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_Boss.ABP_Boss_C'" );
    constexpr const TCHAR* Boss_AIC = TEXT ( "/Script/Engine.Blueprint'/Game/LHW/Blueprint/AIC_Boss.AIC_Boss_C'" );
    constexpr const TCHAR* Boss_STATE_TREE = TEXT ( "/Script/StateTreeModule.StateTree'/Game/LHW/StateTree/ST_Enemy.ST_Enemy_C'" );
    constexpr const TCHAR* Boss_WEAPON_MESH = TEXT ( "/Script/Engine.StaticMesh'/Game/LHW/Paladin_Anim_Set/sword_of_miracle_free/StaticMeshes/sword_of_miracle_free.sword_of_miracle_free'");
}