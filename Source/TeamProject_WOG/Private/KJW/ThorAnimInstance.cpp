// Fill out your copyright notice in the Description page of Project Settings.


#include "KJW/ThorAnimInstance.h"
#include "KJW/Thor.h"

void UThorAnimInstance::NativeInitializeAnimation()
{
	APawn* owner = TryGetPawnOwner();
	if (owner)
	{
		Thor = Cast<AThor>(owner);
		if (Thor)
		{
		
		}
	}
	
}
