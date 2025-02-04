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
	
	
protected:
	void StartPattern_C() override;
	void StopPattern_C() override;
	void EndPattern_C() override;
	bool TickPattern_C() override;



};
