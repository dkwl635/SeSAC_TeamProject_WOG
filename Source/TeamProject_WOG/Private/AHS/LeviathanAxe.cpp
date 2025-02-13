// Fill out your copyright notice in the Description page of Project Settings.


#include "AHS/LeviathanAxe.h"
#include "Components/BoxComponent.h"
#include "AHS/KratosCharacter.h"
#include "Camera/CameraComponent.h"

#include "KJW/Thor.h"

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

	target = Cast<AKratosCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
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

void ALeviathanAxe::OnAxeOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	AThor* thor = Cast<AThor>(OtherActor);
	if ( thor != nullptr ) {
		UE_LOG(LogTemp, Warning, TEXT("Thor"));

		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = SweepResult.ImpactPoint;

		thor->TakeKDamage(DamageData, target);
	}
}

void ALeviathanAxe::OnAxeOverlapBP(AActor* OtherActor , FVector SweepResult)
{
	AThor* thor = Cast<AThor>(OtherActor);
	if ( thor != nullptr ) {
		UE_LOG(LogTemp , Warning , TEXT("Thor"));

		FWOG_DamageEvent DamageData;
		DamageData.DamageValue = 10;
		DamageData.HitPoint = SweepResult;

		thor->TakeKDamage(DamageData , target);
	}
}
