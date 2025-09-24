#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "CEnemyAIController.generated.h"

/**
 * 재사용 가능한 Enemy AI Controller 베이스 클래스
 * AAIController는 이미 IGenericTeamAgentInterface를 상속하므로 중복 상속 제거
 * AIPerception을 사용하여 적/아군 구분
 * 모든 적 AI 컨트롤러의 베이스 클래스로 사용
 */
UCLASS()
class OPERRATION_DDT_API ACEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	// IGenericTeamAgentInterface 구현 (AAIController에서 오버라이드)
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

	// AIPerception 이벤트 핸들러
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 적 감지 시 호출되는 함수 (서브클래스에서 오버라이드 가능)
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnEnemyDetected(AActor* EnemyActor);

	// 아군 감지 시 호출되는 함수 (서브클래스에서 오버라이드 가능)
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnAllyDetected(AActor* AllyActor);

	// 감지된 적 목록 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	TArray<AActor*> GetDetectedEnemies() const { return DetectedEnemies; }

	// 감지된 아군 목록 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	TArray<AActor*> GetDetectedAllies() const { return DetectedAllies; }

	// 가장 가까운 적 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	AActor* GetNearestEnemy() const;

	// 가장 가까운 아군 가져오기
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	AActor* GetNearestAlly() const;

	// 특정 액터가 적인지 확인
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	bool IsEnemy(AActor* Actor) const;

	// 특정 액터가 아군인지 확인
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AI")
	bool IsAlly(AActor* Actor) const;

    // 플레이어 현재 사망 상태 플래그를 활성화/해제
    UFUNCTION(BlueprintCallable, Category = "AI|PlayerState")
    void SetNowPlayerDead();

    UFUNCTION(BlueprintCallable, Category = "AI|PlayerState")
    void ClearNowPlayerDead();

protected:
	// AIPerception 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// 시각 감지 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	// 팀 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	FGenericTeamId TeamID;

	// 감지 범위 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float LoseSightRadius = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PeripheralVisionAngleDegrees = 90.0f;

private:
	// 감지된 적 목록
	UPROPERTY()
	TArray<AActor*> DetectedEnemies;

	// 감지된 아군 목록
	UPROPERTY()
	TArray<AActor*> DetectedAllies;

	// 타겟 포기 타이머
	UPROPERTY()
	FTimerHandle TargetLostTimerHandle;

	// 목록 관리 함수들
	void AddToEnemyList(AActor* Actor);
	void RemoveFromEnemyList(AActor* Actor);
	void AddToAllyList(AActor* Actor);
	void RemoveFromAllyList(AActor* Actor);

	// 타겟 포기 함수
	UFUNCTION()
	void OnTargetLost();

    // 플레이어가 방금 죽은 상태인지 여부 (true면 감지 무시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|PlayerState", meta=(AllowPrivateAccess="true"))
    bool bIsNowPlayerDead = false;
};
