// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/KratosCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Components/ArrowComponent.h"
#include "AHS/LeviathanAxe.h"
#include "Blueprint/UserWidget.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "KJW/Thor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"



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

	// 손에 붙일 도끼 만들기
	AxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AxeMesh"));

	AxeMesh->SetupAttachment(GetMesh(), TEXT("Unarmmed"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempAxeMesh(TEXT("/Script/Engine.StaticMesh'/Game/AHS/Assets/Leviathan_Axe/Axe.Axe'"));
	if ( TempAxeMesh.Succeeded() ) {
		AxeMesh->SetStaticMesh(TempAxeMesh.Object);
	}
	AxeMesh->SetRelativeScale3D(FVector(0.14f));

	// 도끼 충돌 처리 해보자.
	AxeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AxeCollision"));
	AxeCollision->SetupAttachment(AxeMesh);
	AxeCollision->SetCollisionProfileName(TEXT("PlayerAttack"));
	
	AxeCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));
	AxeCollision->SetRelativeScale3D(FVector(0.3f, 0.15f, 0.25f));
	
	//------------------------------------------------------------
	// 3. 주먹 콜리전 구현
	Fist_R = CreateDefaultSubobject<USphereComponent>(TEXT("Fist_R"));
	Fist_R->SetRelativeScale3D(FVector(0.05f));
	Fist_R->SetupAttachment(GetMesh() , FName(TEXT("Hand_R")));
	Fist_R->SetCollisionProfileName(TEXT("PlayerAttack"));
	

	Fist_L = CreateDefaultSubobject<USphereComponent>(TEXT("Fist_L"));
	Fist_L->SetRelativeScale3D(FVector(0.05f));
	Fist_L->SetupAttachment(GetMesh(), FName(TEXT("Hand_L")));
	Fist_L->SetCollisionProfileName(TEXT("PlayerAttack"));
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

	//2. Aim UI + Main UI
	AimAxeUI = CreateWidget(GetWorld(), AimAxeUIFactory);
	MainUI = CreateWidget(GetWorld(), MainUIFactory);
	MainUI->AddToViewport();

	//3. Anim Notify 구현
	//GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AKratosCharacter::AxeComboNotify);

	//4. 주먹 Collider 꺼두기
	FistCollision(false);
	OnAxeCollision(false);

}

// Called every frame
void AKratosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnAxeCollision(false);
	FistCollision(false);

	// 컨트롤러 비활성화
	if ( CharacterState == EWOG_Character_State::STUN) {
		return;
	}

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);

	// 크레토스 이동
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);

	switch ( mState ) {
		case EKratosState::Idle: { } break;
		case EKratosState::Move: { 	//Idle 상태로 초기화
			if ( Direction == FVector::ZeroVector ) 
			{	mState = EKratosState::Idle;	}
		} break;
		case EKratosState::Attack: { 
			Direction = FVector::ZeroVector; 

			currentTime += GetWorld()->DeltaTimeSeconds;
			if ( currentTime >= 1.5f ) {
				mState = EKratosState::Idle;
				currentTime = 0.0f;
				ClickOnce = false;
				//FistCollision(false);
				//OnAxeCollision(false);
			}
		
		} break;
		case EKratosState::Damage: { } break;
		case EKratosState::Die: { } break;
	}

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

}


// 입력 연결
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

		//대쉬
		PlayerInput->BindAction(IA_Dash , ETriggerEvent::Started , this , &AKratosCharacter::DashInput);
		PlayerInput->BindAction(IA_Dash , ETriggerEvent::Completed , this , &AKratosCharacter::DashInput);

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

// 크레토스 HP
void AKratosCharacter::GetKratosHP()
{

}

void AKratosCharacter::SetKratosHP()
{

}

//--------------------------------------------------------
//카메라 X 회전
void AKratosCharacter::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();

	float AdjustedValue = value * HorizontalRotationSpeed;

	AddControllerYawInput(AdjustedValue);
}

//카메라 Y 회전
void AKratosCharacter::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();

	float AdjustedValue = value * VerticalRotationSpeed;

	// 현재 카메라의 Pitch 값을 얻어옴
	float CurrentPitch = GetControlRotation().Pitch;

	// 새로운 Pitch 값을 계산
	float NewPitch = CurrentPitch + AdjustedValue;

	// 0도에서 30도까지 제한
	if ( NewPitch < 0.0f )
	{
		NewPitch = 0.0f;
	}
	else if ( NewPitch > 30.0f )
	{
		NewPitch = 30.0f;
	}

	// GetController()를 통해 PlayerController 얻기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if ( PlayerController )
	{
		// 제한된 Pitch 값을 적용, Yaw는 그대로
		FRotator NewRotation = FRotator(NewPitch , GetControlRotation().Yaw , 0.0f);
		PlayerController->SetControlRotation(NewRotation);
	}
}

// 플레이어 이동
void AKratosCharacter::Move(const FInputActionValue& inputValue)
{
	if ( mState == EKratosState::Attack ) {

	}
	else {
		mState = EKratosState::Move;

		FVector2D value = inputValue.Get<FVector2D>();

		// 상하 입력 처리
		Direction.X = value.X;

		// 좌우 입력 처리
		Direction.Y = value.Y;
	}
}

// Aim
void AKratosCharacter::AimAxeAttack(const FInputActionValue& inputValue)
{
	if ( Kratos_EquippedWeapon && Kratos_HasWeapon) {
		if ( AimAttackState == false ) {
			IsAiming(true);
		}
		else {
			IsAiming(false);
		}
	}
	else {
		IsAiming(false);
	}
}

