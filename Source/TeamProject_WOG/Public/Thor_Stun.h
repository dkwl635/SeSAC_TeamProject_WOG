// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_Stun.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_Stun : public UThorPattern
{
	GENERATED_BODY()
	

public:
	//스턴 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* StunAnimMontage;

	virtual bool IsStartable() override { return false; } 
	
	virtual void StartPattern_CBP();
	virtual void EndPattern_CBP();
};
