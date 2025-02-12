// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KratosAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UKratosAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 매 프레임마다 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 플레이어 앞뒤 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = KratosAnimInstance)
	float Speed = 0.0f;

	// 플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = KratosAnimInstance)
	float Direction = 0.0f;

	
};
