// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_ATTACK_TRIPLE_STRIKE_COMBO.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_ATTACK_TRIPLE_STRIKE_COMBO : public UThorPattern
{
	GENERATED_BODY()
	
public:
	virtual void StartPattern_C() override;
	virtual void StopPattern_C() override;
	virtual void EndPattern_C() override;

public:
	//공격 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

public:
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual  void NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime) override;
};
