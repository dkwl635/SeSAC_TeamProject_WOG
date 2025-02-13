// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_TRIPLE_HAMMER_STRIKES.h"

void UThor_TRIPLE_HAMMER_STRIKES::StartPattern_CBP()
{
	//시작시 히트 상태 전환불가
	bHitable = false;

	UE_LOG(LogTemp , Warning , TEXT("Attack_StartPattern_C"));
	bAttack = false;
	float TargetDistance = Owner->GetDistanceTo(Owner->Target);
	if ( TargetDistance < AttackDistance )
	{
		bAttack = true;
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
	}
	else
	{
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(RunAnimMontage);
	}
}

void UThor_TRIPLE_HAMMER_STRIKES::NotifyEventPattern_C(int32 EventIndex)
{
	FHitResult OutHit;
	// 레이의 시작 지점
	FVector Start = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100;
	// 구의 반경
	float Radius = 60.0f;;
	// 충돌 쿼리 파라미터
	FCollisionQueryParams CollisionParams;
	bool bHit = Owner->GetWorld()->SweepSingleByChannel(OutHit , Start , Start , FQuat::Identity , ECC_Visibility , FCollisionShape::MakeSphere(Radius) , CollisionParams);

	FColor SphereColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugSphere(Owner->GetWorld() , Start , Radius , 12 , SphereColor , false , 1.0f , 0 , 2.0f);

	if ( bHit )
	{
		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = OutHit.ImpactPoint;
		Owner->Target->TakeKDamage(DamageData , Owner);
		UE_LOG(LogTemp , Warning , TEXT("충돌한 액터: %s") , *OutHit.GetActor()->GetName());
	}

	//첫 이벤트 이후 히트 상태 전환가능
	bHitable = true;

}

void UThor_TRIPLE_HAMMER_STRIKES::NotifyBeginPattern_C(int32 EventIndex , float TotalDuration)
{
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_SetPlayRate(AnimMontage , SlowAnimRate);
}

void UThor_TRIPLE_HAMMER_STRIKES::NotifyEndPattrern_C(int32 EventIndex)
{
	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_SetPlayRate(AnimMontage , 1.0f);
}

void UThor_TRIPLE_HAMMER_STRIKES::NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime)
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
			FVector p = OwnerLocation + TargetDirection * WalkSpeed * FrameDeltaTime;
			Owner->SetActorLocationAndRotation(p , rot);
		}
	}




	


}

bool UThor_TRIPLE_HAMMER_STRIKES::IsStartable()
{
	//토르가 해머를 가지고 있다면 시작가능
	return Owner->IsHammer;
}
