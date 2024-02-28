// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurretAttack.h"
#include "../TurretAIController.h"
#include "../Turret.h"

EBTNodeResult::Type UBTT_TurretAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	ATurretAIController* AIControll = Cast<ATurretAIController>(OwnerComp.GetOwner());
	if (AIControll && AIControll->GetPawn())
	{
		ATurret* Executer = Cast<ATurret>(AIControll->GetPawn());

		if (Executer)
		{
			
			Executer->DoNamberOfShots(1);
			TaskResult = EBTNodeResult::Type::Succeeded;

		}
	}
	//EBTNodeResult::Type::Failed;
	//EBTNodeResult::Type::Succeeded;
	return TaskResult;
}
