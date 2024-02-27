// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"


class UProgressBar;
class UTextBlock;
class ADrone;
/**
 * 
 */
UCLASS()
class FPTPROJECT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,
		meta = (BindWidget), Category = "UI|HP Progress")
		UProgressBar* PB_HP;

	UPROPERTY(EditAnywhere,
		meta = (BindWidget), Category = "UI|Magazine Amount")
		UTextBlock* TB_MagazineAmount;


public:
	UFUNCTION()
		void UpdateMagazineAmount(uint32 magazineAmount);

	UFUNCTION()
		void UpdateProggressBar(float currentHP, float MaxHP);

	void SetUpPawnDelegates(APawn* pawn);
	void RemovePawnBindingDelegates(APawn* pawn);
};
