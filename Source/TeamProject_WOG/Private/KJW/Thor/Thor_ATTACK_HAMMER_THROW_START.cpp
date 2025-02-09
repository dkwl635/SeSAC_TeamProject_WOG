// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/Thor/Thor_ATTACK_HAMMER_THROW_START.h"
#include "KJW/ThorHammer.h"

void UThor_ATTACK_HAMMER_THROW_START::InitPattern(AThor* Thor)
{
	Super::InitPattern(Thor);

	if ( ThorHammerClass )
	{
		ThorHammer =Owner->GetWorld()->SpawnActor<AThorHammer>(ThorHammerClass);
		ThorHammer->SetActorLocation(FVector(1000));
	}
}

void UThor_ATTACK_HAMMER_THROW_START::StartPattern_C()
{
	//타겟과의 거리 계산
	FVector Distance = Owner->GetActorLocation() - Owner->Target->GetActorLocation();
	Distance.Z = 0;

	float DisSize = Distance.Size();
	Distance.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("DisSize : %f") , DisSize);

	bEndPattern = false;
	//멀리 있을때
	if (DisSize >= AttackRange)
	{	
		bEndPattern = true;
		ParrtenIndex = 0;
	}
	else
	{
		//뒤로 갈수있는지 체크
		FVector backRot = Distance;
		float dashDistance = AttackRange;
		TArray<FVector> MoveableVector;
		for (int32 i = -3; i < 4; i++)
		{
			float addAngleDeg = 15 * i;
			FVector checkDir = Distance.RotateAngleAxis(addAngleDeg, FVector::UpVector);
			FVector checkLocation = Owner->GetActorLocation() + (checkDir * dashDistance);

			if (FVector::Distance(checkLocation, MapPoint) < MapSize - 100)
			{
				MoveableVector.Add(checkLocation);
			}
		}
		//뒤로 갈 수 있으면 가고
		if (MoveableVector.Num() > 0)
		{
			ParrtenIndex = 1;

			int32 moveIndex = FMath::RandRange(0, MoveableVector.Num() -1);
			//디버그
			for (int32 i = 0; i < MoveableVector.Num(); i++)
			{
				DrawDebugLine(
					Owner->GetWorld(),
					MoveableVector[i],
					MoveableVector[i] + FVector(0, 0, 100), // 위쪽으로 100 유닛
					i == moveIndex ?FColor::Green : FColor::Red , // 랜덤좌표면 
					false,       // 지속 시간 지나면 사라지도록 설정
					2.0f,        // 지속 시간 (2초)
					0,           // 우선순위
					2.0f         // 선 두께
				);
			}
			
			//디버그	
			StartPos = Owner->GetActorLocation();
			MovePos = MoveableVector[moveIndex];

		}
		else	//아니면 킥
		{
			ParrtenIndex = 2;
		}
		
	}

}

void UThor_ATTACK_HAMMER_THROW_START::StartPattern_CBP()
{
	if (ParrtenIndex == 0)
	{
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(AnimMontage);
	}
	else if (ParrtenIndex == 1)
	{
		DashTimer = Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(DashAnimMontage);
		DashDuringTimer = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("%f") , DashTimer);
	}
	else if (ParrtenIndex == 2)
	{
		Owner->GetSkeletalMesh()->GetAnimInstance()->Montage_Play(KickAnimMontage);
	}

}

bool UThor_ATTACK_HAMMER_THROW_START::TickPattern_C()
{
	if (ParrtenIndex == 1)
	{
		DashDuringTimer += Owner->GetWorld()->GetDeltaSeconds();
		if (DashDuringTimer < DashTimer)
		{
			float alpha = DashDuringTimer / DashTimer;
			FVector newMovePos = FMath::InterpEaseOut(StartPos, MovePos, alpha, 2.0f);
			FVector distance = Owner->Target->GetActorLocation() - Owner->GetActorLocation();
			distance.Normalize();
			FRotator rot = distance.Rotation();

			Owner->SetActorLocationAndRotation(newMovePos, rot);
			
		}
		else
		{
			StartPattern();
		}
	}


	return true;
}

void UThor_ATTACK_HAMMER_THROW_START::NotifyTickPattrern_C(int32 EventIndex, float FrameDeltaTime)
{
	if (ParrtenIndex == 2)
	{
		FVector distance = Owner->Target->GetActorLocation() - Owner->GetActorLocation();
		if (distance.Size() < 150) { return; }
		distance.Normalize();
		
		FRotator rot = distance.Rotation();
		FVector newMovePos = Owner->GetActorLocation() + (distance * FrameDeltaTime * 500.f);
		Owner->SetActorLocationAndRotation(newMovePos, rot);
	}

}

void UThor_ATTACK_HAMMER_THROW_START::NotifyEventPattern_C(int32 EventIndex)
{
	if ( ParrtenIndex == 0)
	{
		Owner->ShowHammer(false);
		FVector MoveLocation = Owner->HammerComp->GetComponentLocation();
		ThorHammer->SetActorLocation(MoveLocation);

	}
	else if (ParrtenIndex == 2 && EventIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kick !!!!"));
	}
	else if (ParrtenIndex == 2 && EventIndex == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Kick End!!!!"));
		StartPattern();
	}
	
}
