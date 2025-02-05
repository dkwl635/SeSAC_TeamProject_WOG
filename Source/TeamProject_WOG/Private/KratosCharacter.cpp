// Fill out your copyright notice in the Description page of Project Settings.


#include "KratosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

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
		PlayerInput->BindAction(IA_Weapon, ETriggerEvent::Completed, this, &AKratosCharacter::EquipAction);
		PlayerInput->BindAction(IA_UnequipWeapon , ETriggerEvent::Completed , this , &AKratosCharacter::UnEquipAction);
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

// 무기 장착
void AKratosCharacter::EquipAction(const FInputActionValue& inputValue)
{
	if ( Kratos_HasWeapon == true ) {
		if ( Kratos_EquippedWeapon == true ) {
			AttackAction();
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

// 공격
void AKratosCharacter::AttackAction()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Attack_Axe_Montage);
}

// 무기 장착 해제
void AKratosCharacter::UnEquipAction(const FInputActionValue& inputValue)
{
	if ( Kratos_HasWeapon == true ) {
		if ( Kratos_EquippedWeapon == true ) {
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(UnEquip_Axe_Montage);
			Kratos_EquippedWeapon = false;
		}
	}
}

bool AKratosCharacter::Get_KratosEquippedWeapon() const
{
	return Kratos_EquippedWeapon;
}



