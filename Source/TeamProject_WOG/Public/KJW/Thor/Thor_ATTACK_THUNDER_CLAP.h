// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_ATTACK_THUNDER_CLAP.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_ATTACK_THUNDER_CLAP : public UThorPattern
{
	GENERATED_BODY()
	

protected:
	virtual void StartPattern_CBP() override;
protected:
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual  void NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime) override;


public:
	//박수 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	//대쉬 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashAnimMontage;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 200.f;
	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere)
	float DashSpeed = 1300.0f;
	
	bool bAttack = false;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* Effect;
};
