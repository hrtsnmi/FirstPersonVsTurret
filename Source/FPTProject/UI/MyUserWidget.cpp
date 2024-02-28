// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "../Drone.h"
#include "../AI/Turret.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMyUserWidget::UpdateMagazineAmount(uint32 magazineAmount)
{
	TB_MagazineAmount->SetText(FText::FromString(FString::Printf(TEXT("%d"), magazineAmount)));
}

void UMyUserWidget::UpdateProggressBar(float currentHP, float MaxHP)
{
	PB_HP->SetPercent(currentHP / MaxHP);
}

void UMyUserWidget::SetUpPawnDelegates(APawn* pawn)
{
	if (ADrone* drone = Cast<ADrone>(pawn))
	{
		drone->OnUpdateHPDelegate.BindUObject(this, &UMyUserWidget::UpdateProggressBar);
		drone->OnUpdateMagazineAmountDelegate.BindUObject(this, &UMyUserWidget::UpdateMagazineAmount);
	}
	else if (ATurret* turret = Cast<ATurret>(pawn))
	{
		turret->OnUpdateHPDelegate.BindUObject(this, &UMyUserWidget::UpdateProggressBar);
		turret->OnUpdateMagazineAmountDelegate.BindUObject(this, &UMyUserWidget::UpdateMagazineAmount);
	}
}


void UMyUserWidget::RemovePawnBindingDelegates(APawn* pawn)
{
	if (ADrone* drone = Cast<ADrone>(pawn))
	{
		drone->OnUpdateHPDelegate.Unbind();
		drone->OnUpdateMagazineAmountDelegate.Unbind();
	}
	else if (ATurret* turret = Cast<ATurret>(pawn))
	{
		turret->OnUpdateHPDelegate.Unbind();
		turret->OnUpdateMagazineAmountDelegate.Unbind();
	}
}
