// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UIInterface.h"
#include "PickUp.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FPTPROJECT_API APickUp : public AActor, public IUIInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Item|Info")
		int ItemID{ 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Item|Info")
		float Value{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ADrone|Collision", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ADrone|StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Body;

public:	
	// Sets default values for this actor's properties
	APickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	int GetItemID_Implementation() const { return ItemID; };

	float GetItemValue_Implementation() const { return Value; };

};
