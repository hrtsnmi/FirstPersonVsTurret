#pragma once
#include "Interfaces/FireInterface.h"


//ui
DECLARE_DELEGATE_TwoParams(OnUpdateHPSignature, float, float);
DECLARE_DELEGATE_OneParam(OnUpdateMagazineAmountSignature, uint32);


void AddMagasine(int value, uint32& CurrentMagazine,
	AActor* Shooter, OnUpdateMagazineAmountSignature& OnUpdateMagazineAmountDelegate)
{
	if (!Shooter->GetClass()->ImplementsInterface(UFireInterface::StaticClass()))
	{
		return;
	}

	int tmp = (int)CurrentMagazine + value;

	if (tmp < 0)
	{
		return;
	}

	if (value < 0.f)
	{
		IFireInterface::Execute_Fire(Shooter);
		//SpawnProjectile(StartSpeed);
	}
	CurrentMagazine = (uint32)tmp;


	if (OnUpdateMagazineAmountDelegate.IsBound())
	{
		OnUpdateMagazineAmountDelegate.Execute(CurrentMagazine);
	}
}

void AddHP(float value, float &CurrentHP, float MaxHP,
	AActor* Shooter, OnUpdateHPSignature& OnUpdateHPDelegate)
{
	if (!Shooter->GetClass()->ImplementsInterface(UFireInterface::StaticClass()))
	{
		return;
	}

	float tmp = CurrentHP + value;

	if (tmp <= 0.f)
	{
		IFireInterface::Execute_Dead(Shooter);
	}

	CurrentHP = (tmp <= MaxHP) ? (tmp) : (MaxHP);

	if (OnUpdateHPDelegate.IsBound())
	{
		OnUpdateHPDelegate.Execute(CurrentHP, MaxHP);
	}
}