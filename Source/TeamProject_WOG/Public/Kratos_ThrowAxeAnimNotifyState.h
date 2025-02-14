// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Kratos_ThrowAxeAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UKratos_ThrowAxeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration , const FAnimNotifyEventReference& EventReference) override;



	
};
