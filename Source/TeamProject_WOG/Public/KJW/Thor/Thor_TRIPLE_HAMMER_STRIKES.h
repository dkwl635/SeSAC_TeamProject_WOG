// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_TRIPLE_HAMMER_STRIKES.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_TRIPLE_HAMMER_STRIKES : public UThorPattern
{
	GENERATED_BODY()
	

public:
	//망치 3연 콤보 공격 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	//달리기 애니 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* RunAnimMontage;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 400.f;
	

	UPROPERTY(EditAnywhere)
	float SlowAnimRate = 0.2f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere)
	float AttackDistance = 300.0f;
	UPROPERTY(EditAnywhere)
	float RunSpeed = 700.0f;

	bool bAttack = false;

protected:
	virtual void StartPattern_CBP() override;
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual void NotifyBeginPattern_C(int32 EventIndex , float TotalDuration) override;
	virtual void NotifyEndPattrern_C(int32 EventIndex) override;
	virtual void NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime) override;
	virtual void EndPattern_CBP() override;
	virtual bool IsStartable() override;

};
