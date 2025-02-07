// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KratosFSM.generated.h"


UENUM(BlueprintType)
enum class EKratosState : uint8 {
	Idle UMETA(DisplayName = "대기") ,
	Move UMETA(DisplayName = "이동") ,
	Attack UMETA(DisplayName = "공격") ,
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
	// 상태 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	EKratosState mState = EKratosState::Idle;

	// 대기 상태
	void IdleState();		

	// 이동 상태
	void MoveState();

	// 공격 상태
	void AttackState();

	// 피격 상태
	void DamageState();

	// 죽음 상태
	void DieState();

		
};
