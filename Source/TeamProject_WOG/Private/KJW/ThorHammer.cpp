// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ThorHammer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AHS/KratosCharacter.h"
#include "KJW/Thor.h"
// Sets default values
AThorHammer::AThorHammer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShapeComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	SetRootComponent(SphereComp);
	MeshComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AThorHammer::BeginPlay()
{
	Super::BeginPlay();
	//우선 날아가지 않은 상태
	IsHammerFly = false;
}

void AThorHammer::HammerFly(FVector Direction)
{
	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//날아가는 방향으로 머리 회전 시키기
	FlyDirection = Direction;

	FVector CurrentUpVector = GetActorUpVector();
	FVector NewUpVector = Direction.GetSafeNormal();

	FQuat RotationQuat = FQuat::FindBetweenNormals(CurrentUpVector , NewUpVector);
	FQuat NewActorQuat = RotationQuat * GetActorQuat();

	SetActorRotation(NewActorQuat);
}

void AThorHammer::StartHammerFly(FVector Direction)
{

	//날아가는 2초동안 날아가게
	HammerFly(Direction);
	IsHammerFly = true;
	MoveTimer = 0.0f;
	GetWorldTimerManager().SetTimer(FlyMoveTimerHandle , this , &ThisClass::HammerMoveTick , 0.02f , true , 0.0f);
}

void AThorHammer::HammerMoveTick()
{
	//2초동안 날아가는 tick
	MoveTimer += 0.02f;
	FVector NewPos = GetActorLocation();
	NewPos += MoveSpeed * FlyDirection * 0.02f;
	SetActorLocation(NewPos);

	if ( MoveTimer >= 2.0f )
	{
		GetWorldTimerManager().ClearTimer(FlyMoveTimerHandle);
	}
}

void AThorHammer::ReturnHammerFly()
{
	//돌아오고 손에 있는 상태
	IsHammerFly = false;

	SetActorLocation(FVector(0.0f , 0.f , -200.f));
}

void AThorHammer::HammerDown()
{
	float DeltaTime = 0.02f;

	FVector CurPos = GetActorLocation();
	//바닥에 닿았을 때 Z 좌표 비교
	if ( CurPos.Z < 33.0f )
	{
		//물리 off
		SphereComp->SetSimulatePhysics(false);
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//타임헨들 클리어
		if ( FlyDownTimerHandle.IsValid() )
		{
			GetWorldTimerManager().ClearTimer(FlyDownTimerHandle);
		}

	}
	else
	{
		// 아래쪽으로 향하게
		FRotator TargetRotation = FRotationMatrix::MakeFromZ(FVector::DownVector).Rotator();
	
		// 현재 회전 가져오기
		FRotator CurrentRotation = GetActorRotation();
		// 부드럽게 보간하여 회전 적용
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation , TargetRotation , DeltaTime , 2.0f);
		SetActorRotation(NewRotation);
		
	}

}

void AThorHammer::OnCollisionOverlap(AActor* Otherctor)
{
	ACharacter* player = Cast<ACharacter>(Otherctor);
	if (player)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereComp->SetSimulatePhysics(true);
		
		FVector AddForce = FlyDirection * -1 * 10000.0f;
		AddForce.Z = 10000.0f;
		
		SphereComp->AddImpulse(AddForce);

		GetWorldTimerManager().SetTimer(FlyDownTimerHandle , this , &ThisClass::HammerDown , 0.02f , true , 0.0f);		
		
		if ( Thor )
		{
			FWOG_DamageEvent DamageData;
			DamageData.DamageValue = 10;
			DamageData.HitPoint = GetActorLocation();
			Thor->Target->TakeKDamage(DamageData , Thor);
		}
	}
	else
	{

	}

	if (FlyMoveTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(FlyMoveTimerHandle);
	}

}

