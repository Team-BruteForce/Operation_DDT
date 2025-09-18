// Component to display enemy health bar in world

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyHealthBarComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCEnemyHealthBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyHealthBarComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = UI)
	void ShowHealthBar();

	UFUNCTION(BlueprintCallable, Category = UI)
	void HideHealthBar();

private:
	UPROPERTY()
	class UCEnemyStatusComponent* StatusComponent;

	UPROPERTY()
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<class UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FVector RelativeOffset = FVector(0, 0, 120);

	UPROPERTY(EditAnywhere, Category = UI)
	FVector2D DrawSize = FVector2D(120.0f, 16.0f);

private:
	UFUNCTION()
	void HandleHealthChanged(float NewHealth);

	UFUNCTION()
	void HandleMaxHealthChanged(float NewMaxHealth);

	void UpdateWidgetPercent();
};


