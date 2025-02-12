// Fill out your copyright notice in the Description page of Project Settings.

#include "KJW/ThorLighting.h"
#include "Components/SphereComponent.h"
#include "KJW/Thor.h"
#include "AHS/KratosCharacter.h"

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

void AThorLighting::TakeLightingDamage()
{
	if ( Thor )
	{
		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = GetActorLocation();
		Thor->Target->TakeKDamage(DamageData , Thor);
	}
	
}

