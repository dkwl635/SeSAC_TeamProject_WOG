// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ThorPattern.h"
#include "KJW/Thor.h"

void UThorPattern::InitPattern(AThor* Thor)
{
	if ( !Thor ) { return; }
	
	Owner = Thor;
}

void UThorPattern::StartPattern()
{
	StartPattern_C();
	StartPattern_BP();
	
}

void UThorPattern::StopPattern()
{
	StopPattern_C();
	StopPattern_BP();
}

void UThorPattern::EndPattern()
{
	EndPattern_C();
	EndPattern_BP();
}

void UThorPattern::TickPattern()
{
	
	if ( TickPattern_C() == false )
	{
		return;
	}

	TickPattern_BP();
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
