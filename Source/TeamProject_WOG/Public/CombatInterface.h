// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

//WOG 전용 데미지 구조체
USTRUCT(BlueprintType)
struct FWOG_DamageEvent
{
	GENERATED_BODY()
public:
	//데미지 수치	
	float DamageValue = 0.0f;
	//스턴 수치
	float StunValue = 0.0f;
	//데미지 포인트
	FVector HitPoint = FVector::ZeroVector;
};

UENUM(BlueprintType)
enum class EWOG_Character_State : uint8
{
	NONE ,//아무것도 아님
	IDLE , //평소 상태
	STUN , //그로기(스턴) 상태
	GRAB , //잡힌 상태
	MAX , //아무것도 아님
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAMPROJECT_WOG_API ICombatInterface
{
	GENERATED_BODY()
protected:
	EWOG_Character_State CharacterState = EWOG_Character_State::NONE;
	
// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//현제 상태 리턴
	EWOG_Character_State GetCharacterState() { return CharacterState;}
	//상태 변경시키는 함수
	virtual void SetCharacterState(EWOG_Character_State NewState) = 0;
	//WOG 전용 데미지 주는 함수
	virtual void TakeKDamage(const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser) = 0;
	//애니메이션을 강제로 시켜야할경우
	virtual class USkeletalMeshComponent* GetSkeletalMesh() = 0;
	//뭔가 조작이 필요할떄
	virtual AActor* GetActor() = 0;
};
