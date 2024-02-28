// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class FPTPROJECT_API UBTS_RotateToTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
private:
	UBTS_RotateToTarget(const FObjectInitializer& ObjectInitializer);


	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
