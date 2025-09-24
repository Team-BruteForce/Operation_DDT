#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AllEnemyRestart.generated.h"

UINTERFACE(Blueprintable)
class UAllEnemyRestart : public UInterface
{
	GENERATED_BODY()
};

class IAllEnemyRestart
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Enemy")
	void EnemyRestart();
};

