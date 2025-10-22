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

	//몽타주 에셋 설정.
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
	//재생이 끝나서 다시 무브먼브 모드복구.

	GetCharacterMovement()->SetMovementMode(
		EMovementMode::MOVE_Walking);
}

void AABCharacterBase::ProcessComboCommand()
{
	//이동 막기. (무브먼트 모드를 None으로 설정.)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//몽타주 재생.
	if (AnimInstance != nullptr)
	{
		const float AttackSpeedRate = 1.0f;

		AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
	}

	//몽타주 재생끝나면 다시 움직이게.
	//몽타주 재생이 끝나면 이 클래스의 특정 함수 실행.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboActionMontage);
}

void AABCharacterBase::ComboActionBegin()
{

}
