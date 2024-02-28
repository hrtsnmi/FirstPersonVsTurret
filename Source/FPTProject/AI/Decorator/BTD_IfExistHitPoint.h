// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IfExistHitPoint.generated.h"

/**
 * 
 */
UCLASS()
class FPTPROJECT_API UBTD_IfExistHitPoint : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
private:
	UBTD_IfExistHitPoint(const FObjectInitializer& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
