// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/UIInterface.h"
#include "FPTProjectProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class FPTPROJECT_API AFPTProjectProjectile : public AActor, public IUIInterface
{
	GENERATED_BODY()
	


private:	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

public:	
	// Sets default values for this actor's properties
	AFPTProjectProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION()
		float& AddOwnerSpeed();

private:
	int ProjectileID{2};
	float value{ -37.77f };

public:
	int GetItemID_Implementation() const { return ProjectileID; };
	float GetItemValue_Implementation() const { return value; };
};
