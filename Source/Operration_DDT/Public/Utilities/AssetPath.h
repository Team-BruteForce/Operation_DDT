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
    

#pragma region BossAsset
    constexpr const TCHAR* Boss_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Asset/AngelOfDeath/Mesh/SK_AngelOfDeath.SK_AngelOfDeath'" );
    constexpr const TCHAR* Boss_Wings = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Asset/AngelOfDeath/Mesh/SK_AngelOfDeath_Wings.SK_AngelOfDeath_Wings'" );
    constexpr const TCHAR* Boss_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_Boss.ABP_Boss_C'" );
    constexpr const TCHAR* Boss_WingsAnim = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_BossWing.ABP_BossWing_C'");
    constexpr const TCHAR* Boss_AIC = TEXT ( "/Script/Engine.Blueprint'/Game/LHW/Blueprint/AIC_Boss.AIC_Boss_C'" );
    constexpr const TCHAR* Boss_StateTree = TEXT ("/Script/StateTreeModule.StateTree'/Game/LHW/StateTree/STT_Boss.STT_Boss_C'");
    constexpr const TCHAR* Boss_StatsDataTable = TEXT ("/Script/Engine.DataTable'/Game/LHW/BossData/DT_BossStats.DT_BossStats'");
    constexpr const TCHAR* Boss_Projectile = TEXT ("/Script/Engine.Blueprint'/Game/LHW/Blueprint/BP_Projectile.BP_Projectile_C'");
#pragma endregion BossAsset
}