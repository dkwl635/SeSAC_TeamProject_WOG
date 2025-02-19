// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "KJW/Thor.h"
#include "Kismet/GameplayStatics.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* find = UGameplayStatics::GetActorOfClass(GetWorld() , AThor::StaticClass());
	if ( find )
	{
		Thor = Cast<AThor>(find);

		Thor->UpdateHp.BindUObject(this , &ThisClass::SetThorHp);
		Thor->UpdateStun.BindUObject(this , &ThisClass::SetThorStun);

		SetThorHp();
		SetThorStun();
	}
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

void UMainUI::SetThorHp()
{
	if ( !Thor ) return;


	float orgin = ThorHealthBar->GetPercent();

	if ( Thor->Hp <= 0 )
	{
		ThorHealthBar->SetPercent(0);
	}
	else
	{
		ThorHealthBar->SetPercent(Thor->Hp/ Thor->MaxHp);
	}

	ThorHealthBackBar->SetFillColorAndOpacity(FLinearColor::Red);
	ThorHealthBackBar->SetPercent(orgin);
	
	if ( HideHpBack.IsValid() )
	{
		GetWorld()->GetTimerManager().ClearTimer(HideHpBack);
	}

	GetWorld()->GetTimerManager().SetTimer(HideHpBack , this , &ThisClass::HideBackHp , 0.02f , true);

}

void UMainUI::SetThorStun()
{
	if ( !Thor ) return;

	if ( Thor->StunValue <= 0 )
	{
		StunBar->SetPercent(0);
	}
	else
	{
		StunBar->SetPercent(Thor->StunValue / Thor->StunMaxValue);
	}
}

void UMainUI::HideBackHp()
{
	FLinearColor CurrentColor = ThorHealthBackBar->GetFillColorAndOpacity();
	float NewAlpha = FMath::Clamp(CurrentColor.A - ( 0.02f ) , 0.0f , 1.0f);
	ThorHealthBackBar->SetFillColorAndOpacity(FLinearColor(CurrentColor.R , CurrentColor.G , CurrentColor.B , NewAlpha));


	if ( CurrentColor.A <= 0 )
	{
		if ( HideHpBack.IsValid() )
		{
			GetWorld()->GetTimerManager().ClearTimer(HideHpBack);
		}
	}
	
}
