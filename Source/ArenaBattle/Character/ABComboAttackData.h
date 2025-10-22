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
	//��Ÿ�� ���� �̸� ���λ�(ComboAttack)
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	//�ִ� �޺� ���� (4��)
	UPROPERTY(EditAnywhere, Category = ComboData)
	uint32 MaxComboCount;

	//������ ��� �ӵ�.
	UPROPERTY(EditAnywhere, Category = ComboData)
	float FrameRate;

	//�޺� ���� ������ �޺� ������ �Ǵ� ������ ��,
	//�Է��� �� ������ ���� �� ���Դ��� Ȯ��.
	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EFFectiveFrameCount;
};
