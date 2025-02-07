// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/KratosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

#include "CombatInterface.h"
#include "TeamProject_WOG/TeamProject_WOG.h"

// Sets default values
AKratosCharacter::AKratosCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 카메라 컴포넌트
	// SpringArm 컴포넌트
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f , 60.0f , 80.0f));
	springArmComp->TargetArmLength = 300.0f;

	springArmComp->bUsePawnControlRotation = true;

	// Camera 컴포넌트
	KratosCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("KratosCamComp"));
	KratosCamComp->SetupAttachment(springArmComp);

	KratosCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AKratosCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);
	if ( pc ) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if ( subsystem ) {
			subsystem->AddMappingContext(IMC_Kratos , 0);
		}
	}
}

// Called every frame
void AKratosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 크레토스 이동
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void AKratosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Cast Vs. CastChecked: null이면 강제 종료.
	auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if ( PlayerInput )
	{
		// 1. 플레이어 이동 및 카메라
		PlayerInput->BindAction(IA_Turn , ETriggerEvent::Triggered , this , &AKratosCharacter::Turn);
		PlayerInput->BindAction(IA_LookUp , ETriggerEvent::Triggered , this , &AKratosCharacter::LookUp);
		PlayerInput->BindAction(IA_Move , ETriggerEvent::Triggered , this , &AKratosCharacter::Move);

		// 2. 플레이어 공격 및 무기 장착
		// 2-1. 플레이어 에임
		PlayerInput->BindAction(IA_Aim , ETriggerEvent::Started , this , &AKratosCharacter::AimAxeAttack);
		PlayerInput->BindAction(IA_Aim , ETriggerEvent::Completed, this , &AKratosCharacter::AimAxeAttack);

		// 2-2. 무기 돌려받기(원거리)
		PlayerInput->BindAction(IA_Return , ETriggerEvent::Completed , this , &AKratosCharacter::ReturnAxetoHand);

		// 2-3. 플레이어 공격 및 무기 장착
		PlayerInput->BindAction(IA_Weapon, ETriggerEvent::Completed, this, &AKratosCharacter::AttackAction);
		PlayerInput->BindAction(IA_Sheath_UnSheath, ETriggerEvent::Started, this , &AKratosCharacter::SheathAction);
	}

}

void AKratosCharacter::Turn(const FInputActionValue& inputValue) {
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void AKratosCharacter::LookUp(const FInputActionValue& inputValue) {
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void AKratosCharacter::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	// 상하 입력 처리
	Direction.X = value.X;

	// 좌우 입력 처리
	Direction.Y = value.Y;
}

void AKratosCharacter::AimAxeAttack(const FInputActionValue& inputValue)
{
	if ( AimAttackState == false ) {
		AimAttackState = true;
	}
	else {
		AimAttackState = false;
	}
}

void AKratosCharacter::ReturnAxetoHand(const FInputActionValue& inputValue)
{
	if ( Kratos_HasWeapon == false){
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Return_Axe_Montage);

		Kratos_HasWeapon = true;
	}
}

// 무기 장착
void AKratosCharacter::SheathAction(const FInputActionValue& inputValue)
{
	if ( Kratos_HasWeapon == true ) {
		if ( Kratos_EquippedWeapon == true ) {
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(UnEquip_Axe_Montage);

			Kratos_EquippedWeapon = false;
		}
		else {
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Equip_Axe_Montage);

			Kratos_EquippedWeapon = true;
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , TEXT("No Weapon"));
	}
}

bool AKratosCharacter::Get_KratosEquippedWeapon() const
{
	return Kratos_EquippedWeapon;
}



// 공격
void AKratosCharacter::AttackAction(const FInputActionValue& inputValue)
{
	//무기가 있다.
	if ( Kratos_HasWeapon == true ) {

		//무기를 손에 들고 있다.
		if ( Kratos_EquippedWeapon == true ) {
			// 원거리 공격
			if ( AimAttackState == true ) {
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Attack_Axe_Montage);

				// 무기는 없다.(날아갔으므로)
				Kratos_HasWeapon = false;
				
				// 조준 상태에서 벗어난다.
				AimAttackState = false;
			}
			// 근거리 공격
			else {

			}
		}
		// 주먹 공격을 한다.
		else {

		}
	}
}


