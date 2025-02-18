// Fill out your copyright notice in the Description page of Project Settings.


#include "RageStone.h"

#include "Components/SphereComponent.h"

#include "AHS/KratosCharacter.h"

ARageStone::ARageStone()
{
	PrimaryActorTick.bCanEverTick = false;

	//1. 외형 설정
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	//2. 충돌 처리
	SphereComp->SetCollisionProfileName(TEXT("PlayerItem"));

}

void ARageStone::BeginPlay()
{
	Super::BeginPlay();
}

void ARageStone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AKratosCharacter* kratos = Cast<AKratosCharacter>(OtherActor);

	if ( kratos != nullptr ) {
		kratos->CurrentItem = this;
		kratos->bIsARageItem = true;
	}
}

void ARageStone::NotifyActorEndOverlap(AActor* OtherActor)
{
	AKratosCharacter* kratos = Cast<AKratosCharacter>(OtherActor);
	if ( kratos && kratos->CurrentItem == this )
	{
		kratos->CurrentItem = nullptr;  // 플레이어가 아이템에서 벗어나면 초기화
		kratos->bIsARageItem = false;
	}

}


