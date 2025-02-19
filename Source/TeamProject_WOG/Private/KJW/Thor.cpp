// Fill out your copyright notice in the Description page of Project Settings.
#include "KJW/Thor.h"
#include "TeamProject_WOG/TeamProject_WOG.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "KJW/ThorAnimInstance.h"
#include "KJW/ThorPattern.h"
#include "KJW/Thor/Thor_Idle.h"
#include "KJW/Thor/Thor_HIT.h"
#include "KJW/ThorHammer.h"
#include "NiagaraComponent.h"

// Sets default values
AThor::AThor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyComp"));
	HammerComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HammerComp"));
	WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeaponEffect"));


	SetRootComponent(BodyCollision);
	BodyComp->SetupAttachment(GetRootComponent());
	HammerComp->SetupAttachment(BodyComp, TEXT("hand_r"));
	HammerComp->SetRelativeScale3D(FVector(0.04f));
	HammerComp->SetRelativeLocation(FVector( -0.4f , 0.0f , -0.8f ));

	WeaponEffect->SetupAttachment(HammerComp);
}

// Called when the game starts or when spawned
void AThor::BeginPlay()
{
	Super::BeginPlay();

	ThorAnimIns = Cast<UThorAnimInstance>(GetSkeletalMesh()->GetAnimInstance());
	ThorAnimIns->OnMontageEnded.AddDynamic(this , &ThisClass::OnMontageEnded);
	
	Target = Cast<AKratosCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if ( Target ) { UE_LOG(LogTemp , Warning , TEXT("Is Target")); }

	if ( ThorHammerClass )
	{
		ThorHammer = Owner->GetWorld()->SpawnActor<AThorHammer>(ThorHammerClass);
		ThorHammer->SetActorLocation(FVector(-1000));
	}
	
	ThorHammer->Thor = this;

	InitPatternClass();
	//토르 맵 판단크기
	DrawDebugSphere(GetWorld() , GetActorLocation() , MapSize , 12 , FColor::Blue , false , 15.0f , 0);


	WeaponEffect->Deactivate();
}

// Called every frame
void AThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( Hp <= 0 ) { return; }

	if ( CurPattern )
	{
		CurPattern->TickPattern();
	}

	//만약 데미지를 받은 상태 라면
	if ( IsHit )
	{
		if ( CurPattern )
		{
			//현재 패턴에서 HIT상태로 변환가능한지
			if ( CurPattern->IsHitable() && GetPattern(EThorPattern::HIT)->IsHitable())
			{
				//CurPattern->StopPattern();
				StartPattarn(EThorPattern::HIT);
			}
		}
		IsHit = false;
	}
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


void AThor::SetCharacterState(EWOG_Character_State NewState)
{

}

void AThor::TakeKDamage( const FWOG_DamageEvent& DamageEvent, ICombatInterface* DamageCauser)
{
	if ( Hp <= 0 ) { return; }
	
	float DamageValue = DamageEvent.DamageValue;

	if ( GEngine )
	{
		GEngine->AddOnScreenDebugMessage(-1 , 5.0f , FColor::Red , FString::Printf(TEXT("Thor : Damage Value: %f") , DamageValue));
	}

	IsHit = true;

	Hp -= DamageValue;
	StunValue += 5;

	if ( UpdateHp.IsBound() )UpdateHp.Execute();
	if ( UpdateStun.IsBound() )UpdateStun.Execute();


	
}

USkeletalMeshComponent* AThor::GetSkeletalMesh()
{
	return BodyComp;
}

void AThor::TestDamageEvnet(AActor* DamageTarget)
{
	ICombatInterface* Interface = Cast<ICombatInterface>(DamageTarget);
	if ( Interface )
	{
		FWOG_DamageEvent DamageEvent;
		DamageEvent.DamageValue = 10;
		DamageEvent.StunValue = 10;
		DamageEvent.HitPoint = DamageTarget->GetActorLocation();

		Interface->TakeKDamage(DamageEvent , this);
	}
}

