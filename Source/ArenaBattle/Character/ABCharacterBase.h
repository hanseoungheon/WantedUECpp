// Fill out your copyright notice in the Description page of Project Settings.

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

	//콤보공격이 끝날떄 실행되는함수,
	void ComboActionEnd(UAnimMontage* TargetMontage, bool Interrupted);
protected:
	//컨트롤 데이터 설정함수.
	virtual void SetCharacterControllData(const class UABCharacterControllData* InCharacterControllData);

	//공격 처음 시작할 떄와 콤보액션을 진행할 떄 실행.
	void ProcessComboCommand();

	//콤보액션이 시작될 때 실행.
	void ComboActionBegin();

protected:

	//열거형 타입 - UABCharacterControllData에셋을 짝(Pair)로 관리하는 맵.
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, UABCharacterControllData*> CharacterControllManager;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ComboActionMontage;
};
