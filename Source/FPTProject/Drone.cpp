// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "FPTProjectProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



void ADrone::DrawProjectilePath()
{
	const FVector WhereCharacterLooks = FRotator(GetFirstPersonCameraComponent()->GetComponentRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll).Vector();

	//DrawDebugDirectionalArrow(GetWorld(), WorldSpawnLocation, WorldSpawnLocation + WhereCharacterLooks * 500.f, 50.f, FColor::Red);// false, 5.f);
	//DrawDebugSphere(GetWorld(), WorldSpawnLocation, 10.f, 10, FColor::Orange);

	FPredictProjectilePathResult PredictResult;

	ThownTransform.SetLocation(ProjectileSpawnLocation->GetComponentLocation());
	ThownTransform.SetRotation(FRotator(GetFirstPersonCameraComponent()->GetComponentRotation().Pitch,
		GetActorRotation().Yaw, GetActorRotation().Roll).Quaternion());


	PredictParams.StartLocation = ProjectileSpawnLocation->GetComponentLocation();
	PredictParams.LaunchVelocity = StartSpeed * WhereCharacterLooks + GetVelocity();

	ClearSpline(SplineMeshes, ProjectileSpline);
	SplineMeshes.Reserve(PredictResult.PathData.Num());

	bool CanPredictKnifePagh = UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

	if (CanPredictKnifePagh && OnAddSplineMeshAtIndex.IsBound())
	{
		for (int32 i = 0; i < PredictResult.PathData.Num(); ++i)
		{
			ProjectileSpline->AddSplinePoint(PredictResult.PathData[i].Location, ESplineCoordinateSpace::Local);
		}
		for (int32 i = 0; i < PredictResult.PathData.Num() - 1; ++i)
		{
			//ExecuteDelegate for Add mesh in Blueprint
			OnAddSplineMeshAtIndex.Broadcast(i);
		}
	}

	ProjectileSpline->SetSplinePointType(PredictResult.PathData.Num() - 1, ESplinePointType::CurveClamped);
	EndSpline->SetWorldLocation(PredictResult.PathData.Last().Location);
	EndSpline->SetVisibility(true);
}

void ADrone::RemoveProjectilePath()
{
	ClearSpline(SplineMeshes, ProjectileSpline);
	EndSpline->SetVisibility(false);

	//GetCameraBoom()->SocketOffset = PrevCameraData;
	//GetCameraBoom()->TargetArmLength = PrevAimOfset;
}

AFPTProjectProjectile* ADrone::SpawnProjectile(float speed)
{
	if (!fptProjectile) return nullptr;

	const float CurrCharacterSpeed =
		FVector::DotProduct(GetActorForwardVector(), GetVelocity()) < 0 ?
		(-GetVelocity().Size()) :
		(GetVelocity().Size());

	AFPTProjectProjectile* ThownProjectile = GetWorld()->SpawnActorDeferred<AFPTProjectProjectile>
		(fptProjectile, ThownTransform, this);
	if (ThownProjectile)
	{
		ThownProjectile->AddOwnerSpeed() = CurrCharacterSpeed + speed;
		ThownProjectile->FinishSpawning(ThownTransform);
	}

	return ThownProjectile;
}

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a BoxComp	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->SetBoxExtent(FVector((32.000000, 128.000000, 32.000000)));
	RootComponent = BoxComp;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(BoxComp);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(10.f, 0.f, 10.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a FloatingPawnMovement	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	Body->SetupAttachment(BoxComp);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create a USceneComponent
	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Location"));
	ProjectileSpawnLocation->SetupAttachment(FirstPersonCameraComponent);

	//Predict path
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 5.f;
	PredictParams.bTraceWithChannel = true;
	PredictParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PredictParams.ActorsToIgnore = { this };
	PredictParams.DrawDebugType = EDrawDebugTrace::None;

	//ProjectilePath
	EndSpline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("End Spline Mesh"));;
	EndSpline->SetupAttachment(ProjectileSpawnLocation);
	EndSpline->SetVisibility(false);
	EndSpline->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Projectile Path Spline"));;
	ProjectileSpline->SetupAttachment(ProjectileSpawnLocation);

}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADrone::MoveVertical(const FInputActionValue& Value)
{
	float VerticalValueScale{ Value.Get<float>() };

	FloatingPawnMovement->AddInputVector(GetActorUpVector() * VerticalValueScale);
}

void ADrone::Move(const FInputActionValue& Value)
{
	const FVector2D MoveXYScale{ Value.Get<FVector2D>() };

	const FVector Direction = GetActorForwardVector() * MoveXYScale.Y +
		GetActorRightVector() * MoveXYScale.X;

	FloatingPawnMovement->AddInputVector(Direction);
}


void ADrone::Look(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void ADrone::Fire(const FInputActionValue& Value)
{
	static bool FireStarted{};
	FireStarted = Value.Get<bool>();
	
	if (FireStarted)
	{
		//Draw path
		DrawProjectilePath();
	}
	else
	{
		//Remove path
		RemoveProjectilePath();

		//Fire
		SpawnProjectile(StartSpeed);
	}
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	DrawDebugSphere(GetWorld(), ProjectileSpawnLocation->GetComponentLocation(), 10.f, 10, FColor::Orange);
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
		EnhancedInputComponent->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &ADrone::MoveVertical);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);

		//Looking
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ADrone::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ADrone::Fire);
	}
}

void ClearSpline(TArray<USplineMeshComponent*>& SplineMeshes, USplineComponent* Spline)
{
	if (SplineMeshes.Num() > 0)
	{
		for (USplineMeshComponent* SplineMeshComponent : SplineMeshes)
		{
			SplineMeshComponent->DestroyComponent();
		}
		SplineMeshes.Empty();
	}
	Spline->ClearSplinePoints();
}