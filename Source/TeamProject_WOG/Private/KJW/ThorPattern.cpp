// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ThorPattern.h"
#include "KJW/Thor.h"

void UThorPattern::InitPattern(AThor* Thor)
{
	if ( !Thor ) { return; }
	
	Owner = Thor;
	//임시 맵 중앙 포인트 나중에 ? 게임모드나 그런쪽에서 셋팅 아니면 토르에 넣어놓기
	MapPoint = Thor->GetActorLocation();
	MapSize = Thor->MapSize;
}

void UThorPattern::StartPattern()
{
	StartPattern_C();
	StartPattern_BP();
	StartPattern_CBP();
	
}

void UThorPattern::StopPattern()
{
	StopPattern_C();
	StopPattern_BP();
	StopPattern_CBP();
}

void UThorPattern::EndPattern()
{
	EndPattern_C();
	EndPattern_BP();
	EndPattern_CBP();
}

void UThorPattern::TickPattern()
{
	
	if ( TickPattern_C() == false )
	{
		return;
	}

	TickPattern_BP();
	TickPattern_CBP();
}

void UThorPattern::NotifyEventPattern(int32 EventIndex)
{
	NotifyEventPattern_C(EventIndex);
	NotifyEventPattern_BP(EventIndex);
}

void UThorPattern::NotifyBeginPattern(int32 EventIndex ,float TotalDuration)
{
	NotifyBeginPattern_C(EventIndex , TotalDuration);
	NotifyBeginPattern_BP(EventIndex , TotalDuration);
}


void UThorPattern::NotifyEndPattrern(int32 EventIndex)
{
	NotifyEndPattrern_C(EventIndex);
	NotifyEndPattrern_BP(EventIndex);
}

void UThorPattern::NotifyTickPattrern(int32 EventIndex , float FrameDeltaTime)
{
	NotifyTickPattrern_C(EventIndex, FrameDeltaTime);
	NotifyTickPattrern_BP(EventIndex,  FrameDeltaTime);
}

void UThorPattern::StartPattern_CBP()
{
}

void UThorPattern::StopPattern_CBP()
{
}

void UThorPattern::EndPattern_CBP()
{
	//End Pattern CBP
	Owner->StartPattarn(EThorPattern::IDLE);
}

void UThorPattern::TickPattern_CBP()
{
}

void UThorPattern::StartPattern_C()
{
}

void UThorPattern::StopPattern_C()
{
}

void UThorPattern::EndPattern_C()
{
}

bool UThorPattern::TickPattern_C()
{
	return true;
}

void UThorPattern::NotifyEventPattern_C(int32 EventIndex)
{
}

void UThorPattern::NotifyBeginPattern_C(int32 EventIndex, float TotalDuration)
{
}

void UThorPattern::NotifyEndPattrern_C(int32 EventIndex)
{
}

void UThorPattern::NotifyTickPattrern_C(int32 EventIndex, float FrameDeltaTime)
{
}

void UThorPattern::StartPattern_BP_Implementation()
{
}

void UThorPattern::StopPattern_BP_Implementation()
{
}

void UThorPattern::EndPattern_BP_Implementation()
{
}

void UThorPattern::TickPattern_BP_Implementation()
{
}

void UThorPattern::NotifyEventPattern_BP_Implementation(int32 EventIndex)
{
}

void UThorPattern::NotifyBeginPattern_BP_Implementation(int32 EventIndex, float TotalDuration)
{
}

void UThorPattern::NotifyEndPattrern_BP_Implementation(int32 EventIndex)
{
}

void UThorPattern::NotifyTickPattrern_BP_Implementation(int32 EventIndex, float FrameDeltaTime)
{
}

void UThorPattern::SetOptionValue(float Value_1, float Value_2, float Value_3)
{
}

