// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Thor/Thor_HIT.h"

void UThor_HIT::InitPattern(AThor* Thor)
{
	Super::InitPattern(Thor);

	bHitable = true;
	HitCount = 0;
}

void UThor_HIT::EndPattern_CBP()
{
	bHitable = true;
	IdleTime = 0.3f;
	
	Owner->GetPattern(EThorPattern::IDLE)->SetOptionValue(IdleTime , bHitable);
	Owner->StartPattarn(EThorPattern::IDLE);
	//Super::EndPattern_CBP();
}

void UThor_HIT::StartPattern_CBP()
{


	//Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(FHitAnimMontage);
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(HitAnimMontages[HitCount]);
	bHitable = true;

	HitCount++;
	if (HitCount >= 3)
	{
		//회피 모션 나오게 하기
		bHitable = false;
		HitCount = 0;
	}
	
}


