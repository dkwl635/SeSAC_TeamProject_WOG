// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TeamProject_WOG/TeamProject_WOG.h"
#include "KJW/Thor.h"
#include "ThorPattern.generated.h"


UCLASS(Blueprintable)
class TEAMPROJECT_WOG_API UThorPattern : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    AThor* Owner;
public:
    UPROPERTY(EditAnywhere)
    EThorPattern ThorPattern = EThorPattern::NONE;
public:
    void InitPattern(AThor* Thor);
public:
    void StartPattern(); //시작시 호출되는
    void StopPattern(); //중단시 호출되는
    void EndPattern();  //정상 종료시 호출되는
    void TickPattern(); //매 틱마다 호출

protected:
    //FUNCTION -> C++ -> Blueprint
    virtual void StartPattern_C();
    virtual void StopPattern_C();
    virtual void EndPattern_C();
    virtual bool TickPattern_C();


    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void StartPattern_BP();
    virtual void StartPattern_BP_Implementation();
   
    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void StopPattern_BP();
    virtual void StopPattern_BP_Implementation();

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void EndPattern_BP();
    virtual void EndPattern_BP_Implementation(); 

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void TickPattern_BP();
    virtual void TickPattern_BP_Implementation(); 

    
};

