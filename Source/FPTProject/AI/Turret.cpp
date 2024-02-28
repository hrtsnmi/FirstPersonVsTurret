

#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "../FPTProjectProjectile.h"
#include "Kismet/KismetSystemLibrary.h"//debug sphere
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Interfaces/TargetInterface.h"

#include "Components/WidgetComponent.h"
#include "../UI/MyUserWidget.h"


// Sets default values
ATurret::ATurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(GetMesh());

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GetMesh(), FName("gun_jntSocket"));

	WorldWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("World Widget"));
	WorldWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	//																	Turret is in range 1 to 0 for 55 -> 0 range degree
	//AnimPitch = UKismetMathLibrary::NormalizeToRange(boardAngleRad, 0.0f, 55.f * PI / 180.f);
	//AnimPitch = 1.0f - AnimPitch;

	AnimPitch = 0.75f;

	if(TurretHUD)
	{
		CurrentWidget = CreateWidget<UMyUserWidget>(GetWorld(), TurretHUD);
		WorldWidget->SetWidget(CurrentWidget);
		CurrentWidget->SetUpPawnDelegates(this);

		AddMagasine(0, CurrentMagazine, this, OnUpdateMagazineAmountDelegate);
		AddHP(0, CurrentHP, MaxHP, this, OnUpdateHPDelegate);
	}


	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATurret::ComponentBeginOverlap);
}

void ATurret::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (!OtherActor->GetClass()->ImplementsInterface(UUIInterface::StaticClass()))
		{
			return;
		}

		if (IUIInterface::Execute_GetItemID(OtherActor) == 2)
		{
			AddHP(IUIInterface::Execute_GetItemValue(OtherActor),
				CurrentHP, MaxHP, this, OnUpdateHPDelegate);
		}

		OtherActor->Destroy();
	}
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

void ATurret::SetNewAnimPitch(FVector Location)
{
	//FVector DistanceToTarget = Location - GetActorLocation();
	//DistanceToTarget.Normalize();

	

	//AnimPitch = FMath::Clamp(AnimPitch, 0.18f, 1.f);
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

	//DrawDebugElements();
}

void ATurret::DoNumberOfShots(int value)
{
	if (GetWorldTimerManager().IsTimerActive(ReloadTimer))
	{
		return;
	}

	if (CurrentMagazine == 0)
	{
		CurrentMagazine = 20;
		GetWorldTimerManager().SetTimer(ReloadTimer, 3.0f, false);
		return;
	}

	AddMagasine(-value, CurrentMagazine, this, OnUpdateMagazineAmountDelegate);
}

void ATurret::Fire_Implementation()
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

void ATurret::Dead_Implementation()
{

	Destroy();
}

void ATurret::PrepareFutureHitPoint(AActor* Target, FVector& OutToShoot)
{
	OutToShoot = Target->GetActorLocation();
	
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