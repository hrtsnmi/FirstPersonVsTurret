// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_RotateToTHitPoint.generated.h"

/**
 * 
 */
UCLASS()
class FPTPROJECT_API UBTT_RotateToTHitPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UBTT_RotateToTHitPoint(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditInstanceOnly)
		FBlackboardKeySelector EnemyKey;
};
