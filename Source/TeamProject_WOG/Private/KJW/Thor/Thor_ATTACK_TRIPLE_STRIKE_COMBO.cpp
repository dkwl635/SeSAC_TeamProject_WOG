// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_TRIPLE_STRIKE_COMBO.h"


void UThor_ATTACK_TRIPLE_STRIKE_COMBO::EndPattern_CBP()
{
	//패턴종료시 히트 상태 전환가능
	bHitable = true;

	UE_LOG(LogTemp , Warning , TEXT("UThor_ATTACK_TRIPLE_STRIKE_COMBO_Attack_EndPattern_CBP"));
	Super::EndPattern_CBP();
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::StartPattern_CBP()
{
	//패턴종료시 히트 상태 불가능
	bHitable = false;

	UE_LOG(LogTemp, Warning, TEXT("Attack_StartPattern_C"));
	AttackCount = 0;
	bAttack = false;
	 float TargetDistance = Owner->GetDistanceTo(Owner->Target);
	 if ( TargetDistance < AttackDistance )
	 {
		 Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		 bAttack = true;
	 }
	 else
	 {
		 Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(RunAnimMontage);
	 }
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::NotifyEventPattern_C(int32 EventIndex)
{
	if (!Owner) { return; }

	FHitResult OutHit;
	// 레이의 시작 지점
	FVector Start = FVector(0.f, 0.f, 0.f);
	//왼손 오른손
	if (EventIndex == 0)
	{
		Start = Owner->GetSkeletalMesh()->GetSocketLocation(TEXT("hand_l"));
	}
	else
	{
		Start = Owner->GetSkeletalMesh()->GetSocketLocation(TEXT("hand_r"));
	}

	// 구의 반경
	float Radius = 30.f;
	// 충돌 쿼리 파라미터
	FCollisionQueryParams CollisionParams;
	bool bHit = Owner->GetWorld()->SweepSingleByChannel(OutHit, Start, Start, FQuat::Identity, EWOGTraceChannel::EnemyAttackTrace, FCollisionShape::MakeSphere(Radius), CollisionParams);
	
	FColor SphereColor = bHit ? FColor::Red : FColor::Green;
	//DrawDebugSphere(Owner->GetWorld(), Start, Radius, 12, SphereColor, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = OutHit.ImpactPoint;
		Owner->Target->TakeKDamage(DamageData , Owner);

		UE_LOG(LogTemp, Warning, TEXT("충돌한 액터: %s"), *OutHit.GetActor()->GetName());
	}
	
	AttackCount++;

	//이러면 첫 이벤트 이후로는 히트 상태 전환가능
	bHitable = true;
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::NotifyTickPattrern_C(int32 EventIndex, float FrameDeltaTime)
{
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector TargetDirection = Owner->Target->GetActorLocation() - OwnerLocation;
	TargetDirection.Z = 0;

	float TargetDistance = TargetDirection.Size();
	TargetDirection.Normalize();
	FRotator rot = TargetDirection.Rotation();
	if ( !bAttack )
	{
		
		UE_LOG(LogTemp , Warning , TEXT("TargetDistance: %f") , TargetDistance);
		if ( TargetDistance < AttackDistance )
		{
			Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Stop(0.2f);
			Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
			bAttack = true;
		}
		else
		{		
			FVector p = OwnerLocation + TargetDirection * FrameDeltaTime * RunSpeed;
			Owner->SetActorLocationAndRotation(p , rot);
			
		}
	}
	else
	{
		if ( TargetDistance < 100 )
		{
			Owner->SetActorRotation(rot);
		}
		else
		{
			FVector p = OwnerLocation + TargetDirection * Walk_speed[AttackCount] * FrameDeltaTime;
			Owner->SetActorLocationAndRotation(p , rot);
		}
	}

}

bool UThor_ATTACK_TRIPLE_STRIKE_COMBO::IsStartable()
{
	//현제 토르 해머를 가지고 없다면
	return !Owner->IsHammer;
}
