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
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* BodyCollision;

	UPROPERTY(EditAnywhere)
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
	virtual EWOG_Character_State GetCharacterState() override;
	virtual void TakeKDamage( const FWOG_DamageEvent& KDamageEvent) override;
	virtual class USkeletalMeshComponent* GetSkeletalMesh() override;
	virtual AActor* GetActor() override { return this; };

//토르 패턴 컨트롤
private:
	UPROPERTY(VisibleAnywhere)
	EWOG_Character_State CharacterState = EWOG_Character_State::NONE;
public:
	UPROPERTY(EditAnywhere , Category = PatternClass)
	TArray<TSubclassOf<class UThorPattern>> PatternClass;
	void InitPatternClass();
private:
	UPROPERTY()
	class UThorPattern* CurPattern;
	UPROPERTY()
	TMap<EThorPattern , class UThorPattern*> Patterns;
	
	class UThorPattern* GetPattern(EThorPattern Pattern);

	void TickPattarn();
	void StartPattarn(EThorPattern NewPattern);
	void ChangePattarn(EThorPattern NewPattern);
	void EndPattarn(EThorPattern NewPattern);
	
	
};
