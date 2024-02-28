// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RotateToTarget.h"
#include "../TurretAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Turret.h"

UBTS_RotateToTarget::UBTS_RotateToTarget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "RotateToTarget";
	BlackboardKey.AddObjectFilter(this, FName(TEXT("Target")), AActor::StaticClass());
}

void UBTS_RotateToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	ATurretAIController* AIControll = Cast<ATurretAIController>(OwnerComp.GetOwner());
	if (AIControll && AIControll->GetPawn())
	{
		ATurret* Executer = Cast<ATurret>(AIControll->GetPawn());

		if (Executer)
		{
			AActor* Target =
				Cast<AActor>(AIControll->GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
			if (Target)
			{
				//Rotate To Hit Point or Rotate To Target
				if (!AIControll->GetBlackboardComponent()->GetValueAsBool(FName(TEXT("CanAttack"))))
				{
					FRotator RotateToTarget = Executer->GetActorRotation();
					const FVector& EndPoint = Target->GetActorLocation();
					RotateToTarget.Yaw = FRotationMatrix::MakeFromX(EndPoint - Executer->GetActorLocation()).Rotator().Yaw;
					Executer->SetActorRotation(RotateToTarget);


					Executer->SetActorRelativeRotation(
						FMath::RInterpTo(
							Executer->GetActorRotation(),
							RotateToTarget,
							DeltaSeconds,
							1.f));
				}
			}

		}
	}
}
