// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ThorHammer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AThorHammer::AThorHammer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShapeComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	SetRootComponent(SphereComp);
	MeshComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AThorHammer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThorHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThorHammer::StartHammerFly(FVector Direction)
{
}

void AThorHammer::ReturnHammerFly()
{
}

