// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_HIT.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_HIT : public UThorPattern
{
	GENERATED_BODY()
	
public:

	//전방 Hit 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* FHitAnimMontage;
public:
	virtual void InitPattern(class AThor* Thor) override;
	virtual bool IsStartable() override { return false; }
	virtual void EndPattern_CBP();
protected:
	virtual void StartPattern_CBP() override;

	int32 HitCount = 0;

};
