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
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

#include "MainUI.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Components/SceneComponent.h"
#include "AHS/ItemSpawnManager.h"
#include "AHS/Item.h"

#include "AHS/Temp_GameMode.h"




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

	//--------------------------------------------------------------
	// 4. 방패 추가
	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShieldMesh"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempShield(TEXT("/Script/Engine.SkeletalMesh'/Game/AxeAndShield/Meshes/SK_Shield.SK_Shield'"));

	if ( TempShield.Succeeded() ) {
		ShieldMesh->SetSkeletalMesh(TempShield.Object);
		ShieldMesh->SetupAttachment(GetMesh(), FName(TEXT("Shield")));
		ShieldMesh->SetRelativeScale3D(FVector(0.1f));
		ShieldMesh->SetVisibility(false);
	}

	ShieldCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(ShieldMesh);

	ShieldCollision->SetCollisionProfileName(TEXT("PlayerShield"));
	ShieldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//--------------------------------------------------------------
	// 5. 현재 사용하는 아이템
	CurrentItem = nullptr;

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

	//2. Aim UI + Main UI, RageMode 생성
	AimAxeUI = CreateWidget(GetWorld(), AimAxeUIFactory);
	RageModeUI = CreateWidget(GetWorld() , RageModeUIFactory);

	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIFactory);
	MainUI->AddToViewport();

	if(MainUI == nullptr ) return;

	MainUI->SetKratosHP(CurrentHealth, MaxHealth);
	MainUI->SetKratosRP(CurrentRage, MaxRage);

	//3. Anim Notify 구현
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if ( AnimInstance )
	{
		UE_LOG(LogTemp , Warning , TEXT("Animation Instance Found! Binding Notify"));
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this , &AKratosCharacter::OnAttackNotify);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this , &AKratosCharacter::OnShieldNotifyBegin);

	}
	else
	{
		UE_LOG(LogTemp , Error , TEXT("No Animation Instance Found!"));
	}


	//4. 주먹 Collider 꺼두기
	FistCollision(false);
	OnAxeCollision(false);

	//--------------------------------------------------------------
	// 5. 방패 숨기기
	bShieldVisible = false;


}



// Called every frame
void AKratosCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//-------------------------------------------------------------
	OnAxeCollision(false);
	FistCollision(false);

	// 컨트롤러 비활성화
	if ( CharacterState == EWOG_Character_State::STUN) {
		return;
	}


	//=============================================================
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
		case EKratosState::Rage: { 
			Direction = FVector::ZeroVector;

			currentTime += GetWorld()->DeltaTimeSeconds;
			if ( currentTime >= 1.5f ) {
				mState = EKratosState::Idle;
				currentTime = 0.0f;
			}
		} break;
		case EKratosState::Damage: { 
			Direction = FVector::ZeroVector;

			currentTime += GetWorld()->DeltaTimeSeconds;
			if ( currentTime >= 1.5f ) {
				mState = EKratosState::Idle;
				currentTime = 0.0f;
			}
		
		} break;
		case EKratosState::Die: { } break;
	}

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;

	// Camera Lock On Update
	if ( bIsLockedOn )
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		AThor* Thor = Cast<AThor>(UGameplayStatics::GetActorOfClass(GetWorld() , AThor::StaticClass()));

		if ( PlayerController && Thor )
		{
			FRotator LookAtRotation = ( Thor->GetActorLocation() - GetActorLocation() ).Rotation();
			PlayerController->SetControlRotation(FRotator(0.0f , LookAtRotation.Yaw , 0.0f)); // Pitch는 0으로 고정
		}
	}

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

		//camera lock on 구현
		PlayerInput->BindAction(IA_LockOn , ETriggerEvent::Triggered , this , &AKratosCharacter::LockOnTarget);

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

		//3. 다른 행동
		PlayerInput->BindAction(IA_Block , ETriggerEvent::Started , this , &AKratosCharacter::BlockAttack);
		PlayerInput->BindAction(IA_Block , ETriggerEvent::Completed , this , &AKratosCharacter::BlockAttackEnd);

		// 4. 분노 모드
		PlayerInput->BindAction(IA_RageMode , ETriggerEvent::Started , this , &AKratosCharacter::RageModeAction);

		// 5. 아이템 사용
		PlayerInput->BindAction(IA_ItemUse , ETriggerEvent::Started , this , &AKratosCharacter::UseItemAction);
	}

}

