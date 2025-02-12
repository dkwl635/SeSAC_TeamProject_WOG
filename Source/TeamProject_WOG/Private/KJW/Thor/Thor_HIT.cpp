// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Thor/Thor_HIT.h"

void UThor_HIT::StartPattern_CBP()
{
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(FHitAnimMontage);
}
