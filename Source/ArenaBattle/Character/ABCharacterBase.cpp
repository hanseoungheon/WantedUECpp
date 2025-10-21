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
