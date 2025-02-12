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
	if ( !( Owner->Target ) ) { return; }

	FRotator StartRot = Owner->GetActorRotation();
	FRotator EndRot = FRotator::ZeroRotator;

	float Delta = 0.1f;
	bool bShortestPath = true;

	FVector ActorLocation = Owner->GetActorLocation();
	FVector TargetLocation = Owner->Target->GetActorLocation();
	EndRot = UKismetMathLibrary::FindLookAtRotation(ActorLocation , TargetLocation);
	EndRot.Pitch = 0;

	EndRot = UKismetMathLibrary::RLerp(StartRot , EndRot , Delta , bShortestPath);
	Owner->SetActorRotation(EndRot);

	if ( IdleTime > 0 )
	{
		IdleTime -= Owner->GetWorld()->GetDeltaSeconds();
		if ( IdleTime < 0 )
		{
			SetNextPattern();
		}
	}

}

void UThor_Idle::SetNextPattern()
{
	UE_LOG(LogTemp , Warning , TEXT("SetNextPattern"));

	EThorPattern newPattern = EThorPattern::NONE;
	
	//1 패턴중 시작 가능한 패턴 가져오기
	TMap<EThorPattern , UThorPattern*>& patterns = Owner->GetPattern();
	
	TArray<UThorPattern*> StartablePattern;
	for ( auto pair : patterns )
	{
		if ( pair.Value->IsStartable() )
		{
			StartablePattern.Add(pair.Value);
		}
	}

	//2 시작가능한 패턴 중 무작위 패턴 실행
	int32 RandIndex = FMath::RandRange(0 , StartablePattern.Num() -1);
	newPattern = StartablePattern[RandIndex]->ThorPattern;

	if ( newPattern != EThorPattern::NONE )
	{
		UE_LOG(LogTemp , Warning , TEXT("%s") , *StartablePattern[RandIndex]->GetName());

		Owner->StartPattarn(newPattern);
	}

	//EndPattern();
}

