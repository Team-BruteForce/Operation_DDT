#pragma once

#include "CoreMinimal.h"

namespace AssetPaths
{
    constexpr const TCHAR* ENEMY_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );
    constexpr const TCHAR* ENEMY_ANIM = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/LittleGirlEllie/BaseMesh/Separate/SK_body.SK_body'" );

#pragma region PlayerAsset
    constexpr const TCHAR* PLAYER_MESH = TEXT( "/Script/Engine.SkeletalMesh'/Game/MW/Assets/Character/Western_Pack/Characters/Billy/Mesh/SK_Billy.SK_Billy'");

    constexpr const TCHAR* PLAYER_ANIM = TEXT("/Script/Engine.AnimBlueprint'/Game/MW/Blueprints/ABP_DDTPlayer_Billy.ABP_DDTPlayer_Billy_C'");

    constexpr const TCHAR* SWORD_HOLSTER = TEXT("/Script/Engine.StaticMesh'/Game/MW/Assets/Weapon/Sword/scene/StaticMeshes/SM_Holster.SM_Holster'");

     constexpr const TCHAR* PlayerUI = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MW/UI/WBP/WBP_PlayerUI.WBP_PlayerUI_C'");
#pragma endregion PlayerAsset
    constexpr const TCHAR* IMC_Player = TEXT("/Script/EnhancedInput.InputMappingContext'/Game/MW/Input/IMC_Player.IMC_Player'");
    constexpr const TCHAR* IA_Move = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Move2D.IA_Move2D'");
    constexpr const TCHAR* IA_TurnVer = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_TurnVer.IA_TurnVer'");
    constexpr const TCHAR* IA_TurnHor = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_TurnHor.IA_TurnHor'");
    constexpr const TCHAR* IA_Sprint = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Sprint.IA_Sprint'");
    
    constexpr const TCHAR* IA_Reload = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Reload.IA_Reload'");
    constexpr const TCHAR* IA_Attack = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Attack.IA_Attack'");
    constexpr const TCHAR* IA_AimRifle = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_AimRifle.IA_AimRifle'");
    constexpr const TCHAR* IA_Heal = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Heal.IA_Heal'");
    constexpr const TCHAR* IA_Rifle = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Rifle.IA_Rifle'");
    constexpr const TCHAR* IA_Sword = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Sword.IA_Sword'");
    constexpr const TCHAR* IA_Revolver = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Revolver.IA_Revolver'");
    constexpr const TCHAR* IA_Roll = TEXT("/Script/EnhancedInput.InputAction'/Game/MW/Input/IA_Roll.IA_Roll'");
#pragma endregion
    

#pragma region BossAsset
    constexpr const TCHAR* Boss_MESH = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Asset/AngelOfDeath/Mesh/SK_AngelOfDeath.SK_AngelOfDeath'" );
    constexpr const TCHAR* Boss_Wings = TEXT ( "/Script/Engine.SkeletalMesh'/Game/LHW/Asset/AngelOfDeath/Mesh/SK_AngelOfDeath_Wings.SK_AngelOfDeath_Wings'" );
    constexpr const TCHAR* Boss_ANIM = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_Boss.ABP_Boss_C'" );
    constexpr const TCHAR* Boss_WingsAnim = TEXT ( "/Script/Engine.AnimBlueprint'/Game/LHW/Blueprint/ABP_BossWing.ABP_BossWing_C'");
    constexpr const TCHAR* Boss_AIC = TEXT ( "/Script/Engine.Blueprint'/Game/LHW/Blueprint/AIC_Boss.AIC_Boss_C'" );
    constexpr const TCHAR* Boss_StatsDataTable = TEXT ("/Script/Engine.DataTable'/Game/LHW/BossData/DT_BossStats.DT_BossStats'");
    constexpr const TCHAR* Boss_Projectile = TEXT ("/Script/Engine.Blueprint'/Game/LHW/Blueprint/Actor/BP_Projectile.BP_Projectile_C'");
    constexpr const TCHAR* Boss_Orb = TEXT ("/Script/Engine.Blueprint'/Game/LHW/Blueprint/Actor/BP_Orb.BP_Orb_C'");

    constexpr const TCHAR* Boss_Dissolve = TEXT ("/Script/Engine.Blueprint'/Game/LHW/Asset/AbsorbVFX/ExampleBlueprints/BP_Absorb_SK_7.BP_Absorb_SK_7_C'");
    constexpr const TCHAR* Boss_FlySpline = TEXT("/Script/Engine.Blueprint'/Game/LHW/Blueprint/Actor/BP_FlySpline.BP_FlySpline_C'");

#pragma endregion BossAsset
}