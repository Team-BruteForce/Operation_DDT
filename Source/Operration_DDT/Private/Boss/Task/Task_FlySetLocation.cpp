#include "Boss/Task/Task_FlySetLocation.h"

#include "Boss/Component/FlyingComponent.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus UTask_FlySetLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if(bStarted) return EStateTreeRunStatus::Running;
	bStarted=true;
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	FlyingComponent->StartSplineDistanceMaintenance(1500.0f);
	return EStateTreeRunStatus::Running;
}

void UTask_FlySetLocation::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
}