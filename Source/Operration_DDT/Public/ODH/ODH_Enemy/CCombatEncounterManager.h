#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH/ODH_Enemy/Interface/AllEnemyRestart.h"
#include "CCombatEncounterManager.generated.h"

class UBoxComponent;

UCLASS(Blueprintable)
class OPERRATION_DDT_API ACCombatEncounterManager : public AActor
{
	GENERATED_BODY()

public:
	ACCombatEncounterManager();

protected:
	virtual void BeginPlay() override;

public:
	// 플레이어 감지 트리거
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter|Trigger")
	UBoxComponent* PlayerTrigger;

	// 배치 처리 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Performance")
	int32 BatchSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter|Performance")
	float BatchIntervalSeconds = 0.0f;

	// 수동 호출용
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void ResetCombatInZone(AActor* InstigatorActor);

	// 모든 에너미들에게 EnemyRestart 인터페이스 호출
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void AllEnemyRestart();

	// BP 디버그용 수집
	UFUNCTION(BlueprintCallable, Category = "Encounter")
	void CollectActiveEnemiesInZone_BP(TArray<APawn*>& OutEnemies) const;

    // 등록/해제 API
    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void RegisterEnemy(APawn* Enemy);

    UFUNCTION(BlueprintCallable, Category = "Encounter")
    void UnregisterEnemy(APawn* Enemy);

private:
    void CollectActiveEnemies(TArray<TWeakObjectPtr<APawn>>& OutEnemies) const;
	void ProcessResetBatch(const TArray<TWeakObjectPtr<APawn>>& Batch, AActor* InstigatorActor) const;
	void ResetOneEnemy(APawn* Pawn, AActor* InstigatorActor) const;
	bool TryGetHomeLocationFromPatrolArray(const AActor* Enemy, FVector& OutLocation) const;

	UFUNCTION()
	void OnPlayerTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    UPROPERTY()
    TSet<TWeakObjectPtr<APawn>> RegisteredEnemies;
};


