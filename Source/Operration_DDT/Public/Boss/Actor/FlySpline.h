// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlySpline.generated.h"

UCLASS()
class OPERRATION_DDT_API AFlySpline : public AActor
{
	GENERATED_BODY()
	
public:
	AFlySpline();

	// 원기둥 메시(기본: Engine/BasicShapes/Cylinder)
	UPROPERTY(VisibleAnywhere,Category="Cylinder")
	class UStaticMeshComponent* CylinderMesh;

	// 4개의 수평 스플라인 배열 (동일 비율로 나눔)
	UPROPERTY(VisibleAnywhere,Category="Spline")
	TArray<class USplineComponent*> HorizontalSplines;

	// 파라미터
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Params",meta=(ClampMin="2"))
	int32 NumRimPoints = 32;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Params",meta=(ClampMin="0.1"))
	float Diameter = 600.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Params",meta=(ClampMin="0.1"))
	float Height = 400.f;

	// 에디터에서 값 바꾸면 즉시 재구성
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable,Category="Build")
	void BuildCylinderAndRims(float InDiameter,float InHeight,int32 InNumPoints);

	// 스플라인 배열 접근 함수들
	UFUNCTION(BlueprintPure, Category="Spline")
	TArray<USplineComponent*> GetHorizontalSplines() const { return HorizontalSplines; }
	
	UFUNCTION(BlueprintPure, Category="Spline")
	USplineComponent* GetSplineAtIndex(int32 Index) const;

private:
	void BuildRim(USplineComponent* Rim,float Z);
	void CreateHorizontalSplines();

};
