// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APickUp::APickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a BoxComp	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->SetBoxExtent(FVector((32.000000, 128.000000, 32.000000)));
	RootComponent = BoxComp;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	Body->SetupAttachment(BoxComp);
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void APickUp::BeginPlay()
{
	Super::BeginPlay();
	
}


