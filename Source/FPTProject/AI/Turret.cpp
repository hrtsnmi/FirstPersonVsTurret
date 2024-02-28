

#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "../FPTProjectProjectile.h"
#include "Kismet/KismetSystemLibrary.h"//debug sphere
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Interfaces/TargetInterface.h"

// Sets default values
ATurret::ATurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(GetMesh());

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(), FName("gun_jntSocket"));

	LinesStartEnds.SetNum(20);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	//																	Turret is in range 1 to 0 for 55 -> 0 range degree
	AnimPitch = UKismetMathLibrary::NormalizeToRange(boardAngleRad, 0.0f, 55.f * PI / 180.f);
	AnimPitch = 1.0f - AnimPitch;
}

void ATurret::RotateTurret()
{
	FRotator LookAtRotation = FRotator::ZeroRotator;
	LookAtRotation.Yaw = GetActorRotation().Yaw + 1.f;

	RootComponent->SetWorldRotation(
		FMath::RInterpTo(
			GetActorRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			1000.f));
}

UObject* ATurret::CheckIfInRange()
{

	FVector VStart, VEnd;
	TArray<FHitResult> HitActors;
	bool hit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
		GetActorLocation(),
		GetActorLocation(), searchRadius,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera),
		false, {}, EDrawDebugTrace::ForDuration,
		HitActors, true, FColor::Cyan, FColor::Red);
	//
	if (hit)
	{
		for (FHitResult& HitActor : HitActors)
		{
			if (HitActor.GetActor()->GetClass()->ImplementsInterface(UTargetInterface::StaticClass()))
			{
				return HitActor.GetActor();
			}
		}
	}
	return nullptr;
}

void ATurret::DrawDebugElements() const
{
	UWorld* ThisWorld = GetWorld();

	//Draw Debug Elements
	FlushPersistentDebugLines(ThisWorld);

	//Draw Debug Elements

	int index = 0;

	FVector Start, End;

	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	//deltaDistance
	DrawDebugDirectionalArrow(ThisWorld, Start, End,
		// LinesStartEnds[index++]; - dont work!!!!!!!!!!!!!!!!!!!!!!!!
		10.f, FColor::Purple, true, 5.f, 100U, 0.0f);

	// TargerNormal
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugDirectionalArrow(ThisWorld, Start, End,
		10.f, FColor::Black, true, 5.f, 100U, 0.0f);

	// TargetForwardVector
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugDirectionalArrow(ThisWorld, Start, End,
		10.f, FColor::Red, true, 5.f, 100U, 0.0f);

	//To Hit
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugDirectionalArrow(ThisWorld, Start, End,
		10.f, FColor::Black, true, 5.f, 100U, 0.0f);


	// Lines:
	// 
	//Forward Infinite Line
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld, Start, End,
		FColor::Cyan, true, 5.f, 10U, 0.0f);

	////Distance
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld, Start, End,
		FColor::Yellow, true, 5.f, 10U, 0.0f);

	////h
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld, Start, End,
		FColor::Orange, true, 5.f, 10U, 0.0f);

	////TARGET tO hIT dISTANCE
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld, Start, End, FColor::White, true, 5.f, 10U, 0.0f);

	////Projectile tO hIT dISTANCE
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld,
		Start, End,
		FColor::Black, true, 5.f, 10U, 0.0f);

	////Radius
	Start = LinesStartEnds[index++];
	End = LinesStartEnds[index++];
	DrawDebugLine(ThisWorld, Start, End, FColor::Turquoise, true, 5.f, 10U, 0.0f);

	//Zones
	const FVector& PSPRef = ProjectileSpawnPoint->GetComponentLocation();
	/*DrawDebugSphere(ThisWorld, PSPRef,
		NotRangeRadius1, 10, FColor::Red);
	DrawDebugSphere(ThisWorld, PSPRef,
		NotRangeRadius2, 100, FColor::Red);

	DrawDebugSphere(ThisWorld, PSPRef,
		RangeRadius,
		100, FColor::Green);*/

	// Points of:
	//Projectile Spawn Point
	DrawDebugSphere(ThisWorld, PSPRef, 10.f, 26, FColor::Black, true, -1.f, 10U, 0.0f);

	//Hit Point
	DrawDebugSphere(ThisWorld, PointToShootCenter, 10.f, 26, FColor::Red, true, -1.f, 10U, 0.0f);
}

void ATurret::SetNewAnimPitch(bool ToTarget)
{
	float step{ 0.01f };

	if (ToTarget)
		step *= -1.f;

	AnimPitch = FMath::Clamp(AnimPitch + step, 0.18f, 1.f);
}

void ATurret::SetTergetTall(AActor* Target)
{
	if (Target->GetClass()->ImplementsInterface(UTargetInterface::StaticClass()))
	{
		TargetHalfTall = ITargetInterface::Execute_GetTargetHalfTall(Target);
	}
	else
	{
		TargetHalfTall = 200.f;
	}
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugElements();
}

