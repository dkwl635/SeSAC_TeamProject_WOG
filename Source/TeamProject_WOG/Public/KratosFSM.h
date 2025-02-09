// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AHS/KratosCharacter.h"
#include "KratosFSM.generated.h"


UENUM(BlueprintType)
enum class EKratosState : uint8 {
	Idle UMETA(DisplayName = "대기") ,
	Move UMETA(DisplayName = "이동") ,
	Sheath UMETA(DisplayName = "무기 장착") ,
	Attack UMETA(DisplayName = "공격") ,
	AxeMeleeAttack UMETA(DisplayName = "도끼 근거리 공격") ,
	AxeRangedAttack UMETA(DisplayName = "도끼 원거리 공격") ,
	Damage UMETA(DisplayName = "데미지") ,
	Die UMETA(DisplayName = "죽음") ,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_WOG_API UKratosFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKratosFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:


	// 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EKratosState mState = EKratosState::Idle;

	// 무기를 가지고 있는지 여부
	bool bHasAxe;

	// 무기 장착 여부
	bool bSheathAxe;

	// 공격이 끝났는지 여부
	bool bAttackFinished = false;

	// 피해를 받은 후, 상태를 회복했는지 여부
	bool bDamageRecovered = false;

	// 현재 FSM를 사용하는 캐릭터
	class AKratosCharacter* KratosCharacter;

	void SetState(EKratosState NewState);

	// 대기 상태
	void IdleState();		

	// 이동 상태
	void MoveState();

	// 무기 장착 상태
	void EquipAxe();

	//-------------------------------------------
	// 공격 상태
	void AttackState();

	// 도끼 장착
	// 공격 상태
	void AxeMeleeAttackState();

	// 공격 상태
	void AxeRangedAttackState();

	//-----------------------------------------
	// 피격 상태
	void DamageState();

	// 죽음 상태
	void DieState();

		
	//===========================================
	void UpdateWeaponState(bool bNewHasAxe);
	void PlayPunchAttack();
	bool IsAiming();

};
