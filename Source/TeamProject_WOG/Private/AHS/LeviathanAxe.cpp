// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/LeviathanAxe.h"
#include "Components/BoxComponent.h"
#include "AHS/KratosCharacter.h"
#include "Camera/CameraComponent.h"

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

	switch ( mState ) {
		case EAxeState::Default: {	DefaultState();	} break;
		case EAxeState::Thorwed: {	ThrowState();	} break;
	}
}

void ALeviathanAxe::DefaultState()
{

}

void ALeviathanAxe::ThrowState()
{
	//FVector destination = target->KratosCamComp
}
