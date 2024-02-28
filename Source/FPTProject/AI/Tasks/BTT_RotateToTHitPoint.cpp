// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RotateToTHitPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../TurretAIController.h"
#include "../Turret.h"

UBTT_RotateToTHitPoint::UBTT_RotateToTHitPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "RotateToTarget";

	EnemyKey.AddObjectFilter(this, "Target", AActor::StaticClass());
}

EBTNodeResult::Type UBTT_RotateToTHitPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type TaskResult = EBTNodeResult::Type::Failed;

	ATurretAIController* AIControll = Cast<ATurretAIController>(OwnerComp.GetOwner());
	if (AIControll && AIControll->GetPawn())
	{
		ATurret* Executer = Cast<ATurret>(AIControll->GetPawn());

		if (Executer)
		{
			AActor* Target = Cast<AActor>(AIControll->GetBlackboardComponent()->GetValueAsObject(EnemyKey.SelectedKeyName));

			if (Target)
			{
				FVector HitPoint = FVector::ZeroVector;
				FRotator RotateToTarget = Executer->GetActorRotation();

				//Find point to hit
				Executer->PrepareFutureHitPoint(Target, HitPoint);

				bool FindHitPoint = (HitPoint != FVector::ZeroVector);
				AIControll->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("CanAttack")), FindHitPoint);

				if (FindHitPoint)
				{
					//Rotate To Hit Point or Rotate To Target
					RotateToTarget.Yaw = FRotationMatrix::MakeFromX(HitPoint - Executer->GetActorLocation()).Rotator().Yaw;
					Executer->SetActorRotation(RotateToTarget);

					TaskResult = EBTNodeResult::Type::Succeeded;
				}
			}
		}
	}
	//EBTNodeResult::Type::Failed;
	//EBTNodeResult::Type::Succeeded;
	return TaskResult;
}
