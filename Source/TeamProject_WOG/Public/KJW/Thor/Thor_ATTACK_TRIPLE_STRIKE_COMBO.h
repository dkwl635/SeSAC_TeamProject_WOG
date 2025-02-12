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
	
protected:
	virtual void EndPattern_CBP() override;
	virtual void StartPattern_CBP() override;
protected:
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual  void NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime) override;

	virtual bool IsStartable() override;// { return true; }
public:
	//공격 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;
	//달리기 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* RunAnimMontage;

	int32 AttackCount = 0;
	UPROPERTY(EditAnywhere)
	TArray<float> Walk_speed;


	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere)
	float AttackDistance = 300.0f;
	UPROPERTY(EditAnywhere)
	float RunSpeed = 700.0f;

	bool bAttack = false;


};
