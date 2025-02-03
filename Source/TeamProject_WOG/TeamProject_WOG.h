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

enum class ETraceChannel : uint8
{
	PlayerAttackTrace = ECC_GameTraceChannel5 ,
	EnemyAttackTrace = ECC_GameTraceChannel6
};

