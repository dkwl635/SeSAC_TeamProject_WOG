// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_TRIPLE_LIGHTING_STRIKES.h"
#include "KJW/ThorLighting.h"


void UThor_TRIPLE_LIGHTING_STRIKES::InitPattern(AThor* Thor)
{
	Super::InitPattern(Thor);
	FVector SpawnPos = FVector(0.0f, 0.0f, -10000.f);
	for (int32 i = 0; i < LightingInitSpawnCount; i++)
	{
		AThorLighting* actor = Owner->GetWorld()->SpawnActor<AThorLighting>(ThorLightingClass, SpawnPos, FRotator());
		ThorLightings.Add(actor);
	}
}

void UThor_TRIPLE_LIGHTING_STRIKES::StartPattern_CBP()
{
	//시작시 히트 상태 전환불가
	bHitable = false;


	Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
	LightingSpawnCount = 0;
	LightingShowCount = 0;
	Owner->GetWorldTimerManager().SetTimer(SpawnLightingTimerHandle, this, &ThisClass::SpawnLightings, 0.3f, true , 0.1f);
}



void UThor_TRIPLE_LIGHTING_STRIKES::NotifyEventPattern_C(int32 EventIndex)
{
	ThorLightings[LightingShowCount]->SpawnLighting();
	LightingShowCount++;

		
}

void UThor_TRIPLE_LIGHTING_STRIKES::SpawnLightings()
{
	FVector targetPos = Owner->Target->GetActorLocation();
	FVector thorPos = Owner->GetActorLocation();

	FVector LightPos = FVector::ZeroVector;
	if (LightingSpawnCount == 2)
	{
		LightPos = targetPos;
	}
	else
	{
		FVector2D Random = FMath::RandPointInCircle(1);
		Random = Random * FMath::RandRange(0.0f , MapSize * 0.5f);
		LightPos.X = Random.X;
		LightPos.Y = Random.Y;

	}
	LightPos.Z = 0.0f;
	ThorLightings[LightingSpawnCount]->SetActorLocation(LightPos);

	if (LightingSpawnCount == 2)
	{
		Owner->GetWorldTimerManager().ClearTimer(SpawnLightingTimerHandle);
	}
	else
	{
	LightingSpawnCount++;
	}
}
