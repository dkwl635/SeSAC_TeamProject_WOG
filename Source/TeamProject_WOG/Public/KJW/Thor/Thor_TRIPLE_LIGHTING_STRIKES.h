// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KJW/ThorPattern.h"
#include "Thor_TRIPLE_LIGHTING_STRIKES.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UThor_TRIPLE_LIGHTING_STRIKES : public UThorPattern
{
	GENERATED_BODY()
	

public:
	//내려 찍는 모션 몽타주
	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

	//번개 BP class
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AThorLighting> ThorLightingClass;

	//번개 풀
	UPROPERTY(EditDefaultsOnly)
	TArray<AThorLighting*> ThorLightings;
	UPROPERTY(EditDefaultsOnly)
	int32 LightingInitSpawnCount = 3;
	
	int32 LightingSpawnCount = 0;
	int32 LightingShowCount = 0;
protected:	
	virtual void InitPattern(class AThor* Thor) override;
	virtual void StartPattern_CBP() override;
	virtual void NotifyEventPattern_C(int32 EventIndex) override;
	virtual void StopPattern_C() override;

	FTimerHandle SpawnLightingTimerHandle;
	void SpawnLightings();
};
