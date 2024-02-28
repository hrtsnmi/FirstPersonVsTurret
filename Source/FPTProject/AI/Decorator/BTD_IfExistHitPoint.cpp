// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_IfExistHitPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../TurretAIController.h"

UBTD_IfExistHitPoint::UBTD_IfExistHitPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "PointToShootWasFound";

	BlackboardKey.AddBoolFilter(this, FName(TEXT("CanAttack")));
}

bool UBTD_IfExistHitPoint::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ATurretAIController* AIControll = Cast<ATurretAIController>(OwnerComp.GetOwner());

	return (AIControll) ?
		(AIControll->GetBlackboardComponent()->GetValueAsBool(FName(TEXT("CanAttack")))) : (false);
}
