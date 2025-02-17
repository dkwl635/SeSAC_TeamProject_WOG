// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 크레토스 HP 바 정보 연결하기
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* KratosHPBar;

	void SetKratosHP(float Cur, float Max);

	// 크레토스 분노모드 정보 연결하기
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* KratosRageBar;

	void SetKratosRP(float Cur, float Max);

	
};
