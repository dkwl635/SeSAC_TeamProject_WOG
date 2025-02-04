// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Thor.h"
#include "TeamProject_WOG/TeamProject_WOG.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "KJW/ThorAnimInstance.h"
#include "KJW/ThorPattern.h"

// Sets default values
AThor::AThor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyComp"));

	SetRootComponent(BodyCollision);
	BodyComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AThor::BeginPlay()
{
	Super::BeginPlay();

	ThorAnimIns = Cast<UThorAnimInstance>(GetSkeletalMesh()->GetAnimInstance());

	InitPatternClass();
}

// Called every frame
void AThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AThor::SetIsMove(bool Isbool)
{
	ThorAnimIns->IsMove = Isbool;
}



EWOG_Character_State AThor::GetCharacterState()
{
	return EWOG_Character_State::NONE;
}

void AThor::TakeKDamage( const FWOG_DamageEvent& KDamageEvent)
{
	
}

USkeletalMeshComponent* AThor::GetSkeletalMesh()
{
	return BodyComp;
}

void AThor::InitPatternClass()
{
	//PatternClass 생성후 TMap 넣기
	for ( int32 i = 0; i < PatternClass.Num(); i++ )
	{
		UThorPattern* ThorPattern = NewObject<UThorPattern>(this , PatternClass[i]);
		if ( ThorPattern )
		{
			EThorPattern type = ThorPattern->ThorPattern;
			
			//추가 중 중복이 있다면 문제있는거임
			if ( Patterns.Contains(type) )
			{
				UE_LOG(LogTemp , Error , TEXT("Patterns ERROR "));
				continue; 
			}
			ThorPattern->InitPattern(this);
			Patterns.Add(type , ThorPattern);
		}
	}

	StartPattarn(EThorPattern::MOVE);
}


UThorPattern* AThor::GetPattern(EThorPattern PatternType)
{
	if (!Patterns.Contains(PatternType)) { return nullptr; }

	return Patterns[PatternType];
}

void AThor::TickPattarn()
{
	if (CurPattern)
	{
		CurPattern->TickPattern();
	}
}

void AThor::StartPattarn(EThorPattern NewPattern)
{
	UThorPattern* Start = GetPattern(NewPattern);
	if (!Start) return;

	Start->StartPattern();
	CurPattern = Start;
}

void AThor::ChangePattarn(EThorPattern NewPattern)
{
	
}

void AThor::EndPattarn(EThorPattern NewPattern)
{
	UThorPattern* End= GetPattern(NewPattern);
	if (!End) return;

	End->EndPattern();

}



