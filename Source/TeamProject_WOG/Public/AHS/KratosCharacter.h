// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

#include "CombatInterface.h"

#include "TeamProject_WOG/TeamProject_WOG.h"

#include "KratosCharacter.generated.h"

UCLASS()
class TEAMPROJECT_WOG_API AKratosCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKratosCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// 0. Properties
public:
	// HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float Health = 100.0f;

	// 분노 게이지(Rage meter)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float Rage = 100.0f;

// 2. Camera
public:
	UPROPERTY( EditDefaultsOnly , Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* KratosCamComp;
	
// Camera Input
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Kratos;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY( EditDefaultsOnly , Category = "Input" )
	class UInputAction* IA_Turn;

	void Turn(const FInputActionValue& inputValue);

	void LookUp(const FInputActionValue& inputValue);

// 3. Movement Input
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	// 이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float WalkSpeed = 600.0f;

	// 이동 방향
	FVector Direction;

	void Move(const FInputActionValue& inputValue);

// 4. 무기 장착
private:
	bool AimAttackState = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Aim;

	void AimAxeAttack(const FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Return;

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Return_Axe_Montage;

	void ReturnAxetoHand(const FInputActionValue& inputValue);

private:
	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Equip_Axe_Montage;


	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Attack_Axe_Montage;

	bool Temp = true;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sheath_UnSheath;

	void SheathAction(const FInputActionValue& inputValue);

	// Default Settings
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	bool Kratos_HasWeapon = true;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	bool Kratos_EquippedWeapon = false;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* UnEquip_Axe_Montage;

public:
	UFUNCTION(BlueprintPure)
	bool Get_KratosEquippedWeapon() const;

//5. 공격
	//도끼 원거리 공격
	UPROPERTY(EditDefaultsOnly , Category = "Input")
	class UInputAction* IA_Weapon;

	void AttackAction(const FInputActionValue& inputValue);

	// 도끼 근접 공격
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* Melee_Attack_Montage;

	// 근접 공격(주먹)
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* Fist_Attack_Montage;

	// ICombatInterface을(를) 통해 상속됨
	void SetCharacterState(EWOG_Character_State NewState) override;
	void TakeKDamage(const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser) override;
	USkeletalMeshComponent* GetSkeletalMesh() override;
	AActor* GetActor() override;
};
