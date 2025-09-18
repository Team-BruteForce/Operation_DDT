#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AllResettable.generated.h"

UINTERFACE(Blueprintable)
class UAllResettable : public UInterface
{
	GENERATED_BODY()
};

class IAllResettable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Encounter")
	void AllReset();
};


