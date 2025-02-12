// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThorHammer.generated.h"



UCLASS()
class TEAMPROJECT_WOG_API AThorHammer : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

public:	
	// Sets default values for this actor's properties
	AThorHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void StartHammerFly(FVector Direction);
	void HammerFly(FVector Direction);
	void HammerMoveTick();
	void ReturnHammerFly();
	void HammerDown();

public:
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 1500.0f;
private:
	FRotator HamerFly = FRotator(90.0f , 0.0f , 0.0f);
	FVector FlyDirection = FVector::ZeroVector;

	FTimerHandle FlyMoveTimerHandle;
	FTimerHandle FlyDownTimerHandle;
	float MoveTimer = 0.0f;
public:
	bool IsHammerFly = false;

	float Damage = 10.0f;
public:

	UFUNCTION()
	void OverlapHammer(
	  UPrimitiveComponent* OverlappedComponent , AActor*  OtherActor , UPrimitiveComponent*  OtherComp , int32  OtherBodyIndex , bool  bFromSweep , const FHitResult&  SweepResult);

public:
	UPROPERTY()
	class AThor* Thor;
	bool IsGround = false;

};
