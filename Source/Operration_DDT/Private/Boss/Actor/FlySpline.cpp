/**
 * @file FlySpline.cpp
 * @brief 비행 스플라인 액터 구현 파일
 * 
 * @details
 * 보스의 비행 경로를 정의하는 스플라인 액터입니다.
 * 실린더 메시와 스플라인 컴포넌트를 통해 비행 궤도를 시각화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Actor/FlySpline.h"

#include "Components/SplineComponent.h"

// Sets default values
AFlySpline::AFlySpline()
{
	PrimaryActorTick.bCanEverTick = false;

	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderMesh"));
	SetRootComponent(CylinderMesh);

	// 기본 실린더 메시 할당(엔진 기본)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylMeshObj(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylMeshObj.Succeeded())
	{
		CylinderMesh->SetStaticMesh(CylMeshObj.Object);
	}

	// 4개의 수평 스플라인 생성
	CreateHorizontalSplines();
}

void AFlySpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildCylinderAndRims(Diameter,Height,NumRimPoints);
}

void AFlySpline::BuildCylinderAndRims(float InDiameter, float InHeight, int32 InNumPoints)
{
	// 파라미터 저장
	Diameter   = FMath::Max(0.1f,InDiameter);
	Height     = FMath::Max(0.1f,InHeight);
	NumRimPoints = FMath::Max(2,InNumPoints);

	// 언리얼 기본 실린더는 지름≈100,높이≈200(반지름 50,반높이 100) 기준이라,
	// 원하는 실제 치수(Diameter,Height)에 맞게 상대 스케일을 잡습니다.
	const float BaseDiameter = 100.f;
	const float BaseHeight   = 200.f;
	const FVector NewScale(Diameter / BaseDiameter,Diameter / BaseDiameter,Height / BaseHeight);
	CylinderMesh->SetRelativeScale3D(NewScale);

	// 4개의 수평 스플라인 재구성 (바텀 스플라인은 높이 0에 위치)
	for (int32 i = 0; i < HorizontalSplines.Num(); i++)
	{
		if (HorizontalSplines[i])
		{
			float ZPosition = 0.0f;
			
			// 스플라인별 Z 위치 설정
			switch (i)
			{
				case 0: // 바텀 스플라인 (높이 0)
					ZPosition = 0.0f;
					break;
				case 1: // 아래에서 1/3 지점
					ZPosition = Height * 0.25f;
					break;
				case 2: // 위에서 1/3 지점
					ZPosition = Height * 0.75f;
					break;
				case 3: // 탑 스플라인
					ZPosition = Height;
					break;
			}
			
			BuildRim(HorizontalSplines[i], ZPosition);
		}
	}
}

void AFlySpline::BuildRim(USplineComponent* Rim, float Z)
{
	if (!Rim) return;

	Rim->ClearSplinePoints(false);

	const float Radius = Diameter * 0.5f; // 로컬 좌표
	const int32 N = NumRimPoints;

	for (int32 i=0;i<N;++i)
	{
		const float T = (static_cast<float>(i)/N) * 2.f * PI;
		const float X = Radius * FMath::Cos(T);
		const float Y = Radius * FMath::Sin(T);
		const FVector Pos(X,Y,Z);

		// 둘레는 기본적으로 직선 세그먼트(Linear)로 충분, 필요시 Curve로 변경
		Rim->AddSplinePoint(Pos,ESplineCoordinateSpace::Local,ESplinePointType::Linear);
	}
	// 닫힌 루프
	Rim->SetClosedLoop(true);
	Rim->UpdateSpline();
}

void AFlySpline::CreateHorizontalSplines()
{
	// 기존 스플라인들 제거
	HorizontalSplines.Empty();
	
	// 4개의 수평 스플라인 생성
	for (int32 i = 0; i < 4; i++)
	{
		FString SplineName = FString::Printf(TEXT("HorizontalSpline_%d"), i);
		USplineComponent* NewSpline = CreateDefaultSubobject<USplineComponent>(*SplineName);
		NewSpline->SetupAttachment(RootComponent);
		NewSpline->bDrawDebug = true;
		
		HorizontalSplines.Add(NewSpline);
	}
}

USplineComponent* AFlySpline::GetSplineAtIndex(int32 Index) const
{
	if (HorizontalSplines.IsValidIndex(Index))
	{
		return HorizontalSplines[Index];
	}
	return nullptr;
}
