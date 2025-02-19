// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

#include "CombatInterface.h"

#include "TeamProject_WOG/TeamProject_WOG.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"


#include "KratosCharacter.generated.h"

UENUM(BlueprintType)
enum class EKratosState : uint8 {
// 기본 상태
	Idle UMETA(DisplayName = "Normal Idle") ,
	Move UMETA(DisplayName = "Move") ,
	Shield UMETA(DisplayName = "Shield") ,
	Dodge UMETA(DisplayName = "Dodge") ,
	Damage UMETA(DisplayName = "Damaged") ,
	Die UMETA(DisplayName = "Die") ,

// 기본 공격(주먹)
	Attack UMETA(DisplayName = "Attack"),

	FistAttack UMETA(DisplayName = "Fist Attack") ,
	FistAttackCombo UMETA(DisplayName = "Fist Attack Combo") ,

// 무기 - 도끼 공격
	SheathAxe UMETA(DisplayName = "Sheath Axe") ,
	SheathAxeIdle UMETA(DisplayName = "Sheath Axe Idle") ,
	AxeMeleeAttack UMETA(DisplayName = "Axe Melee Attack") ,

	AxeAim UMETA(DisplayName = "Axe Aiming") ,
	AxeUnAim UMETA(DisplayName = "Axe UnAiming") ,

	AxeRangedAttack UMETA(DisplayName = "Axe Ranged Attack") ,
	AxeMeleeAttackCombo UMETA(DisplayName = "Axe Melee Attack Combo") ,

// 분노 상태(Rage)
	Rage UMETA(DisplayName = "Rage"),
	RageIdle UMETA(DisplayName = "Rage Idle") ,
	RageAttackCombo UMETA(DisplayName = "Rage Attack Combo") ,
};


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
	// 기본 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = KratosState)
	EKratosState mState = EKratosState::Idle;

	//-------------------------------------------------------------------
	// HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float CurrentHealth = 10.0f;

	float GetKratosHP();
	void SetKratosHP(float hp);


	
	//-------------------------------------------------------------------
	// 분노 게이지(Rage meter)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float CurrentRage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float MaxRage = 100.0f;

	// 카메라 회전 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float VerticalRotationSpeed = 0.5f; // 기본값보다 작게 조정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float HorizontalRotationSpeed = 0.25f;

	// 이동 속도
	UPROPERTY(EditAnywhere , Category = PlayerSettings)
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere , Category = PlayerSettings)
	float RunSpeed = 800.0f;

	// 이동 방향
	FVector Direction;

	// 무기(도끼) --> 손에 붙어 있는 것.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Axe)
	class UStaticMeshComponent* AxeMesh;

	// 무기 충돌 처리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Axe)
	class UBoxComponent* AxeCollision;

	// 무기가 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSettings)
	bool Kratos_HasWeapon = true;

	// 무기 장착 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerSettings)
	bool Kratos_EquippedWeapon = false;

	// 무기 장착 후, 에임 상태 진입
	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	bool AimAttackState = false;

	// 무기 장착 상태 반환(BluePrint에서 장착된 무기의 Visibility 설정에 사용중)
	UFUNCTION(BlueprintPure)
	bool Get_KratosEquippedWeapon() const;

	float SavedPitch = 0.0f;



// 1. Camera
public:
	UPROPERTY( EditDefaultsOnly , Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* KratosCamComp;

	
//2. Input
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Kratos;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY( EditDefaultsOnly , Category = "Input" )
	class UInputAction* IA_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Aim;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Return;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sheath_UnSheath;

	//도끼 원거리 공격
	UPROPERTY(EditDefaultsOnly , Category = "Input")
	class UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly , Category = "Input")
	class UInputAction* IA_LockOn;


	// 카메라 회전
	void Turn(const FInputActionValue& inputValue);

	void LookUp(const FInputActionValue& inputValue);

	// 플레이어 이동
	void Move(const FInputActionValue& inputValue);
	// 에임(무기 던지기) 상태
	void AimAxeAttack(const FInputActionValue& inputValue);

	// 던진 무기 돌려받기
	void ReturnAxetoHand(const FInputActionValue& inputValue);

	// 무기 장착 및 해제
	void SheathAction(const FInputActionValue& inputValue);

	// 공격
	void AttackAction(const FInputActionValue& inputValue);

	// 카메라 록온
	void LockOnTarget(const FInputActionValue& inputValue);

//3. 애니메이션
	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Equip_Axe_Montage;

	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* UnEquip_Axe_Montage;

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Attack_Axe_Montage;

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* Return_Axe_Montage;

	// 도끼 근접 공격
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* Melee_Attack_Montage;

	// 근접 공격(주먹)
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* Fist_Attack_Montage;

	// 데미지받았을 때
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* TakeDamage_Montage;


