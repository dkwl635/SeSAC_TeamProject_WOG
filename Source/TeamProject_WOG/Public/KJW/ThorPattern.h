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
    UPROPERTY(BlueprintReadWrite)
    AThor* Owner;
public:
    UPROPERTY(EditAnywhere)
    EThorPattern ThorPattern = EThorPattern::NONE;
public:
    virtual void InitPattern(class AThor* Thor);
public:
    void StartPattern(); //시작시 호출되는
    void StopPattern(); //중단시 호출되는
    void EndPattern();  //정상 종료시 호출되는
    void TickPattern(); //매 틱마다 호출
    
    void NotifyEventPattern(int32 EventIndex);    
    void NotifyBeginPattern(int32 EventIndex, float TotalDuration);
    void NotifyEndPattrern(int32 EventIndex);
    void NotifyTickPattrern(int32 EventIndex, float FrameDeltaTime);

protected:
    //FUNCTION -> C++ -> Blueprint
    virtual void StartPattern_C();
    virtual void StopPattern_C();
    virtual void EndPattern_C();
    virtual bool TickPattern_C();

    virtual void NotifyEventPattern_C(int32 EventIndex);
    virtual void NotifyBeginPattern_C(int32 EventIndex , float TotalDuration);
    virtual void NotifyEndPattrern_C(int32 EventIndex);
    virtual void NotifyTickPattrern_C(int32 EventIndex, float FrameDeltaTime);

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void StartPattern_BP();
    
    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void StopPattern_BP();
   
    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void EndPattern_BP();
    
    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void TickPattern_BP();


    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void NotifyEventPattern_BP(int32 EventIndex);

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void NotifyBeginPattern_BP(int32 EventIndex, float TotalDuration);

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void NotifyEndPattrern_BP(int32 EventIndex);

    UFUNCTION(BlueprintNativeEvent , Category = "ThorPattern")
    void NotifyTickPattrern_BP(int32 EventIndex , float FrameDeltaTime);

private:
     void StartPattern_BP_Implementation();
     void StopPattern_BP_Implementation();
     void EndPattern_BP_Implementation();
     void TickPattern_BP_Implementation();

     void NotifyEventPattern_BP_Implementation(int32 EventIndex);
     void NotifyBeginPattern_BP_Implementation(int32 EventIndex , float TotalDuration);
     void NotifyEndPattrern_BP_Implementation(int32 EventIndex);
     void NotifyTickPattrern_BP_Implementation(int32 EventIndex, float FrameDeltaTime);
};

