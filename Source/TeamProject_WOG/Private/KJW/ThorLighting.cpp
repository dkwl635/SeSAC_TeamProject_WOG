// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/ThorLighting.h"
#include "Components/SphereComponent.h"

// Sets default values
AThorLighting::AThorLighting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
}

// Called when the game starts or when spawned
void AThorLighting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThorLighting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThorLighting::SpawnLighting_Implementation()
{
}

