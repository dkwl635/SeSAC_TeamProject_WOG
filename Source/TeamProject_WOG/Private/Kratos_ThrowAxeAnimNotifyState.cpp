// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos_ThrowAxeAnimNotifyState.h"

void UKratos_ThrowAxeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration , const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp , Animation , TotalDuration , EventReference);
	
	FString logMsg = "Anim Notify Start";
	GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);
}
