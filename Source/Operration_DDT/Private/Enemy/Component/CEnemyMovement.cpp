#include "Enemy/Component/CEnemyMovement.h"
#include "Enemy/AEnemy.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Enemy/CEnemyController.h"

UCEnemyMovement::UCEnemyMovement()
{
	
}
/**
 * @brief 게임 시작 시 순찰 경로 정보를 읽어와 초기화합니다.
 * @details 외부에서 지정된 PatrolPath 액터로부터 스플라인 데이터를 가져와 전체 순찰 지점 개수를 설정합니다.
 */
void UCEnemyMovement::BeginPlay()
{
	Super::BeginPlay();


}

/**
 * @brief 현재 순찰 지점(CurrentPatrolPoints)으로의 이동을 AI 컨트롤러에 명령합니다.
 */
void UCEnemyMovement::Patrol()
{
	Enemy=Cast<AAEnemy>(GetOwner());
	EnemyController=Cast<ACEnemyController>(Enemy->GetController());
	FVector TargetLocation = Enemy->GetPatrolPath()->SplineComponent->GetLocationAtSplinePoint(EnemyController->CurrentPatrolPoints, ESplineCoordinateSpace::World);
	Owner->MoveToLocation(TargetLocation);
}

/**
 * @brief 다음 순찰 지점으로 인덱스를 업데이트하고, 해당 지점으로의 순찰을 다시 시작합니다.
 * @details 순찰 지점이 마지막을 넘어서면 처음으로 되돌아가는 순환(loop) 로직을 포함합니다.
 */
void UCEnemyMovement::IncreasePatrolPoint()
{
	EnemyController->CurrentPatrolPoints++;
	if (EnemyController->CurrentPatrolPoints >= EnemyController->TotalPatrolPoints)
	{
		EnemyController->CurrentPatrolPoints = 0;
	}

	Patrol();
}