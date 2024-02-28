// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "../UIDelegates.h"
#include "../Interfaces/FireInterface.h"

#include "Turret.generated.h"


class AFPTProjectProjectile;
class UWidgetComponent;
class UMyUserWidget;
/**
 * 
 */
UCLASS()
class FPTPROJECT_API ATurret : public ACharacter, public IFireInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//for Debug Draw
	UFUNCTION()
		void DrawDebugElements() const;

	void SetNewAnimPitch(bool ToTarget = false);

	void SetTergetTall(AActor* Target);
	//lines
	UPROPERTY()
		TArray<FVector>LinesStartEnds;
	//spheres
	UPROPERTY()
		FVector PointToShootCenter;

	float NotRangeRadius1{};
	float NotRangeRadius2{};
	float RangeRadius{};

private:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Combat")
		TSubclassOf<AFPTProjectProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WorldWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		UMyUserWidget* CurrentWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UMyUserWidget> TurretHUD;

private:
	static constexpr float MaxHP{ 100.f };

	float CurrentHP{ MaxHP };

	uint32 CurrentMagazine{ 20 };
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Speed", meta = (AllowPrivateAccess = "true"))
		float ProjectileSpeed{ 3900.f };

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params", meta = (AllowPrivateAccess = "true"))
		float ProjectileScale{ 0.3f };

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Params", meta = (AllowPrivateAccess = "true"))
		float ProjectileCapsuleRadius{ 55.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimPitch", meta = (AllowPrivateAccess = "true"))
		float AnimPitch{ };

	UPROPERTY(VisibleAnywhere, Category = "Target", meta = (AllowPrivateAccess = "true"))
		float TargetHalfTall;

	constexpr static float boardAngle{ 45.f };
	constexpr static float boardAngleRad{ boardAngle * PI / 180.f };

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly) float searchRadius{ 800.0f };

	void DoNamberOfShots(int value);

	void Fire_Implementation();

	void Dead_Implementation();

	//func to check if in Range
	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = "ObjClass"))
		UObject* CheckIfInRange();

	UFUNCTION()
		void PrepareFutureHitPoint(AActor* Target, FVector& OutToShoot);

	//func for Rotate Turret
	void RotateTurret();

	UFUNCTION() float GetAnimPitch() const { return AnimPitch; }


	OnUpdateHPSignature OnUpdateHPDelegate;
	OnUpdateMagazineAmountSignature OnUpdateMagazineAmountDelegate;

};


float CalcQuadraticEquation(float a, float b, float c);
float CalcQuadraticEquation(float a, float b, float c, float& x1, float& x2);