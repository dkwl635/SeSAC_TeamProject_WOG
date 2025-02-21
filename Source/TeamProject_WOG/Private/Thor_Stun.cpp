// Fill out your copyright notice in the Description page of Project Settings.


#include "Thor_Stun.h"

void UThor_Stun::StartPattern_CBP()
{
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(StunAnimMontage);
}

void UThor_Stun::EndPattern_CBP()
{
	Owner->StunValue = 0;
	if ( Owner->UpdateStun.IsBound() )	Owner->UpdateStun.Execute();

	Super::EndPattern_CBP();
}
