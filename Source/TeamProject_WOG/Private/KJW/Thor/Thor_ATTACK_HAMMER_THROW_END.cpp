// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_HAMMER_THROW_END.h"
#include "KJW/ThorHammer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

void UThor_ATTACK_HAMMER_THROW_END::InitPattern(AThor* Thor)
{
	Super::InitPattern(Thor);

	HammerGoalComp = Thor->HammerComp;
}

void UThor_ATTACK_HAMMER_THROW_END::StartPattern_C()
{
	Super::StartPattern_C();
	
	//몽타주 시작시 데미지 히트 불가
	bHitable = false;
}

void UThor_ATTACK_HAMMER_THROW_END::StartPattern_CBP()
{
	Super::StartPattern_CBP();
	//받을려는 
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
}

void UThor_ATTACK_HAMMER_THROW_END::NotifyEventPattern_C(int32 EventIndex)
{
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Pause(AnimMontage);

}

bool UThor_ATTACK_HAMMER_THROW_END::TickPattern_C()
{
	if (!Owner->ThorHammer->IsHammerFly) { return true; }

	//날아오는 망치와 와야하는 위치의 길이 체크
	FVector goalPos = HammerGoalComp->GetComponentLocation();
	FVector flyHammerPos = Owner->ThorHammer->GetActorLocation();

	FVector flyMoveDir = goalPos - flyHammerPos;
	float Dis = flyMoveDir.Size();
	flyMoveDir.Normalize();
	//100정도 가까워지면
	if (Dis < 100)
	{
		Owner->ThorHammer->ReturnHammerFly();
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Start"), AnimMontage );
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Resume(AnimMontage);
		Owner->ShowHammer(true);

		//몽타주 시작시 데미지 히트 가능하게
		bHitable = true;
	}
	else
	{
		Owner->ThorHammer->HammerFly(flyMoveDir);
		FVector NewPos = Owner->ThorHammer->GetActorLocation();
		NewPos += (Owner->ThorHammer->MoveSpeed *1.5f) * flyMoveDir * Owner->GetWorld()->GetDeltaSeconds();
		Owner->ThorHammer->SetActorLocation(NewPos);
	}



	return true;
}

bool UThor_ATTACK_HAMMER_THROW_END::IsStartable()
{
	//현제 토르 해머가 땅에 있으면 패턴 변경 가능
	return !Owner->IsHammer;
}
