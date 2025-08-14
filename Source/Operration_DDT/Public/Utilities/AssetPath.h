#pragma once

#include "CoreMinimal.h"

namespace AssetPaths
{
    constexpr const TCHAR* ENEMY_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );
    constexpr const TCHAR* ENEMY_ANIM = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );

#pragma region PlayerAsset
    constexpr const TCHAR* PLAYER_MESH = TEXT( "/Script/Engine.SkeletalMesh'/Game/ThirdPerson/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'");

    constexpr const TCHAR* PLAYER_ANIM = TEXT("/Script/Engine.AnimBlueprint'/Game/MW/Blueprints/ABP_DDTPlayer.ABP_DDTPlayer_C'");

    constexpr const TCHAR* SWORD_HOLSTER = TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Weapon/Sword/scene/StaticMeshes/SM_Holster.SM_Holster'");
#pragma endregion PlayerAsset
    


    constexpr const TCHAR* Boss_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Paladin_Anim_Set/Animation/Skeleton/SK_Mannequin.SK_Mannequin'" );
    constexpr const TCHAR* Boss_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_Boss.ABP_Boss_C'" );
    constexpr const TCHAR* Boss_AIC = TEXT ( "/Script/Engine.Blueprint'/Game/LHW/Blueprint/AIC_Boss.AIC_Boss_C'" );
    constexpr const TCHAR* Boss_STATE_TREE = TEXT ( "/Script/StateTreeModule.StateTree'/Game/LHW/StateTree/ST_Enemy.ST_Enemy_C'" );
}