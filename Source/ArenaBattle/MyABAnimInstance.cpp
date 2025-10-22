// Fill out your copyright notice in the Description page of Project Settings.


#include "MyABAnimInstance.h"
#include "GameFramework\Character.h"
#include "GameFramework\CharacterMovementComponent.h"

UMyABAnimInstance::UMyABAnimInstance()
{
	//이동을 판단할 때 사용할 기준 값 설정.
	MovingThreshold = 3.0f; //초당 3cm

	//점프중인지 판단할 기준 값 설정.
	JumpThershold = 100.0f;
}

void UMyABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());

	if (Owner != nullptr)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMyABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
		
	//애니메이션 재생에 사용될 값 설정하기.
	if (Movement != nullptr)
	{
		//속도 구하기.
		Velocity = Movement->Velocity;

		//지면서에서의 이동 빠르게 구하기.
		GroundSpeed = Velocity.Size2D();

		// 이동/정지 상태 설정.
		bIsIdle = GroundSpeed < MovingThreshold;

		//공중에 떠 있는지 확인.
		bIsFalling = Movement->IsFalling();

		//점프중인지 판단.
		bIsJumping = bIsFalling & (Velocity.Z > JumpThershold);
	}
}