void AThor::InitPatternClass()
{
	if (!IdlePatternClass)
	{
		UE_LOG(LogTemp, Error, TEXT("No IdlePatternClass"));
	}

	UThorPattern* ThorIdlePattern = NewObject<UThorPattern>(this, IdlePatternClass);
	ThorIdlePattern->InitPattern(this);
	Patterns.Add(EThorPattern::IDLE, ThorIdlePattern);

	UThorPattern* ThorHitPattern = NewObject<UThorPattern>(this , HitPatternClass);
	ThorHitPattern->InitPattern(this);
	Patterns.Add(EThorPattern::HIT , ThorHitPattern);


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

	StartPattarn(EThorPattern::IDLE);
}


UThorPattern* AThor::GetPattern(EThorPattern PatternType)
{
	//등록된 패턴 가져오기
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
	if ( CurPattern )
	{
		CurPattern->StopPattern();
	}

	StartPattarn(NewPattern);

}

void AThor::EndPattarn(EThorPattern NewPattern)
{
	UThorPattern* End= GetPattern(NewPattern);
	if (!End) return;

	End->EndPattern();

}

void AThor::NotifyEventPattern(int32 EventIndex)
{
	if (!CurPattern) { return; }
	CurPattern->NotifyEventPattern(EventIndex);
}

void AThor::NotifyBeginPattern(int32 EventIndex,float TotalDuration)
{
	if (!CurPattern) { return; }
	CurPattern->NotifyBeginPattern(EventIndex ,TotalDuration);
}

void AThor::NotifyEndPattrern(int32 EventIndex)
{
	if (!CurPattern) { return; }
	CurPattern->NotifyEndPattrern(EventIndex);
}

void AThor::NotifyTickPattrern(int32 EventIndex, float FrameDeltaTime)
{
	if (!CurPattern) { return; }
	CurPattern->NotifyTickPattrern(EventIndex , FrameDeltaTime);
}

void AThor::OnMontageEnded(UAnimMontage* Montage , bool bInterrupted)
{
	if ( bInterrupted )
	{
		if ( Montage->bLoop ) { return; }
		UE_LOG(LogTemp , Warning , TEXT("몽타주가 강제 종료됨!"));
		
	}
	else
	{
		UE_LOG(LogTemp , Log , TEXT("몽타주가 정상적으로 끝남!"));
		if ( !CurPattern->IsEndPattern() ) { return; }

		EndPattarn(CurPattern->ThorPattern);
	}
}

FVector AThor::GetMoveRandomPos(FVector SpawnPos , float Dist)
{
	FVector Result = FVector::ZeroVector;
	MapSize /= 2;
	FVector2D RandPos = FMath::RandPointInCircle(1) * Dist;
	Result.X = SpawnPos.X + RandPos.X;
	Result.Y = SpawnPos.Y + RandPos.Y;

	Result.X = FMath::Clamp(Result.X , -MapSize , MapSize);
	Result.Y = FMath::Clamp(Result.Y , -MapSize , MapSize);
	
	return Result;

}

void AThor::SetIdleTimer(float IdleTimer)
{
	GetPattern(EThorPattern::IDLE)->SetOptionValue(IdleTimer);
	
}

void AThor::ShowHammer(bool bShow)
{
	IsHammer = bShow;
	
	HammerComp->SetVisibility(bShow);

	
}

void AThor::ShowHammerEffect(bool bShow)
{
	if ( bShow )
	{
		WeaponEffect->Activate();
	}
	else
	{
		WeaponEffect->Deactivate();
	}
}

void AThor::TestKDamaged()
{
	IsHit = true;
}

void AThor::ToggleAI()
{
	auto	a = Cast<UThor_Idle>(GetPattern(EThorPattern::IDLE));
	if ( a )
	{
		a->IsAI = !( a->IsAI );
	}
}




