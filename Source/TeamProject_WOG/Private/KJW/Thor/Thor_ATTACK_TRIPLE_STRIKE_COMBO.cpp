// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_TRIPLE_STRIKE_COMBO.h"


void UThor_ATTACK_TRIPLE_STRIKE_COMBO::StartPattern_C()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack_StartPattern_C"));

	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::StopPattern_C()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack_StopPattern_C"));
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::EndPattern_C()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack_EndPattern_C"));
	Owner->StartPattarn(EThorPattern::MOVE);
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
		Start =Owner->GetSkeletalMesh()->GetSocketLocation(TEXT("hand_r"));
	}

	// 구의 반경
	float Radius = 30.f;
	// 충돌 쿼리 파라미터
	FCollisionQueryParams CollisionParams;
	bool bHit = Owner->GetWorld()->SweepSingleByChannel(OutHit, Start, Start, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), CollisionParams);
	
	FColor SphereColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugSphere(Owner->GetWorld(), Start, Radius, 12, SphereColor, false, 1.0f, 0, 2.0f);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("충돌한 액터: %s"), *OutHit.GetActor()->GetName());
	}
}

void UThor_ATTACK_TRIPLE_STRIKE_COMBO::NotifyTickPattrern_C(int32 EventIndex, float FrameDeltaTime)
{
	//순간적으로 따라가게
	if (Owner && Owner->Target)
	{
		FVector Dir = Owner->Target->GetActorLocation() - Owner->GetActorLocation();
		Dir.Normalize();

		FRotator rot = Dir.Rotation();
		Owner->SetActorRotation(rot);
		Owner->SetActorLocation(Owner->GetActorLocation() + (Dir * Walk_speed * FrameDeltaTime));

	}

}
