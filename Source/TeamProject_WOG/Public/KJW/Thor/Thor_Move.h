// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_Move.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_Move : public UThorPattern
{
	GENERATED_BODY()
	
public:
	virtual void InitPattern(class AThor* Thor) override;
	
protected:
	void StartPattern_C() override;
	void StopPattern_C() override;
	void EndPattern_C() override;
	bool TickPattern_C() override;

	virtual void NotifyEventPattern_C(int32 EventIndex) override;

public:
	FVector SpawnPos = FVector::ZeroVector;
	FVector MovePos = FVector::ZeroVector;
	FVector Direction = FVector::ZeroVector;

	
	
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 300.0f;

	

};
