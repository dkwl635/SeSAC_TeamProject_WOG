// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_Move.h"

void UThor_Move::StartPattern_C()
{
	Owner->SetIsMove(true);
	UE_LOG(LogTemp, Warning, TEXT("StartPattern_C_MOVE"));
}

void UThor_Move::StopPattern_C()
{
	Owner->SetIsMove(false);
}

void UThor_Move::EndPattern_C()
{
	Owner->SetIsMove(false);
}

bool UThor_Move::TickPattern_C()
{
	return false;
}

