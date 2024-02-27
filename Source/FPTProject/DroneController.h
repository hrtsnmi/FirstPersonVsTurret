// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DroneController.generated.h"

class UMyUserWidget;
/**
 * 
 */



UCLASS()
class FPTPROJECT_API ADroneController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMyUserWidget> DronHUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		UMyUserWidget* CurrentUI;
protected:
	
	virtual void BeginPlay() override;

};
