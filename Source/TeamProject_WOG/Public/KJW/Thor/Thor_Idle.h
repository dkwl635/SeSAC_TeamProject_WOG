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

};
