// Enemy Health Bar Widget

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyHealthBarWidget.generated.h"

UCLASS()
class OPERRATION_DDT_API UCEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Health")
    void SetHealthPercent(float InPercent, float InFrontDecreaseSpeed = 2.5f, float InGhostDecreaseSpeed = 1.2f);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	class UProgressBar* HealthProgressBar;

    // 잔상(고스트) 체력 표시용 ProgressBar (같은 위치에 겹쳐 배치)
    UPROPERTY(meta = (BindWidgetOptional))
    class UProgressBar* HealthGhostBar;

private:
	// 현재 표시 중인 값과 목표 값
    float DisplayPercent = 1.0f; // FrontBar 표시값
    float TargetPercent = 1.0f;  // 목표 체력 퍼센트

    float GhostPercent = 1.0f;   // GhostBar 표시값

    // 초당 변화 속도 (퍼센트 보간 속도). 컴포넌트에서 전달받아 사용
    float FrontDecreaseSpeed = 2.5f;
    float GhostDecreaseSpeed = 1.2f;

	// true면 감소만 부드럽게, 증가(회복)는 즉시 반영
	UPROPERTY(EditAnywhere, Category = "Health")
    bool bAnimateOnlyOnDecrease = true;
};