// 크레토스 HP
float AKratosCharacter::GetKratosHP()
{
	return CurrentHealth;
}

void AKratosCharacter::SetKratosHP(float hp)
{
	CurrentHealth += hp;
}

//--------------------------------------------------------
#pragma region 카메라 관련 설정(Camera)
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

	// Pitch 값을 -180 ~ 180 범위로 정규화
	CurrentPitch = FRotator::NormalizeAxis(CurrentPitch);

	// 새로운 Pitch 값을 계산하고 -20 ~ 30도로 제한
	SavedPitch = FMath::Clamp(CurrentPitch + AdjustedValue , -20.0f , 30.0f);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ( PlayerController )
	{
		// 제한된 Pitch 값을 적용, Yaw는 그대로
		FRotator NewRotation = FRotator(SavedPitch , GetControlRotation().Yaw , 0.0f);
		PlayerController->SetControlRotation(NewRotation);
	}
}

void AKratosCharacter::CameraAimRotation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ( PlayerController )
	{
		// 기존의 SavedPitch 값을 그대로 사용하여 바라보던 각도 유지
		FRotator NewRotation = FRotator(SavedPitch , GetControlRotation().Yaw , 0.0f);
		PlayerController->SetControlRotation(NewRotation);
	}
}

#pragma endregion

