// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SpawnPoisonProjectile.generated.h"

UCLASS()
class OPERRATION_DDT_API UCAnimNotify_SpawnPoisonProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// 독성 프로젝타일 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile")
	TSubclassOf<class ACPoisonProjectile> PoisonProjectileClass;

	// 발사 위치 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile")
	FVector SpawnOffset = FVector(100.0f, 0.0f, 0.0f);

	// 발사 높이 (목표 위치에서 위로 올라갈 높이)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile")
	float LaunchHeight = 500.0f;

	// 발사 거리 (플레이어로부터의 거리)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile")
	float LaunchDistance = 300.0f;

private:
	// 플레이어 찾기
	class APawn* FindPlayer(USkeletalMeshComponent* MeshComp);

	// 독성 프로젝타일 스폰
	void SpawnPoisonProjectile(USkeletalMeshComponent* MeshComp, FVector SpawnLocation, FVector TargetLocation);
};
