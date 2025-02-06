// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "LeviathanAxe.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_WOG_API ALeviathanAxe : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALeviathanAxe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;
	
};
