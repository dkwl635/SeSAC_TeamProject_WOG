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

	// TPS 카메라를 붙이고 싶다.
	// SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f , 60.0f , 80.0f));
	springArmComp->TargetArmLength = 300.0f;

	//카메라 회전(02/04(화))
	springArmComp->bUsePawnControlRotation = true;

	// Camera 컴포넌트 붙이기
	KratosCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("KratosCamComp"));
	KratosCamComp->SetupAttachment(springArmComp);

	//카메라 회전(02/04(화))
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
		PlayerInput->BindAction(IA_Turn , ETriggerEvent::Triggered , this , &AKratosCharacter::Turn);
		PlayerInput->BindAction(IA_LookUp , ETriggerEvent::Triggered , this , &AKratosCharacter::LookUp);
		PlayerInput->BindAction(IA_Move , ETriggerEvent::Triggered , this , &AKratosCharacter::Move);
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

