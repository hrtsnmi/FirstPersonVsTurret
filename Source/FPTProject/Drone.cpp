// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "FPTProjectProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"



void ADrone::DrawProjectilePath()
{
}

void ADrone::RemoveProjectilePath()
{
}

void ADrone::SpawnProjectile(float speed)
{
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
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a FloatingPawnMovement	
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Pawn Movement"));
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
	const FVector2D MoveXYScale{ Value.Get<FVector>() };

	const FVector Direction = GetActorForwardVector() * MoveXYScale.X +
		GetActorRightVector() * MoveXYScale.Y;

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
		SpawnProjectile(800.f);
	}
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ADrone::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ADrone::Fire);
	}
}
