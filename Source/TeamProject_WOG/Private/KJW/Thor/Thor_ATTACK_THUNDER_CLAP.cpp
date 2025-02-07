// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_THUNDER_CLAP.h"


void UThor_ATTACK_THUNDER_CLAP::StartPattern_CBP()
{
	Super::StartPattern_CBP();

	bAttack = false;
	float TargetDistance = Owner->GetDistanceTo(Owner->Target);
	if ( TargetDistance < AttackRadius * 2.0f)
	{
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
		bAttack = true;
	}
	else
	{
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(DashAnimMontage , 0.1f);
	}
	
}


void UThor_ATTACK_THUNDER_CLAP::NotifyEventPattern_C(int32 EventIndex)
{
	if ( !Owner ) { return; }

	FHitResult OutHit;
	// 레이의 시작 지점
	FVector Start = FVector(0.f , 0.f , 0.f);
	Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * AttackRadius;

	// 충돌 쿼리 파라미터
	FCollisionQueryParams CollisionParams;
	bool bHit = Owner->GetWorld()->SweepSingleByChannel(OutHit , Start , Start , FQuat::Identity , EWOGTraceChannel::EnemyAttackTrace , FCollisionShape::MakeSphere(AttackRadius) , CollisionParams);

	FColor SphereColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugSphere(Owner->GetWorld() , Start , AttackRadius , 12 , SphereColor , false , 1.0f , 0 , 2.0f);

	if ( bHit )
	{
		UE_LOG(LogTemp , Warning , TEXT("충돌한 액터: %s") , *OutHit.GetActor()->GetName());
	}
}

void UThor_ATTACK_THUNDER_CLAP::NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime)
{
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector TargetDirection = Owner->Target->GetActorLocation() - OwnerLocation;
	TargetDirection.Z = 0;

	float TargetDistance = TargetDirection.Size();
	TargetDirection.Normalize();
	FRotator rot = TargetDirection.Rotation();
	if ( bAttack )
	{
		Owner->SetActorRotation(rot);
	}
	else
	{
		if ( TargetDistance < AttackRadius)
		{
			Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Stop(0.2f);
			Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
			bAttack = true;
		}
		else
		{
			FVector p = OwnerLocation + TargetDirection * FrameDeltaTime * DashSpeed;
			Owner->SetActorLocationAndRotation(p , rot);

		}
	}

	
}
