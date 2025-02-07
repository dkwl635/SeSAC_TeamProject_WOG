// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_Idle.h"

void UThor_Idle::StartPattern_CBP()
{
	Owner->SetIsMove(false);
	if ( IdleTime <= 0 )
	{
		IdleTime = 2.0f;
	}
}

void UThor_Idle::TickPattern_CBP()
{
	if ( IdleTime > 0 )
	{
		IdleTime -= Owner->GetWorld()->GetDeltaSeconds();
		if ( IdleTime < 0 )
		{
			SetNextPattern();
		}
	}

	if ( !( Owner->Target ) ) { return; }

	FRotator StartRot = Owner -> GetActorRotation();
	FRotator EndRot = FRotator::ZeroRotator;

	float Delta = 0.2f;
	bool bShortestPath = true;
	
	FVector ActorLocation = Owner-> GetActorLocation();
	FVector TargetLocation = Owner -> Target ->GetActorLocation();
	EndRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation , TargetLocation);
	EndRot.Pitch = 0;
	
	EndRot = UKismetMathLibrary::RLerp(StartRot , EndRot , Delta , bShortestPath);
	Owner->SetActorRotation(EndRot);

}

void UThor_Idle::SetNextPattern()
{
	UE_LOG(LogTemp , Warning , TEXT("SetNextPattern"));
	EndPattern();
}

