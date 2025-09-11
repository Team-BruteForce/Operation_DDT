// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle,
	Sword,
	Revolver,
	Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* DataAssets[(int32)EWeaponType::Max];

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsRifleMode() { return Type == EWeaponType::Rifle; }
	FORCEINLINE bool IsSwordMode() { return Type == EWeaponType::Sword; }
	FORCEINLINE bool IsRevolverMode() { return Type == EWeaponType::Revolver; }

private:
	bool IsIdleMode();
	// StateComp, WeaponComp 둘 다 같은 레벨
	// 서로 소유할 수 없는 관계 -> 참조만해서 리턴 받기 위함

public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();

public: // 무기 세팅
	void SetUnarmedMode();
	void SetRifleMode();
	void SetSwordMode();
	void SetRevolverMode();

	void DoAction();
private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	class ACharacter* OwnerCharacter;
	EWeaponType Type = EWeaponType::Rifle;


		
};
