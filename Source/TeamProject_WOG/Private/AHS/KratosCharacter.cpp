// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/KratosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"
#include "AHS/LeviathanAxe.h"
#include "Blueprint/UserWidget.h"
#include "KratosFSM.h"



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

	//2. 원거리 도끼 Spawn Point
	AxeSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AxeSpawnPoint"));
	AxeSpawnPoint->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKratosCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//1. 컨트롤 연결
	auto pc = Cast<APlayerController>(Controller);
	if ( pc ) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if ( subsystem ) {
			subsystem->AddMappingContext(IMC_Kratos , 0);
		}
	}

	//2. Aim UI
	AimAxeUI = CreateWidget(GetWorld(), AimAxeUIFactory);

	//3. FSM 추가
	//fsm = CreateDefaultSubobject<UKratosFSM>(TEXT("FSM"));

}

// Called every frame
void AKratosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 컨트롤러 비활성화
	if ( CharacterState == EWOG_Character_State::STUN) {
		return;
	}

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
		PlayerInput->BindAction(IA_Return , ETriggerEvent::Started , this , &AKratosCharacter::ReturnAxetoHand);

		// 2-3. 플레이어 공격 및 무기 장착
		PlayerInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &AKratosCharacter::AttackAction);
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

// Aim
void AKratosCharacter::AimAxeAttack(const FInputActionValue& inputValue)
{
	if ( Kratos_EquippedWeapon && Kratos_HasWeapon) {
		if ( AimAttackState == false ) {
			//Aim UI 활성화
			AimAxeUI->AddToViewport();

			//카메라 Zoom in
			KratosCamComp->SetFieldOfView(45.0f);

			AimAttackState = true;
			GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , TEXT("Aiming"));
		}
		else {
			//Aim UI 비활성화
			AimAxeUI->RemoveFromParent();

			//카메라 원상태 복귀
			KratosCamComp->SetFieldOfView(90.0f);

			AimAttackState = false;

			GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , TEXT("Aim Canceled"));
		}
	}
	else {
		//Aim UI 비활성화
		AimAxeUI->RemoveFromParent();

		//카메라 원상태 복귀
		KratosCamComp->SetFieldOfView(90.0f);
	}
}

void AKratosCharacter::ReturnAxetoHand(const FInputActionValue& inputValue)
{
	if ( Kratos_HasWeapon == false){
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Return_Axe_Montage);

		RecallAxe();

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

// 무기 장착 상태 판단
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
				AimAttackState = true;

				// 생성되어 날라가기
				ThrowAxe();

				/*
				// LineTrace로, 바라보는 방향의 위치까지 날아가기
				FVector startPos = KratosCamComp->GetComponentLocation();
				FVector endPos = KratosCamComp->GetComponentLocation() + KratosCamComp->GetForwardVector() * 5000.0f;	//5km(5000cm)
				FHitResult hitInfo;
				FCollisionQueryParams params;
				params.AddIgnoredActor(this);
				bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos,
																ECollisionChannel::ECC_Visibility, params);
				if ( bHit == true ) {
					FTransform t = AxeSpawnPoint->GetComponentTransform();
					GetWorld()->SpawnActor<ALeviathanAxe>(SpawnedAxe, t);




				}
				*/



			}
			// 근거리 공격
			else {
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Melee_Attack_Montage);
			}
		}
		// 주먹 공격을 한다.
		else {
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			AnimInstance->Montage_Play(Fist_Attack_Montage);
		}
	}
	else {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(Fist_Attack_Montage);
	}
}


FVector AKratosCharacter::GetAimLocation()
{
	FVector StartPos = KratosCamComp->GetComponentLocation();
	FVector EndPos = StartPos + KratosCamComp->GetForwardVector() * 5000.0f; // 50m 범위

	FHitResult HitInfo;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo , StartPos , EndPos , ECC_Visibility , Params);

	return bHit ? HitInfo.ImpactPoint : EndPos;
}

void AKratosCharacter::ThrowAxe()
{
	if ( AxeActor == nullptr )
	{
		// ✅ 소켓이 존재하는지 확인 후 가져오기
		FVector SpawnLocation = FVector::ZeroVector;
		if ( GetMesh() && GetMesh()->DoesSocketExist("Armmed") )
		{
			SpawnLocation = GetMesh()->GetSocketLocation("Armmed");
		}
		else
		{
			UE_LOG(LogTemp , Warning , TEXT("소켓 'Armmed'를 찾을 수 없습니다! 기본 위치로 설정."));
		}

		FRotator SpawnRotation = GetControlRotation();
		AxeActor = GetWorld()->SpawnActor<ALeviathanAxe>(AxeClass , SpawnLocation , SpawnRotation);
	}

	FVector TargetLocation = GetAimLocation(); // 에임한 위치
	AxeActor->ThrowAxe(TargetLocation);
}


void AKratosCharacter::RecallAxe()
{
	if ( AxeActor )
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Armmed");
		AxeActor->ReturnAxe(HandLocation);
		AxeActor = nullptr;
	}
}

//--------------------------------------------------------------------------------------
void AKratosCharacter::SetCharacterState(EWOG_Character_State NewState)
{
	CharacterState = NewState;
}

void AKratosCharacter::TakeKDamage(const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser)
{
}

USkeletalMeshComponent* AKratosCharacter::GetSkeletalMesh()
{
	return GetSkeletalMesh();
}

AActor* AKratosCharacter::GetActor()
{
	return this;
}