//4. 도끼
public:
	// 소환된 도끼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Axe)
	class ALeviathanAxe* AxeActor;

	 // 도끼의 스폰 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ALeviathanAxe> AxeClass;

	// 도끼 스폰 포인트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Axe)
	class UArrowComponent* AxeSpawnPoint;

	// 도끼 Aim UI 위젯
	UPROPERTY(EditDefaultsOnly, Category = AimAxeUI)
	TSubclassOf<class UUserWidget> AimAxeUIFactory;

	// 도끼 Aim UI 위젯 인스턴스
	UPROPERTY()
	class UUserWidget* AimAxeUI;

	// 바라보는 방향 위치
	FVector GetAimLocation();

	//도끼 던지기
	UFUNCTION(BlueprintCallable)
	void ThrowAxe();

	//도끼 불러오기
	void RecallAxe();

//-----------------------------------------
// 2/11(화)
	void IsAiming(bool bIsAiming);
	
	// 공격 모션 후, 대기 시간
	float currentTime = 0.0f;

	// 두 번 누르면 중복 공격되지 않도록 처리
	bool ClickOnce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fist)
	class USphereComponent* Fist_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fist)
	class USphereComponent* Fist_L;

//2/12(수) 주먹 공격 Collision 구현
	UFUNCTION(BlueprintCallable)
	void OnHandOverlapBP(  AActor* OtherActor   , FVector SweepResult);

//2/13(목) BP 구현된 기능들 C++로 옮겨오기
	//01) 콤보 공격(도끼)
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* AxeCombo_Montage;

	//주먹 Collision 켜기/끄기
	void FistCollision(bool bValue);

	// 무기 Collision 켜기/끄기
	void OnAxeCollision(bool bValue);

	// Dash 기능 추가
	UPROPERTY( EditDefaultsOnly , Category = "Input" )
	class UInputAction* IA_Dash;

	void DashInput();

	// Main UI 추가
	UPROPERTY(EditDefaultsOnly, Category = AimAxeUI)
	TSubclassOf<class UUserWidget> MainUIFactory;

	// Main UI 위젯 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMainUI* MainUI;

//2/14(금)
	//방패 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield")
	class USkeletalMeshComponent* ShieldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fist)
	class UBoxComponent* ShieldCollision;

	//---------------------------------------------------------
	void CameraAimRotation();

	// Anim Montage C++에서 실행
	void OnAttackNotify(FName NotifyName , const FBranchingPointNotifyPayload& BranchingPointPayload);

	bool bIsLockedOn = false;

	// 방패 막기
	UPROPERTY( EditDefaultsOnly , Category = "Input" )
	class UInputAction* IA_Block;

	void BlockAttack(const FInputActionValue& inputValue);
	void BlockAttackEnd(const FInputActionValue& inputValue);

	// 방패로 막기
	UPROPERTY(EditAnywhere, Category = AnimMontage)
	UAnimMontage* Block_Montage;

	void EnableCollision();

	FTimerHandle UnblockTimerHandle;


	void OnShieldNotifyBegin(FName NotifyName , const FBranchingPointNotifyPayload& Payload);

	// 방패 보이기 안 보이기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shield)
	bool bShieldVisible = false;

	// 무기 장착 상태 반환(BluePrint에서 장착된 무기의 Visibility 설정에 사용중)
	UFUNCTION(BlueprintPure)
	bool Get_KratosShieldState() const;

	UFUNCTION(BlueprintCallable)
	void OnShieldOverlapBP(AActor* OtherActor , FVector SweepResult);
	//---------------------------------------------------------
//0217(월)
	// 분노 게이지 차오르기
	void AddRage(float fValue);

	UPROPERTY( EditDefaultsOnly , Category = "Input" )
	class UInputAction* IA_RageMode;

	UPROPERTY(EditAnywhere , Category = AnimMontage)
	UAnimMontage* RageMode_Start_Montage;

	// 분노 게이지 모드
	void RageModeAction(const FInputActionValue& inputValue);

	void RageMode();

	FTimerHandle RageTimerHandle;

	bool bRageMode = false;

	//==================================================================
//0218(화)
	// 아이템 사용 Input Action
	UPROPERTY(EditDefaultsOnly , Category = "Input")
	class UInputAction* IA_ItemUse;

	void UseItemAction(const FInputActionValue& inputValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	AActor* CurrentItem;

	//UPROPERTY(EditAnywhere)
	//class UNiagaraSystem* HealItemUseNiagaraVFX;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* HealItemUseVFX;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* RageItemUseVFX;

	bool bIsAHealItem = false;
	bool bIsARageItem = false;

	//====================================================
	//데미지 받았을 때 처리

	UFUNCTION(BlueprintCallable)
	void OnKratosDamageBP();


	//-------------------------------------------------------------------
	// ICombatInterface을(를) 통해 상속됨
	void SetCharacterState(EWOG_Character_State NewState) override;
	void TakeKDamage(const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser) override;
	USkeletalMeshComponent* GetSkeletalMesh() override;
	AActor* GetActor() override;
};