//----------------------------------------------------------------------------
// 플레이어 이동
void AKratosCharacter::Move(const FInputActionValue& inputValue)
{
	if ( mState == EKratosState::Attack || mState == EKratosState::Damage) {
		return;
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
	CameraAimRotation();

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
	//분노 모드일 때는 장착할 수 없음
	if ( bRageMode == true ) {
		return;
	}

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
//Anim Montage 들고오기
void AKratosCharacter::OnAttackNotify(FName NotifyName , const FBranchingPointNotifyPayload& BranchingPointPayload)
{
/*
	if ( NotifyName == "PrintString" )
	{
		// 여기에 무기 충돌 활성화 로직 추가
		FString logMsg = TEXT("Axe Collision Enabled!");
		GEngine->AddOnScreenDebugMessage(0 , 1 , FColor::Red , logMsg);

		UE_LOG(LogTemp , Warning , TEXT("Axe Collision Enabled!"));
		AxeCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	*/

	UE_LOG(LogTemp , Warning , TEXT("OnAttackNotify Triggered: %s") , *NotifyName.ToString());
}

//---------------------------------------------------------
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
				//ThrowAxe();

			}
			// 근거리 공격(도끼)
			else {
				//카메라 셰이크 재생
				//auto controller = GetWorld()->GetFirstPlayerController();
				//controller->PlayerCameraManager->StartCameraShake(CameraShake);
			}
		}
		// 주먹 공격을 한다.
		else {	
		}
	}
	else {

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
	AThor* thor = Cast<AThor>(OtherActor);
	if ( thor != nullptr ) {

		FWOG_DamageEvent DamageData;

		// 분노 모드 판별
		if ( bRageMode == false ) {
			AddRage(10);
			DamageData.DamageValue = 10;
		}
		else {
			DamageData.DamageValue = 30;
		}
		
		DamageData.HitPoint = SweepResult;

		thor->TakeKDamage(DamageData , this);

		//카메라 셰이크 재생
		auto controller = GetWorld()->GetFirstPlayerController();
		controller->PlayerCameraManager->StartCameraShake(CameraShake);

		// VFX 생성(피 효과)
		/*
		UParticleSystemComponent* SpawnedVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , BloodVFX , AxeCollision->GetComponentTransform());

		// 1초 후 삭제 예약
		FTimerHandle TimerHandle_Blood;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Blood , [SpawnedVFX]()
		{
		if ( SpawnedVFX )
		{
			SpawnedVFX->DestroyComponent();
		}
		} , 1.0f , false);
		*/
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

	OnKratosDamageBP();
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
// Lock On Camera 구현
void AKratosCharacter::LockOnTarget(const FInputActionValue& inputValue)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	AThor* Thor = Cast<AThor>(UGameplayStatics::GetActorOfClass(GetWorld() , AThor::StaticClass()));

	if ( !PlayerController ) return;

	// Lock On 해제
	if ( bIsLockedOn )
	{
		bIsLockedOn = false;
		return; // 락온을 해제하고 함수 종료
	}

	// Lock On 활성화
	if ( Thor )
	{
		FRotator LookAtRotation = ( Thor->GetActorLocation() - GetActorLocation() ).Rotation();
		PlayerController->SetControlRotation(FRotator(0.0f , LookAtRotation.Yaw , 0.0f)); // Pitch는 0으로 고정

		bIsLockedOn = true;
	}
}

// 방패 막기 구현
void AKratosCharacter::BlockAttack(const FInputActionValue& inputValue)
{
	if ( bShieldVisible == true) {

	}
	else {
		bShieldVisible = true;
		// 캡슐 콜라이더의 충돌 비활성화 (데미지 받지 않음)
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ShieldCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		// 일정 시간이 지나면 다시 충돌 활성화 (1.5초 후)
		GetWorld()->GetTimerManager().SetTimer(UnblockTimerHandle , this , &AKratosCharacter::EnableCollision , 1.5f , false);
	}
}

void AKratosCharacter::BlockAttackEnd(const FInputActionValue& inputValue)
{
	EnableCollision();
	bShieldVisible = false;
	ShieldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKratosCharacter::EnableCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


// Notify 이벤트 처리
void AKratosCharacter::OnShieldNotifyBegin(FName NotifyName , const FBranchingPointNotifyPayload& Payload)
{
/*
	if ( NotifyName == "ShowShield" )
	{
		ShieldMesh->SetVisibility(true);
	}
	else if ( NotifyName == "HideShield" )
	{
		ShieldMesh->SetVisibility(false);
	}
	*/
}

bool AKratosCharacter::Get_KratosShieldState() const
{
	return bShieldVisible;
}

// 방패 충돌 처리(Collision)
void AKratosCharacter::OnShieldOverlapBP(AActor* OtherActor , FVector SweepResult)
{
	// 토르 공격이 들어오게 된다면, 
	
	// 애니메이션 몬타주 실행


	// 방패 상태 종료
	bShieldVisible = false;
	ShieldCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnableCollision();

}

//========================================================================================
// 분노 모드 추가
void AKratosCharacter::AddRage(float fValue)
{
	if ( CurrentRage > 100.0f ) {
		CurrentRage = 100.0f;
	}
	else {
		CurrentRage += fValue;
	}
	
	MainUI->SetKratosRP(CurrentRage , MaxRage);
}

void AKratosCharacter::RageModeAction(const FInputActionValue& inputValue)
{
	//------------------------------------------------------------
	if ( CurrentRage >= 100.0f ) {
		RageModeUI->AddToViewport();

		mState = EKratosState::Rage;

		//카메라 셰이크 재생
		auto controller = GetWorld()->GetFirstPlayerController();
		controller->PlayerCameraManager->StartCameraShake(CameraShake);

		//--------------------------------------------------------
		bRageMode = true;

		// VFX 소환
		UParticleSystemComponent* SpawnedVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , RagemodeVFX , GetActorLocation());

		// 1초 후 삭제 예약
		FTimerHandle TimerHandle_RageVFX;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_RageVFX , [SpawnedVFX]()
		{
		if ( SpawnedVFX )
		{
			SpawnedVFX->DestroyComponent();
		}
		} , 2.0f , false);

		//-----------------------------------------------------------
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(RageMode_Start_Montage);

		if ( Kratos_EquippedWeapon == true ) {
			Kratos_EquippedWeapon = false;

			//AxeMesh->SetupAttachment(GetMesh() , TEXT("Unarmmed"));
		}

		GetWorld()->GetTimerManager().SetTimer(RageTimerHandle , this ,
	   &AKratosCharacter::RageMode , 0.5f , true);
	}
}


