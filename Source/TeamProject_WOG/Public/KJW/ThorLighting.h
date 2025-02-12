// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThorLighting.generated.h"

UCLASS()
class TEAMPROJECT_WOG_API AThorLighting : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThorLighting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere , BlueprintReadWrite)
	class USphereComponent* SphereComp;


	UFUNCTION(BlueprintNativeEvent)
	void SpawnLighting();

	void SpawnLighting_Implementation();

	UFUNCTION(BlueprintCallable)
	void TakeLightingDamage();

	UPROPERTY()
	class AThor* Thor;

	UPROPERTY(EditAnywhere)
	float Damage = 10;


};
