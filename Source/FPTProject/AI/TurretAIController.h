// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;
class UMyUserWidget;

UCLASS()
class FPTPROJECT_API ATurretAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public:
	ATurretAIController();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		UMyUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMyUserWidget> TurretHUD;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		float PatrolRadius{ 1000.f };

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Data")
		FName SearchTag {
		FName("Target")
	};

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Data", meta = (AllowPrivateAccess = true))
		void PerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
