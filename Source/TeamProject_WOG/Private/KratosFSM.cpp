// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosFSM.h"
#include "AHS/KratosCharacter.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UKratosFSM::UKratosFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bHasAxe = false;
}


// Called when the game starts
void UKratosFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	KratosCharacter = Cast<AKratosCharacter>(GetOwner());
	
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



void UKratosFSM::SetState(EKratosState NewState)
{
	mState = NewState;
}

void UKratosFSM::IdleState()
{
	if ( KratosCharacter && KratosCharacter->GetCharacterMovement()->Velocity.Size() > 0 )
	{
		SetState(EKratosState::Move);
	}
}

void UKratosFSM::MoveState()
{
	if ( KratosCharacter && KratosCharacter->GetCharacterMovement()->Velocity.Size() == 0 )
	{
		SetState(EKratosState::Idle);
	}
}

void UKratosFSM::EquipAxe()
{
	bHasAxe = !bHasAxe;
	UpdateWeaponState(bHasAxe);
}


void UKratosFSM::AttackState()
{
	if ( !bHasAxe )
	{
		PlayPunchAttack();
	}
	else if ( IsAiming() )
	{
		SetState(EKratosState::AxeRangedAttack);
	}
	else
	{
		SetState(EKratosState::AxeMeleeAttack);
	}
}

void UKratosFSM::AxeMeleeAttackState()
{

}

void UKratosFSM::AxeRangedAttackState()
{

}

//--------------------------------------------------------
void UKratosFSM::DamageState()
{
	// 일정 시간 후 다시 Idle 상태로 변경
	if ( bDamageRecovered )
	{
		SetState(EKratosState::Idle);
	}
}

void UKratosFSM::DieState()
{
	// 캐릭터 조작 불가능 처리
	KratosCharacter->DisableInput(nullptr);
}

//--------------------------------------------------------
void UKratosFSM::UpdateWeaponState(bool bNewHasAxe)
{
	bHasAxe = bNewHasAxe;
}

void UKratosFSM::PlayPunchAttack()
{
	if ( KratosCharacter )
	{
		UAnimInstance* AnimInstance = KratosCharacter->GetMesh()->GetAnimInstance();
		if ( AnimInstance )
		{
			AnimInstance->Montage_Play(KratosCharacter->Fist_Attack_Montage);
		}
	}
}

bool UKratosFSM::IsAiming()
{
	return KratosCharacter && KratosCharacter->AimAttackState;
}