void AKratosCharacter::RageMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Rage Mode is Activated!"));
	AddRage(-10.0f);

	if ( CurrentRage <= 0.0f )
	{
		//분노 모드 UI 비활성화
		RageModeUI->RemoveFromParent();

		mState = EKratosState::Idle;

		GetWorld()->GetTimerManager().ClearTimer(RageTimerHandle);

		bRageMode = false;
	}
}

//========================================================================================
// 회복


// 아이템 사용
void AKratosCharacter::UseItemAction(const FInputActionValue& inputValue)
{
	if ( CurrentItem )
	{
		AItemSpawnManager* itemSpawnManager = Cast<AItemSpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld() , AItemSpawnManager::StaticClass()));

		if ( itemSpawnManager )
		{
			itemSpawnManager->AllItemsinlevel -= 1;

			// CurrentItem을 AItem 타입으로 캐스팅
			AItem* UsedItem = Cast<AItem>(CurrentItem);
			if ( UsedItem ) // 캐스팅이 성공했을 경우에만 실행
			{
				itemSpawnManager->FreeSpawnPoint(UsedItem->GetSpawnIndex());
			}
		}


		// 회복 아이템을 썼을 때
		if ( bIsAHealItem )
		{
			SetKratosHP(30.0f);
			MainUI->SetKratosHP(CurrentHealth , MaxHealth);


			UParticleSystemComponent* SpawnedVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , HealItemUseVFX , CurrentItem->GetActorTransform());

			// 1초 후 삭제 예약
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle , [SpawnedVFX]()
			{
							if ( SpawnedVFX )
							{
								SpawnedVFX->DestroyComponent();
							}
			} , 1.0f , false);

			// 나이아가라 VFX 사용
			/*
			if ( HealItemUseNiagaraVFX )
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld() ,
					HealItemUseNiagaraVFX , // 나이아가라 시스템 변수
					CurrentItem->GetActorLocation() ,
					CurrentItem->GetActorRotation()
				);
			}
			*/

			bIsAHealItem = false;
		}
		else
		{
			// 분노 아이템을 썼을 때
			if ( bIsARageItem == true ) 
			{
				AddRage(30.0f);
				MainUI->SetKratosRP(CurrentRage, MaxRage);

				UParticleSystemComponent* SpawnedVFX = UGameplayStatics::SpawnEmitterAtLocation(GetWorld() , RageItemUseVFX , CurrentItem->GetActorTransform());

				// 1초 후 삭제 예약
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle , [SpawnedVFX]()
				{
						if ( SpawnedVFX )
						{
							SpawnedVFX->DestroyComponent();
						}
				} , 1.0f , false);


				bIsARageItem = false;
			}
		}

		CurrentItem->Destroy();  // 아이템 삭제
		CurrentItem = nullptr;
	}
}

//=======================================================================================
// 플레이어 피격 판정
void AKratosCharacter::OnKratosDamageBP()
{
	mState = EKratosState::Damage;

	//카메라 셰이크 재생
	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(CameraShake);

	// 애니메이션 몬타주 실행
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(TakeDamage_Montage);

	// 데미지 적용, HP 업데이트
	SetKratosHP(-10.0f);
	MainUI->SetKratosHP(CurrentHealth , MaxHealth);

	
	
}






