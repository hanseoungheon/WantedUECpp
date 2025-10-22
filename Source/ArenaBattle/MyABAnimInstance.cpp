// Fill out your copyright notice in the Description page of Project Settings.


#include "MyABAnimInstance.h"
#include "GameFramework\Character.h"
#include "GameFramework\CharacterMovementComponent.h"

UMyABAnimInstance::UMyABAnimInstance()
{
	//�̵��� �Ǵ��� �� ����� ���� �� ����.
	MovingThreshold = 3.0f; //�ʴ� 3cm

	//���������� �Ǵ��� ���� �� ����.
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
		
	//�ִϸ��̼� ����� ���� �� �����ϱ�.
	if (Movement != nullptr)
	{
		//�ӵ� ���ϱ�.
		Velocity = Movement->Velocity;

		//���鼭������ �̵� ������ ���ϱ�.
		GroundSpeed = Velocity.Size2D();

		// �̵�/���� ���� ����.
		bIsIdle = GroundSpeed < MovingThreshold;

		//���߿� �� �ִ��� Ȯ��.
		bIsFalling = Movement->IsFalling();

		//���������� �Ǵ�.
		bIsJumping = bIsFalling & (Velocity.Z > JumpThershold);
	}
}