void AKratosCharacter::IsAiming(bool bIsAiming)
{
	if ( bIsAiming ) {
		mState = EKratosState::AxeAim;

		//Aim UI 활성화
		AimAxeUI->AddToViewport();

		//카메라 Zoom in
		KratosCamComp->SetFieldOfView(45.0f);

		GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , TEXT("Aiming"));
	}
	else {
		mState = EKratosState::AxeUnAim;

		//Aim UI 비활성화
		AimAxeUI->RemoveFromParent();

		//카메라 원상태 복귀
		KratosCamComp->SetFieldOfView(90.0f);

		GEngine->AddOnScreenDebugMessage(-1 , 2.0f , FColor::Red , TEXT("Aim Canceled"));
	}

	AimAttackState = bIsAiming;
}

//--------------------------------------------------------
// 던진 무기 돌려받기
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

//--------------------------------------------------------
// 공격
void AKratosCharacter::AttackAction(const FInputActionValue& inputValue)
{
	mState = EKratosState::Attack;

	/*
	if ( ClickOnce == true ) {
		return;
	}
	*/

	//무기가 있다.
	if ( Kratos_HasWeapon == true ) {

		//무기를 손에 들고 있다.
		if ( Kratos_EquippedWeapon == true ) {
			// 원거리 공격
			if ( AimAttackState == true ) {
				ClickOnce = true;

				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				AnimInstance->Montage_Play(Attack_Axe_Montage);

				// 무기는 없다.(날아갔으므로)
				Kratos_HasWeapon = false;

				// 조준 상태에서 벗어난다.
				AimAttackState = true;

				// 생성되어 날라가기
				ThrowAxe();

			}
			// 근거리 공격(도끼)
			else {

				/*
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				
				// 현재 콤보 몬타주를 재생하고 있지 않다면,
				
				if ( !( AnimInstance->Montage_IsPlaying(AxeCombo_Montage) ) ) {
					AnimInstance->Montage_Play(AxeCombo_Montage);
					
				}
				*/
				
				
				//OnAxeCollision(true);
				ClickOnce = true;

				//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				//AnimInstance->Montage_Play(Melee_Attack_Montage);
			}
		}
		// 주먹 공격을 한다.
		else {
			FistCollision(true);
			ClickOnce = true;

			//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			//AnimInstance->Montage_Play(Fist_Attack_Montage);			
		}
	}
	else {
		FistCollision(true);
		ClickOnce = true;

		// 일반 주먹 공격
		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//AnimInstance->Montage_Play(Fist_Attack_Montage);
	}
}

void AKratosCharacter::FistCollision(bool bValue)
{
	//Collision 켜기
	if ( bValue ) {
		Fist_R->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Fist_L->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	//Collision 끄기
	else {
		Fist_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Fist_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AKratosCharacter::OnAxeCollision(bool bValue)
{
	if ( bValue ) {
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	//Collision 끄기
	else {
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//----------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------
// 원거리 무기 공격
// 도끼 던지기
void AKratosCharacter::ThrowAxe()
{
	if ( AxeActor == nullptr )
	{
		// 소켓이 존재하는지 확인 후 가져오기
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

// 도끼 돌려받기
void AKratosCharacter::RecallAxe()
{
	if ( AxeActor )
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Armmed");
		AxeActor->ReturnAxe(HandLocation);
		AxeActor = nullptr;
	}
}

//----------------------------------------------------------------------------------
// 충돌 처리(Collision)
void AKratosCharacter::OnHandOverlapBP(AActor* OtherActor , FVector SweepResult)
{
	//FistCollision(true);
	//OnAxeCollision(true);

	AThor* thor = Cast<AThor>(OtherActor);
	if ( thor != nullptr ) {
		FString logMsg = TEXT("Thor Take Damage!");
		GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);

		UE_LOG(LogTemp , Warning , TEXT("Thor"));

		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = SweepResult;

		thor->TakeKDamage(DamageData , this);
	}
}


void AKratosCharacter::DashInput()
{
		auto movement = GetCharacterMovement();
		if ( movement == nullptr ) {
			return;
		}

		// 현재 달리기 모드라면,
		if ( movement->MaxWalkSpeed > WalkSpeed ) {
			//걷기 모드로 바꾼다.
			movement->MaxWalkSpeed = WalkSpeed;
		}
		// 걷기 모드라면
		else {
			//달리기 모드로 바꾼다.
			movement->MaxWalkSpeed = RunSpeed;
		}
}

//--------------------------------------------------------------------------------------
// 공통 데미지 주고 받기
void AKratosCharacter::SetCharacterState(EWOG_Character_State NewState)
{
	CharacterState = NewState;
}

void AKratosCharacter::TakeKDamage(const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser)
{
	FString logMsg = TEXT("Kratos Take Damage!");
	GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);
	UE_LOG(LogTemp , Warning , TEXT("Kratos Take Damage!"));
}

USkeletalMeshComponent* AKratosCharacter::GetSkeletalMesh()
{
	return GetSkeletalMesh();
}

AActor* AKratosCharacter::GetActor()
{
	return this;
}

//=======================================================================
//WIP

void AKratosCharacter::AxeComboNotify(FName AxeCombo , const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	FString logMsg = TEXT("Kratos Axe Notify!");
	GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);
}




