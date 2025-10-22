// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControllData.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UABCharacterControllData>
		ShoulderDataRef(TEXT("/Script/ArenaBattle.ABCharacterControllData'/Game/ArenaBattle/CharacterController/ABC_Shoulder.ABC_Shoulder'"));

	if (ShoulderDataRef.Succeeded() == true)
	{
		CharacterControllManager.Add(
			ECharacterControlType::Shoulder,
			ShoulderDataRef.Object
		);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControllData>
		QuaterDataRef(TEXT("/Script/ArenaBattle.ABCharacterControllData'/Game/ArenaBattle/CharacterController/ABC_Quater.ABC_Quater'"));

	if (QuaterDataRef.Succeeded() == true)
	{
		CharacterControllManager.Add(
			ECharacterControlType::Quater,
			QuaterDataRef.Object
		);
	}

	//��Ÿ�� ���� ����.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack'"));

	if (ComboActionMontageRef.Succeeded() == true)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}


}

void AABCharacterBase::SetCharacterControllData(const UABCharacterControllData* InCharacterControllData)
{
	bUseControllerRotationYaw
		= InCharacterControllData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement
		= InCharacterControllData->bOrientRotationtoMovement;

	GetCharacterMovement()->bUseControllerDesiredRotation
		= InCharacterControllData->bUseControllerDesiredRotation;

	GetCharacterMovement()->RotationRate
		= InCharacterControllData->RotationRate;

}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	//����� ������ �ٽ� ����պ� ��庹��.

	GetCharacterMovement()->SetMovementMode(
		EMovementMode::MOVE_Walking);
}

void AABCharacterBase::ProcessComboCommand()
{
	//�̵� ����. (�����Ʈ ��带 None���� ����.)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//��Ÿ�� ���.
	if (AnimInstance != nullptr)
	{
		const float AttackSpeedRate = 1.0f;

		AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
	}

	//��Ÿ�� ��������� �ٽ� �����̰�.
	//��Ÿ�� ����� ������ �� Ŭ������ Ư�� �Լ� ����.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboActionMontage);
}

void AABCharacterBase::ComboActionBegin()
{

}
