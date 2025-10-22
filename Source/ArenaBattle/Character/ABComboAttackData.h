// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UABComboAttackData();

public:
	//몽타주 섹션 이름 접두사(ComboAttack)
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	//최대 콤보 갯수 (4개)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint32 MaxComboCount;

	//프레임 재생 속도.
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	//콤보 공격 마다의 콤보 판정이 되는 프레임 값,
	//입력이 이 프레임 전에 잘 들어왔는지 확인.
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EFFectiveFrameCount;
};
