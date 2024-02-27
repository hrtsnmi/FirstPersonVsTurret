// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"
#include "UI/MyUserWidget.h"

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if(DronHUD)
	{
		CurrentUI = CreateWidget<UMyUserWidget>(this, DronHUD);
		CurrentUI->SetUpPawnDelegates(GetPawn());

		//Add Widget to ViewPort:
		CurrentUI->AddToViewport();
	}
	
}
