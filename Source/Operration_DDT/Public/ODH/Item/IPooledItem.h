#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPooledItem.generated.h"

UINTERFACE(MinimalAPI)
class UPooledItem : public UInterface
{
    GENERATED_BODY()
};

class OPERRATION_DDT_API IPooledItem
{
    GENERATED_BODY()

public:
    // 풀에서 활성화될 때 호출
    virtual void OnPooledActivated(const FVector& DesiredLocation, const FRotator& DesiredRotation) = 0;

    // 풀에서 비활성화될 때 호출
    virtual void OnPooledDeactivated() = 0;
};


