// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControllData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControllData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UABCharacterControllData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;	

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bOrientRotationtoMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bDoCollisionTest : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	/** Should we inherit pitch from parent component. Does nothing if using Absolute Rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritPitch : 1;

	/** Should we inherit yaw from parent component. Does nothing if using Absolute Rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritYaw : 1;

	/** Should we inherit roll from parent component. Does nothing if using Absolute Rotation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpringArm)
	uint32 bInheritRoll : 1;
};
