// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainUI::SetKratosHP(float Cur , float Max)
{

	if( KratosHPBar == nullptr )	 return;
	KratosHPBar->SetPercent(Cur/Max);
}

void UMainUI::SetKratosRP(float Cur , float Max)
{
	if ( KratosRageBar == nullptr )	 return;
	KratosRageBar->SetPercent(Cur / Max);
}
