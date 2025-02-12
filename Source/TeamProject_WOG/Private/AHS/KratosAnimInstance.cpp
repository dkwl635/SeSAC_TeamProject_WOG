// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/KratosAnimInstance.h"
#include "AHS/KratosCharacter.h"

void UKratosAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AKratosCharacter* kratos = Cast<AKratosCharacter>(TryGetPawnOwner());
	// 플레이어가 없으면 강제 종료.
	if ( kratos == nullptr ) {
		return;
	}

	// 앞뒤 이동
	FVector velocity = kratos->GetVelocity();
	FVector forwardVector = kratos->GetActorForwardVector();

	Speed = FVector::DotProduct(forwardVector, velocity);

	//좌우 이동
	FVector rightVector = kratos->GetActorRightVector();
	Direction = FVector::DotProduct(rightVector, velocity);
}
