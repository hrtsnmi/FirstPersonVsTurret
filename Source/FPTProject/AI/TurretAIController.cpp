// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"

void ATurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
	}
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::PerceptionUpdated);
}

ATurretAIController::ATurretAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(
		TEXT("AIPerception")
	);

}

void ATurretAIController::PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	bool ActorHasTag = Actor->ActorHasTag(SearchTag) && Stimulus.WasSuccessfullySensed();
	if (ActorHasTag)
	{
		//GetWorldTimerManager().ClearTimer(EnemyTimer);

		UBlackboardComponent* BB = GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsObject(SearchTag, Actor);
			/*BB->SetValueAsBool(HasLineOfSight, true);
			BB->SetValueAsObject(EnemyActor, Actor);
			BB->SetValueAsBool(LostRecently, false);*/
		}
	}
	else
	{
		//GetWorldTimerManager().SetTimer(EnemyTimer, FTimerDelegate::CreateLambda([this, Actor]
			//{
				UBlackboardComponent* BB = GetBlackboardComponent();
				if (BB)
				{
					BB->ClearValue(SearchTag);
					/*BB->SetValueAsBool(HasLineOfSight, false);
					BB->SetValueAsObject(EnemyActor, nullptr);
					BB->SetValueAsBool(LostRecently, true);*/
				}
			//}),
			//5, false);
	}
}
