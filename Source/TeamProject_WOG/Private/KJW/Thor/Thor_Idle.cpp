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
	if ( !IsAI ) { return; }

	UE_LOG(LogTemp , Warning , TEXT("SetNextPattern"));

	EThorPattern newPattern = EThorPattern::NONE;
	
	//1 패턴중 시작 가능한 패턴 가져오기
	TMap<EThorPattern , UThorPattern*>& patterns = Owner->GetPattern();
	int32 rate = 0;
	TArray<UThorPattern*> StartablePattern;
	for ( auto pair : patterns )
	{
		if ( pair.Value->IsStartable() )
		{
			StartablePattern.Add(pair.Value);
			rate += pair.Value->CurPatternRate;
		}
	}


	//2 시작가능한 패턴 중 rate를 
	int32 randRate = FMath::RandRange(1 , rate);
	int32 accumulatedRate = 0;
	for ( UThorPattern* pattern : StartablePattern )
	{
		accumulatedRate += pattern->CurPatternRate;

		if ( randRate <= accumulatedRate )
		{
			newPattern = pattern->ThorPattern;
			break;
		}
	}


	if ( newPattern != EThorPattern::NONE )
	{
		Owner->StartPattarn(newPattern);
	}

	//EndPattern();
}

bool UThor_Idle::IsHitable()
{
	if ( IdleTime < 0.32f )
	{
		return false;
	}

	return bHitable;
}

void UThor_Idle::SetOptionValue(float Value_1, float Value_2, float Value_3)
{
	//다음 패턴 대기 시간 지정
	IdleTime = Value_1;
	//다음 패턴 실행까지 히트 애니메이션이 가능한지
	bHitable = Value_2 > 0 ? true : false;
	//임시
	//bHitable = true;
}