void ATurret::Fire()
{
	FTransform ProjectileTransform;
	ProjectileTransform.SetLocation(ProjectileSpawnPoint->GetComponentLocation());
	ProjectileTransform.SetRotation(ProjectileSpawnPoint->GetComponentRotation().Quaternion());
	ProjectileTransform.SetScale3D(FVector(ProjectileScale));

	AFPTProjectProjectile* projectile = GetWorld()->SpawnActorDeferred<AFPTProjectProjectile>(ProjectileClass, ProjectileTransform, this);

	if (projectile)
	{
		projectile->AddOwnerSpeed() = ProjectileSpeed;
		//projectile->SetSize(ProjectileCapsuleRadius);

		projectile->FinishSpawning(ProjectileTransform);
	}
}


void ATurret::PrepareFutureHitPoint(AActor* Target, FVector& OutToShoot)
{
	//GetVelocity
	FVector TargetVelocity = Target->GetVelocity();

	FVector TargetForwardVector{};
	if (TargetVelocity == FVector::ZeroVector)
	{
		TargetForwardVector = Target->GetActorForwardVector();
	}
	else
	{
		TargetForwardVector = TargetVelocity;
		TargetForwardVector.Normalize();
	}

	//Start Target Location
	FVector TargetStartLocation = Target->GetActorLocation();
	//Start Projectile Spawn Location
	FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();


	// Projectile Speed for X and Z
	FVector2D ProjectileXZSpeed(
		ProjectileSpeed * FVector::DotProduct(GetActorForwardVector(), ProjectileSpawnPoint->GetForwardVector()),
		ProjectileSpeed * FVector::DotProduct(GetActorUpVector(), ProjectileSpawnPoint->GetForwardVector()));

	float Speed = ProjectileXZSpeed.Size();

	//Get Current Distance to Target
	FVector DistanceToTarget = TargetStartLocation - GetActorLocation();

	//Get Distance From Root Actor To Projectile Spawn Location
	float Radius = (ProjectileSpawnLocation - GetActorLocation()).Size();

	//Get Target Speed
	float TargetSpeed = TargetVelocity.Size();

	////Get Target Normal To Actor
	FVector TargetNormal;
	{
		FVector DistanceToTargetNormalize = DistanceToTarget;
		DistanceToTargetNormalize.Normalize();

		TargetNormal = FVector::CrossProduct(
			FVector::CrossProduct(TargetForwardVector, DistanceToTargetNormalize),
			TargetForwardVector);

		if (FVector::DotProduct(TargetNormal, DistanceToTarget) > 0.0f)
		{
			TargetNormal = -TargetNormal;
		}
		TargetNormal.Normalize();
	}

	//Get h - distance to  Target Forvard Vector
	FVector h = FVector::DotProduct(TargetNormal, DistanceToTarget) * TargetNormal;

	//Get deltaDistance - Distance between TargetStartLocation and end of h
	FVector DeltaDistance = FVector::DotProduct(TargetForwardVector, DistanceToTarget) * TargetForwardVector;
	float DeltaDistanceMagnitude = (FVector::DotProduct(TargetForwardVector, DeltaDistance) < 0.0f) ? (-DeltaDistance.Size()) : (DeltaDistance.Size());

	//with radius
	float RadiusProjectionX{ static_cast<float>
		(FVector::DotProduct(ProjectileSpawnPoint->GetForwardVector(), GetActorForwardVector() * Radius)) };

	float a = (ProjectileXZSpeed.X - TargetSpeed) * (ProjectileXZSpeed.X + TargetSpeed);
	float b = 2.0f * (ProjectileXZSpeed.X * RadiusProjectionX - DeltaDistanceMagnitude * TargetSpeed);
	float c = RadiusProjectionX * RadiusProjectionX - DeltaDistanceMagnitude * DeltaDistanceMagnitude - h.Size() * h.Size();

	//Get ballisticTime from /ax^2 + bx + c = 0
	float ballisticTime{ CalcQuadraticEquation(a, b, c) };

	//Get distanceToHit = Speed * Time
	float distanceToHit = TargetSpeed * ballisticTime;

	//Get Point of Hit!
	FVector HitPoint = TargetForwardVector * distanceToHit + TargetStartLocation;

	{
		//Get BorderTime Depends of Time in Flying
		float  ProjectilePointHeight = ProjectileSpawnLocation.Z;
		float  TargetHeight = HitPoint.Z;

		//Get Terget Tall
		SetTergetTall(Target);
		float ProjectileHalfTall = ProjectileScale * ProjectileCapsuleRadius;

		float TimeWhenProjectileIsOnFly{ };

		a = 0.5f * GetWorld()->GetGravityZ();
		b = ProjectileXZSpeed.Y;
		c = ProjectilePointHeight - TargetHeight;// +ProjectileHalfTall;// +ProjectileHalfTall - TargetHalfTall;

		TimeWhenProjectileIsOnFly = CalcQuadraticEquation(a, b, c);
		//TimeWhenProjectileIsOnFly *= 1.1f;
		RangeRadius = TimeWhenProjectileIsOnFly * ProjectileXZSpeed.X;

		bool TargetIsNotInRange = (ballisticTime > TimeWhenProjectileIsOnFly);// || (FMath::IsNearlyEqual(ballisticTime, TimeWhenProjectileIsOnFly, 0.1f));

		//check if Character height is in range of parabolic trajectory
		float t1{}, t2{};
		c = ProjectilePointHeight - TargetHeight - ProjectileHalfTall - TargetHalfTall;
		CalcQuadraticEquation(a, b, c, t1, t2);
		//Projectile Spawn Point

		NotRangeRadius1 = t1 * ProjectileXZSpeed.X;
		NotRangeRadius2 = t2 * ProjectileXZSpeed.X;

		bool leftBorder = (ballisticTime < t2); //|| (FMath::IsNearlyEqual(ballisticTime, t2, 0.01f));
		bool rightBorder = (ballisticTime > t1);// || (FMath::IsNearlyEqual(ballisticTime, t1, 0.01f));

		if (TargetIsNotInRange)
		{
			SetNewAnimPitch(true);

			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow,
					FString::Printf(TEXT("target is in air %f, but need %f"),
						TimeWhenProjectileIsOnFly, ballisticTime));
			}*/
			return;
		}
		else if (leftBorder && rightBorder) //check if Character height is in range of parabolic trajectory
		{
			SetNewAnimPitch();

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow,
					FString::Printf(TEXT("ballisticTime in Range Where Pawn height is not on parabola Trajectory")));
			}
			return;
		}
		else if ((t1 == 0.0) && (t1 == 0.0))
		{
			AnimPitch = 0.9f;
			//return;
		}


	}

	OutToShoot = HitPoint;

	/////////////////////////////////////////////////////////////////////////////////////////////////

	int index = 0;
	//Points for drawing Debug Elements
	//deltaDistance
	LinesStartEnds[index++] = (-DeltaDistance + TargetStartLocation);
	LinesStartEnds[index++] = (TargetStartLocation);

	// TargerNormal
	LinesStartEnds[index++] = (TargetStartLocation);
	LinesStartEnds[index++] = (TargetStartLocation + TargetNormal * 50.f);

	// TargetForwardVector
	LinesStartEnds[index++] = (TargetStartLocation);
	LinesStartEnds[index++] = (TargetStartLocation + TargetForwardVector * 50.f);

	//To Hit
	{
		FVector ToHitPointNorm = OutToShoot - ProjectileSpawnLocation;
		ToHitPointNorm.Normalize();

		LinesStartEnds[index++] = (ProjectileSpawnLocation);
		LinesStartEnds[index++] = (ProjectileSpawnLocation + ProjectileSpawnPoint->GetForwardVector() * 50.f);
	}



	// Points of:

	//Hit Point
	PointToShootCenter = OutToShoot;


	// Lines:
	// 
	//Forward Infinite Line
	LinesStartEnds[index++] = (TargetStartLocation + TargetForwardVector * 10000.f);
	LinesStartEnds[index++] = (TargetStartLocation - TargetForwardVector * 10000.f);

	////Distance
	LinesStartEnds[index++] = (GetActorLocation());
	LinesStartEnds[index++] = (GetActorLocation() + DistanceToTarget);

	////h
	LinesStartEnds[index++] = (GetActorLocation());
	LinesStartEnds[index++] = (GetActorLocation() + h);

	////TARGET tO hIT dISTANCE
	LinesStartEnds[index++] = (TargetStartLocation);
	LinesStartEnds[index++] = (OutToShoot);

	////Projectile tO hIT dISTANCE
	FVector FromCenterToTarhet = OutToShoot - GetActorLocation();
	FromCenterToTarhet.Normalize();

	LinesStartEnds[index++] = (GetActorLocation() + FromCenterToTarhet * RadiusProjectionX);
	LinesStartEnds[index++] = (OutToShoot);

	////Radius
	LinesStartEnds[index++] = (GetActorLocation());
	LinesStartEnds[index++] = (GetActorLocation() + FromCenterToTarhet * RadiusProjectionX);
}

float CalcQuadraticEquation(float a, float b, float c)
{
	float x1, x2;
	return CalcQuadraticEquation(a, b, c, x1, x2);
}


float CalcQuadraticEquation(float a, float b, float c, float& x1, float& x2)
{
	float Result{ -1.0f };

	if (a == 0.0f)
	{
		if (b != 0.0f)
		{
			//Speeds of Projectile and Target is equal
			Result = -c / b;
		}
	}
	else if (b == 0.0f)
	{
		float TimeSquare = -c / a;
		if (TimeSquare >= 0.f)
		{
			Result = FMath::Sqrt(TimeSquare);
		}
	}
	else if (c == 0.0f)
	{
		Result = -b / a;
	}
	else
	{
		//full ax^2 + bx + c = 0
		float Discriminante = b * b - 4.0f * a * c;
		if (Discriminante < 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Discriminante: %f"), Discriminante);
			return Result;
		}
		x1 = (-b + FMath::Sqrt(Discriminante)) / (2.0f * a);
		x2 = (-b - FMath::Sqrt(Discriminante)) / (2.0f * a);

		Result = FMath::Max<float>(x1, x2);
	}

	if (x1 > x2)
	{
		Swap(x1, x2);
	}

	return Result;
}