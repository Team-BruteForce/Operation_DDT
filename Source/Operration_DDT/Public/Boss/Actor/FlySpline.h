/**
 * @file FlySpline.h
 * @brief 비행 스플라인 액터 헤더 파일
 * 
 * @details
 * 보스의 비행 경로를 정의하는 스플라인 액터입니다.
 * 실린더 메시와 스플라인 컴포넌트를 통해 비행 궤도를 시각화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlySpline.generated.h"

/**
 * @brief 비행 스플라인 액터
 * 
 * 보스의 비행 경로를 정의하는 스플라인 액터입니다.
 * 실린더 메시와 스플라인 컴포넌트를 통해 비행 궤도를 시각화합니다.
 */
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
