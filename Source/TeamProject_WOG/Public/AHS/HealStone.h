// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AHS/Item.h"
#include "HealStone.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API AHealStone : public AItem
{
	GENERATED_BODY()

public:
	AHealStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	// 1. 힐링 스톤 외형
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	// 2. 충돌 처리
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	
	
};
