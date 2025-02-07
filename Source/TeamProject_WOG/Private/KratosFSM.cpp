// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosFSM.h"

// Sets default values for this component's properties
UKratosFSM::UKratosFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKratosFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKratosFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);

	switch ( mState ) {
	case EKratosState::Idle: { IdleState(); } break;
	case EKratosState::Move: { MoveState(); } break;
	case EKratosState::Attack: { AttackState(); } break;
	case EKratosState::Damage: { DamageState(); } break;
	case EKratosState::Die: { DieState(); } break;
	}
}

void UKratosFSM::IdleState()
{

}

void UKratosFSM::MoveState()
{

}

void UKratosFSM::AttackState()
{

}

void UKratosFSM::DamageState()
{

}

void UKratosFSM::DieState()
{

}

