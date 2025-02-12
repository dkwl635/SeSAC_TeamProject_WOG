// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_Move.h"

void UThor_Move::InitPattern(AThor* Thor)
{
	Super::InitPattern(Thor);

	SpawnPos = Thor->GetActorLocation();

}

void UThor_Move::StartPattern_C()
{
	Owner->SetIsMove(true);
	UE_LOG(LogTemp, Warning, TEXT("StartPattern_C_MOVE"));

	MovePos = Owner->GetMoveRandomPos(SpawnPos, 400.f);

	Direction = MovePos - Owner->GetActorLocation();
	Direction.Z = 0;
	Direction.Normalize();
	
	FRotator rot = Direction.Rotation();
	Owner->SetActorRotation(rot);
}

void UThor_Move::StopPattern_C()
{
	Owner->SetIsMove(false);
}

void UThor_Move::EndPattern_C()
{
	Owner->SetIsMove(false);
}

bool UThor_Move::TickPattern_C()
{ 
	FVector OwnerLocation = Owner->GetActorLocation();

	// MovePos와 Owner의 거리 계산
	float Distance = FVector::Dist(OwnerLocation, MovePos);

	// 거리가 10 이하라면 추가 작업 수행
	if (Distance <= 100.0f)
	{
		//재설정
		StartPattern_C();
	}


	FVector Move = Direction * WalkSpeed * Owner->GetWorld()->DeltaTimeSeconds;
	Owner->SetActorLocation(Owner->GetActorLocation() + Move);


	// 디버그 라인: MovePos에서 위쪽(Z+ 방향)으로 100 유닛 길이
	DrawDebugLine(
		Owner->GetWorld(),
		MovePos,
		MovePos + FVector(0, 0, 100), // 위쪽으로 100 유닛
		FColor::Red, // 빨간색 선
		false,       // 지속 시간 지나면 사라지도록 설정
		2.0f,        // 지속 시간 (2초)
		0,           // 우선순위
		2.0f         // 선 두께
	);

	return true;
}

void UThor_Move::NotifyEventPattern_C(int32 EventIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Move_NotifyBeginPattern_C"));
}

