// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineMeshComponent.h"
#include "UIDelegates.h"
#include "Interfaces/FireInterface.h"

#include "Drone.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddSplineMeshAtIndexSignature, int32, index);


class UInputComponent;
class UCameraComponent;
class UBoxComponent;
class UFloatingPawnMovement;
class AFPTProjectProjectile;
class UStaticMeshComponent;
class USplineComponent;

void ClearSpline(TArray<USplineMeshComponent*>& SplineMeshes, USplineComponent* Spline);


UCLASS()
class FPTPROJECT_API ADrone : public APawn, public IFireInterface
{
	GENERATED_BODY()

private:
	void DrawProjectilePath();
	void RemoveProjectilePath();
	AFPTProjectProjectile* SpawnProjectile(float speed);

public:
	// Sets default values for this pawn's properties
	ADrone();

public:
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ADrone|Collision", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ADrone|StaticMeshComponent", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Body;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UFloatingPawnMovement* FloatingPawnMovement;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Move Vertical Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveVerticalAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* FireAction;

private:
	/** Projectile */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AFPTProjectProjectile> fptProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ProjectileSpawnLocation;

	UPROPERTY() FTransform ThownTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true), meta = (AllowPrivateAccess = "true"), Category = "Projectile|Speed")
		float StartSpeed{ 1000.f };

	UPROPERTY(BlueprintAssignable)
		FAddSplineMeshAtIndexSignature OnAddSplineMeshAtIndex;

	UPROPERTY(VisibleAnywhere, Category = "Projectile|Info")
		FPredictProjectilePathParams PredictParams;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile|Info", meta = (AllowPrivateAccess = "true"))
		TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Info", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* EndSpline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile|Info", meta = (AllowPrivateAccess = "true"))
		USplineComponent* ProjectileSpline;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void MoveVertical(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for fire input */
	void OnFireCommandCalled(const FInputActionValue& Value);

	UFUNCTION()
		void BoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	static constexpr float MaxHP{ 100.f };

	float CurrentHP{ 51.86f };

	uint32 CurrentMagazine{ 20 };


public:
	OnUpdateHPSignature OnUpdateHPDelegate;
	OnUpdateMagazineAmountSignature OnUpdateMagazineAmountDelegate;


	float GetTargetHalfTall_Implementation();
	

	void Fire_Implementation();
	void Dead_Implementation();
};
