// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ThorAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThorAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	class AThor* Thor;
public:
	UFUNCTION(BlueprintCallable)
	void SetThor(AThor* Value) { Thor = Value; }
};
