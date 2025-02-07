// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_ATTACK_HAMMER_THROW_START.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_ATTACK_HAMMER_THROW_START : public UThorPattern
{
	GENERATED_BODY()
	
public:
	//망치 날리기 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	//백대쉬 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashAnimMontage;

	//킥 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* KickAnimMontage;
	UPROPERTY(EditAnywhere)
	float AttackRange = 500.f;

	//0-백대쉬  
	//1-킥
	//2-망치 던지기
	int32 ParrtenIndex = 0;
	FVector StartPos = FVector::ZeroVector;
	FVector MovePos = FVector::ZeroVector;

	float DashTimer = 0;
	float DashDuringTimer = 0;
protected:
	virtual void StartPattern_C() override;
	virtual void StartPattern_CBP() override;

	virtual bool TickPattern_C() override;
	virtual void NotifyTickPattrern_C(int32 EventIndex , float FrameDeltaTime) override;
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
};
