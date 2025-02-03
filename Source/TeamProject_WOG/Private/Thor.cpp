// Fill out your copyright notice in the Description page of Project Settings.
#include "Thor.h"
#include "TeamProject_WOG/TeamProject_WOG.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AThor::AThor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyComp"));

	SetRootComponent(BodyCollision);
	BodyComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AThor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

