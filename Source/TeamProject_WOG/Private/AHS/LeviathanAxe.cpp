// Fill out your copyright notice in the Description page of Project Settings.


#include "LeviathanAxe.h"
#include "Components/BoxComponent.h"

ALeviathanAxe::ALeviathanAxe()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
}

void ALeviathanAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
