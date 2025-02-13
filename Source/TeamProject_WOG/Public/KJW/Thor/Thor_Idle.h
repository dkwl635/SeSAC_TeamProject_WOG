// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_Idle.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_Idle : public UThorPattern
{
	GENERATED_BODY()
	

protected:
	virtual void StartPattern_CBP() override;
	virtual void TickPattern_CBP() override;
protected:
	void SetNextPattern();

public:
	float IdleTime;
	virtual bool IsStartable() override { return false; } 
	virtual bool IsHitable() override;
public:
	//특수한 값을 줄때 사용할 함수
	virtual void SetOptionValue(float Value_1 = 0 , float Value_2 = 0 , float Value_3 = 0) override;

	UPROPERTY(EditDefaultsOnly)
	bool IsAI = true;
};
