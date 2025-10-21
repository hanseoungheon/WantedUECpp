﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"

//캐릭터 컨트롤 타입을 지정하는 열거형.

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

protected:
	//컨트롤 데이터 설정함수.
	virtual void SetCharacterControllData(const class UABCharacterControllData* InCharacterControllData);

protected:

	//열거형 타입 - UABCharacterControllData에셋을 짝(Pair)로 관리하는 맵.
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, UABCharacterControllData*> CharacterControllManager;
};
