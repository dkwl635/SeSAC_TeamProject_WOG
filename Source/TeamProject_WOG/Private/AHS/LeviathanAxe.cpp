// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/LeviathanAxe.h"
#include "Components/BoxComponent.h"
#include "AHS/KratosCharacter.h"
#include "Camera/CameraComponent.h"

ALeviathanAxe::ALeviathanAxe()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);

	//------------------------------------------------
	bIsReturning = false;
	bIsMoving = false;

}

void ALeviathanAxe::BeginPlay()
{
	Super::BeginPlay();
}


void ALeviathanAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch ( mState ) {
		case EAxeState::Default: {	DefaultState();	} break;
		case EAxeState::Thorwed: {	ThrowState();	} break;
	}
	//--------------------------------------------------------
    if ( bIsMoving )
    {
        ElapsedTime += DeltaTime;
        float T = FMath::Clamp(ElapsedTime / MoveDuration , 0.0f , 1.0f);

        // 베지어 곡선 공식 적용
        FVector ControlPoint = ( StartPos + EndPos ) / 2 + FVector(400 , 0 , 150);
        FVector NewPos = FMath::Lerp(
            FMath::Lerp(StartPos , ControlPoint , T) ,
            FMath::Lerp(ControlPoint , EndPos , T) ,
            T
        );

        SetActorLocation(NewPos);

        if ( T >= 1.0f )
        {
            bIsMoving = false;
            if ( bIsReturning )
            {
                Destroy();
            }
        }
    }

}


/*
void ALeviathanAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch ( mState )
	{
	case EAxeState::Default: { DefaultState(); } break;
	case EAxeState::Thorwed: { ThrowState(); } break;
	}

	if ( bIsMoving )
	{
		ElapsedTime += DeltaTime;
		float T = FMath::Clamp(ElapsedTime / MoveDuration , 0.0f , 1.0f);

		// Control Points 수정
		FVector MidPoint = ( StartPos + EndPos ) / 2;
		float CurveDirection = ( FMath::RandBool() ) ? 1.0f : -1.0f;

		FVector ControlPoint1 = MidPoint + FVector(0 , CurveDirection * 200 , 200);
		FVector ControlPoint2 = MidPoint + FVector(0 , CurveDirection * 150 , 100);

		// 이동 위치 계산
		FVector NextPos = FMath::CubicInterp(StartPos , ControlPoint1 , ControlPoint2 , EndPos , T);

		//디버그 로그 추가
		UE_LOG(LogTemp , Warning , TEXT("T: %f, NextPos: %s") , T , *NextPos.ToString());

		// LineTrace 충돌 검사
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult , GetActorLocation() , NextPos , ECC_Visibility , Params);

		UE_LOG(LogTemp , Warning , TEXT("LineTrace Hit: %s") , bHit ? TEXT("true") : TEXT("false"));

		if ( bHit )
		{
			SetActorLocation(HitResult.Location);
			SetActorRotation(HitResult.ImpactNormal.Rotation());
			bIsMoving = false;
			return;
		}

		// 도끼 회전 추가
		SetActorLocation(NextPos);
		FRotator SpinRotation = FRotator(0 , 0 , 720 * DeltaTime);
		AddActorLocalRotation(SpinRotation);

		if ( T >= 1.0f )
		{
			bIsMoving = false;
		}
	}
}
*/







void ALeviathanAxe::DefaultState()
{

}

void ALeviathanAxe::ThrowState()
{
	//FVector destination = target->KratosCamComp
}

//----------------------------------------------
void ALeviathanAxe::ThrowAxe(FVector TargetLocation)
{
    StartPos = GetActorLocation();
    EndPos = TargetLocation;
    ElapsedTime = 0.0f;
    bIsMoving = true;
    bIsReturning = false;
}

void ALeviathanAxe::ReturnAxe(FVector StartLocation)
{
    StartPos = GetActorLocation();
    EndPos = StartLocation;
    ElapsedTime = 0.0f;
    bIsMoving = true;
    bIsReturning = true;
}
