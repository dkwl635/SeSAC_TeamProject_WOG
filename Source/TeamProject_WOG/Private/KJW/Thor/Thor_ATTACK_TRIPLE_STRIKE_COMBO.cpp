// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_TRIPLE_STRIKE_COMBO.h"


void UThor_ATTACK_TRIPLE_STRIKE_COMBO::EndPattern_CBP()
{
	UE_LOG(LogTemp , Warning , TEXT("UThor_ATTACK_TRIPLE_STRIKE_COMBO_Attack_EndPattern_CBP"));
	Owner->SetIdleTimer(4.0f);
	Super::EndPattern_CBP();
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::StartPattern_CBP()
{
	Super::StartPattern_CBP();
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
	DrawDebugSphere(Owner->GetWorld(), Start, Radius, 12, SphereColor, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("충돌한 액터: %s"), *OutHit.GetActor()->GetName());
	}
	
	AttackCount++;
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
