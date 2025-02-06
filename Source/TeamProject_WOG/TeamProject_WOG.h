// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/EngineTypes.h" 

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

namespace EKCollisionProfileName
{
	static const FName Player = FName("Player");
	static const FName Enemy = FName("Enemy");
	static const FName PlayerAttack = FName("PlayerAttack");
	static const FName EnemyAttack = FName("EnemyAttack");
}

namespace EWOGTraceChannel
{
    static const ECollisionChannel PlayerAttackTrace = ECollisionChannel::ECC_GameTraceChannel5;
    static const ECollisionChannel EnemyAttackTrace = ECollisionChannel::ECC_GameTraceChannel6;
}


UENUM(BlueprintType)
enum class EThorPattern : uint8
{
    NONE ,
    SPAWN ,
    IDLE ,
    MOVE ,
    DASH ,
    //ATTACK_ACTION
    ATTACK_TRIPLE_STRIKE_COMBO ,
    ATTACK_PUNCH_AND_KICK_COMBO ,
    ATTACK_THUNDER_CLAP ,
    ATTACK_TRIPLE_HAMMER_STRIKES ,
    ATTACK_TRIPLE_LIGHTING_STRIKES ,
    //DAMGE_ACTION
    HIT ,
    STUN ,
    DEATH ,

    MAX ,
};
