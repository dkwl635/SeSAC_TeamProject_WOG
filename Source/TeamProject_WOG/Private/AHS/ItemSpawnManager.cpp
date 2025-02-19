// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/ItemSpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "AHS/HealStone.h"
#include "AHS/RageStone.h"
#include "AHS/KratosCharacter.h"

// Sets default values
AItemSpawnManager::AItemSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	float createTime = FMath::RandRange(minTime, maxTime);
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle, this, &AItemSpawnManager::CreateItem, createTime);

	FindSpawnPoints();	
}

// Called every frame
void AItemSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AItemSpawnManager::CreateItem()
{
	if ( AllItemsinlevel >= 5 ) {
		// 랜덤한 시간을 구하기
		float createTime = FMath::RandRange(minTime , maxTime);

		// 다시 타이머에 등록하기
		GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle , this , &AItemSpawnManager::CreateItem , createTime);

		return;
	}

	// 랜덤한 스폰 위치 찾기 (이미 사용된 위치는 제외)
	int index;
	do {
		index = FMath::RandRange(0 , spawnPoints.Num() - 1);
	} while ( usedIndices.Contains(index) && usedIndices.Num() < spawnPoints.Num() );

	// 사용된 위치 저장
	usedIndices.Add(index);

	// 아이템 배치하기
	int32 RandItem = FMath::RandRange(1 , 100);

	// 플레이어의 상태에 따라, 우선순위를 가져가는 상태가 존재함.
	int32 PlayerCurrentState = 0;
	AKratosCharacter* kratos = Cast<AKratosCharacter>(GetWorld()->GetFirstPlayerController());

	if ( kratos != nullptr ) {
		float hp = kratos->CurrentHealth;
		float rp = kratos->CurrentRage;
		
		// HP가 우선순위를 가져감.
		if (hp <= 50){
			PlayerCurrentState = 70;
		}
		else {
			// 분노 게이지 체크
			if ( rp <= 50 ) {
				PlayerCurrentState = 30;
			}
			else {
				//HP도 높고, RP도 높다면 50%
				PlayerCurrentState = 50;
			}
		}

	}
	

	// AItemSpawnManager.cpp (아이템 생성 시 설정)
	if ( RandItem < 50 ) {
		AHealStone* newItem = GetWorld()->SpawnActor<AHealStone>(healStoneFactory , spawnPoints[index]->GetActorLocation() , FRotator(0));
		newItem->SetSpawnIndex(index);
	}
	else {
		ARageStone* newItem = GetWorld()->SpawnActor<ARageStone>(rageStoneFactory , spawnPoints[index]->GetActorLocation() , FRotator(0));
		newItem->SetSpawnIndex(index);
	}

	AllItemsinlevel++;

	// 랜덤한 시간을 구하기
	float createTime = FMath::RandRange(minTime , maxTime);

	// 다시 타이머에 등록하기
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandle , this , &AItemSpawnManager::CreateItem , createTime);
}


void AItemSpawnManager::FindSpawnPoints()
{
	// 검색으로 찾은 결과를 저장할 배열
	TArray<AActor*> allActors;

	// 원하는 타입의 액터를 모두 찾아오기
	UGameplayStatics::GetAllActorsOfClass(GetWorld() , AActor::StaticClass() , allActors);

	// 찾은 결과가 있을경우에는 반복적으로 처리
	for ( auto spawn : allActors )
	{
		// 찾은 액터의 이름에 해당 문자열을 포함하고 있다면
		if ( spawn->GetName().Contains(TEXT("BP_ItemSpawnPoint")) )
		{
			// 스폰 목록에 추가
			spawnPoints.Add(spawn);
		}
	}
}

void AItemSpawnManager::FreeSpawnPoint(int index)
{
	usedIndices.Remove(index); // 사용된 위치 제거
}

