// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_ATTACK_HAMMER_THROW_END.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_ATTACK_HAMMER_THROW_END : public UThorPattern
{
	GENERATED_BODY()
	

public:
	//망치 날리기 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	UPROPERTY()
	class UStaticMeshComponent* HammerGoalComp;
public:
	virtual void InitPattern(class AThor* Thor) override;
	virtual void StartPattern_C() override;
	virtual void StartPattern_CBP() override;

	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual bool TickPattern_C() override;
	

	virtual bool IsStartable() override; //{ return true; }
};
