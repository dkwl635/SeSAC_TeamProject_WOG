// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ThorAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	bool IsMove = false;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class AThor* Thor;
public:
	virtual void NativeInitializeAnimation() override;

};
