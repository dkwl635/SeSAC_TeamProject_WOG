// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CombatInterface.h"
#include "TeamProject_WOG/TeamProject_WOG.h"
#include "Thor.generated.h"



UCLASS()
class TEAMPROJECT_WOG_API AThor : public APawn , public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AThor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//토르 구조
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* BodyCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* BodyComp;
//애니메이션 관련
private:
	UPROPERTY()
	class UThorAnimInstance* ThorAnimIns;
public:
	
	void SetIsMove(bool Isbool);

//크레토스가 들어갈 공간	
public:
	UPROPERTY()
	AActor* Target;

//WOG 전투 관련 인터페이스
public:
	virtual void SetCharacterState(EWOG_Character_State NewState) override;
	virtual void TakeKDamage( const FWOG_DamageEvent& DamageEvent , ICombatInterface* DamageCauser) override;
	virtual class USkeletalMeshComponent* GetSkeletalMesh() override;
	virtual AActor* GetActor() override { return this; };
	
	void TestDamageEvnet(AActor* DamageTarget);

//토르 패턴 컨트롤

public:
	UPROPERTY(EditAnywhere , Category = PatternClass)
	TArray<TSubclassOf<class UThorPattern>> PatternClass;
	void InitPatternClass();

public:
	UFUNCTION(BlueprintCallable)
	void ChangePattarn(EThorPattern NewPattern);
	UFUNCTION(BlueprintCallable)
	void StartPattarn(EThorPattern NewPattern);
	UFUNCTION(BlueprintCallable)
	void TickPattarn();
	UFUNCTION(BlueprintCallable)
	void EndPattarn(EThorPattern NewPattern);
private:
	UPROPERTY()
	class UThorPattern* CurPattern;
	UPROPERTY()
	TMap<EThorPattern , class UThorPattern*> Patterns;

	class UThorPattern* GetPattern(EThorPattern Pattern);



	UFUNCTION(BlueprintCallable)
	void NotifyEventPattern(int32 EventIndex);
	UFUNCTION(BlueprintCallable)
	void NotifyBeginPattern(int32 EventIndex , float TotalDuration);
	UFUNCTION(BlueprintCallable)
	void NotifyEndPattrern(int32 EventIndex);
	UFUNCTION(BlueprintCallable)
	void NotifyTickPattrern(int32 EventIndex, float FrameDeltaTime);

public:
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage , bool bInterrupted);

	
public:
	FVector GetMoveRandomPos(FVector SpawnPos , float MapSize , float Dist);
	
	
};
