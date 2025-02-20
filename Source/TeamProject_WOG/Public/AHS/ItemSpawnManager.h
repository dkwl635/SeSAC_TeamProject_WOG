// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnManager.generated.h"

UCLASS()
class TEAMPROJECT_WOG_API AItemSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//=====================================================================================
public:
	// 랜덤한 시간 최소값
	UPROPERTY( EditAnywhere, Category = SpawnSetting)
	float minTime = 0.0f;

	// 랜덤한 시간 최대값
	UPROPERTY( EditAnywhere, Category = SpawnSetting)
	float maxTime = 5.0f;

	// 스폰할 위치 정보 배열
	UPROPERTY( EditAnywhere , Category = SpawnSetting )
	TArray<class AActor*>	spawnPoints;

	// healstone 팩토리
	UPROPERTY( EditAnywhere, Category = SpawnSetting)
	TSubclassOf<class AHealStone> healStoneFactory;

	// ragestone 팩토리
	UPROPERTY( EditAnywhere, Category = SpawnSetting)
	TSubclassOf<class ARageStone> rageStoneFactory;

	// 스폰을 위한 타이머
	FTimerHandle	spawnTimerHandle;

	// 적 생성 하는 함수
	void CreateItem();

	// 스폰할 위치를 동적으로 찾아서 할당하기
	void FindSpawnPoints();

	//=================================================
	// 총 스폰된 아이템 갯수
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = SpawnSetting)
	int32 AllItemsinlevel = 0;

	TArray<int> usedIndices; // 사용된 인덱스를 저장

	void FreeSpawnPoint(int index); // 특정 위치를 다시 사용할 수 있도록 해제


};
