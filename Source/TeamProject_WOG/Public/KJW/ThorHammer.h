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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartHammerFly(FVector Direction);
	void ReturnHammerFly();

};
