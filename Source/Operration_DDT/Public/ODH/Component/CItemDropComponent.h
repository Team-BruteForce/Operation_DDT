#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CItemDropComponent.generated.h"

/**
 * 에너미 사망 시 아이템 드랍을 담당하는 컴포넌트
 * - 사망 시 70% 확률로 드랍
 * - 드랍 시 70% 확률로 총알, 30% 확률로 체력 포션
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCItemDropComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCItemDropComponent();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleOwnerDeath();

    void TryDropItem();

    void SpawnItem(TSubclassOf<AActor> ItemClass);

	// 무작위 XY 오프셋 생성 (X,Y 각각 절대값이 [Min, Max] 범위, 부호 랜덤)
	FVector GenerateRandomXYOffset(float MinAbs, float MaxAbs) const;

	// 원형 반경 내 무작위 오프셋(폴라 좌표)
	FVector GenerateRandomXYOffsetCircular(float MinRadius, float MaxRadius, float AngleRadians = -1.0f) const;

public:
    // 외부에서 직접 아이템 드랍을 시도할 수 있는 public 함수
    UFUNCTION(BlueprintCallable, Category = "Item Drop")
    void ForceDropItem();

    // 특정 위치에 아이템을 스폰하는 함수
    UFUNCTION(BlueprintCallable, Category = "Item Drop")
    void SpawnItemAtLocation(TSubclassOf<AActor> ItemClass, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);

public:
    // 사망 시 드랍할 확률 (0~1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    float DropChance = 0.7f;

    // 드랍이 발생했을 때 총알이 나올 확률 (0~1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    float BulletWeight = 0.7f; // 포션은 0.3

    // 드랍할 아이템 클래스들 (액터)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    TSubclassOf<AActor> BulletItemActors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    TSubclassOf<AActor> HealthPotionItemActors;

    // 아이템 드로퍼 모드: true일 때 총알과 체력 포션을 모두 드랍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    bool IsItemDropper = false;

	// 드랍 반경 설정: X,Y 좌표의 절대값 범위 [Min, Max]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Radius")
	float DropRadiusMinXY = 170.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Radius")
	float DropRadiusMaxXY = 200.0f;

    // 두 아이템을 배치할 때 각도 분리(도 단위). 예: 20도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Radius")
    float PairAngleSeparationDegrees = 20.0f;
};


