// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "KratosCharacter.generated.h"

UCLASS()
class TEAMPROJECT_WOG_API AKratosCharacter : public ACharacter
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

// 2. Camera
public:
	UPROPERTY( EditDefaultsOnly , Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
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

// 4. Weapon & Attack Input
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Weapon;

	void EquipAction(const FInputActionValue& inputValue);
	void AttackAction();

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Equip_Axe_Montage;

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Attack_Axe_Montage;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_UnequipWeapon;

	void UnEquipAction(const FInputActionValue& inputValue);

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
};
