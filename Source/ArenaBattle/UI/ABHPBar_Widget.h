// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHPBar_Widget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHPBar_Widget : public UABUserWidget
{
	GENERATED_BODY()

public:
	UABHPBar_Widget(const FObjectInitializer& ObjectInitializer);

	//MaxHp 세터
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }

	//HpBar에 게이지 설정을 위해 설정할 함수.
	void UpdateHpBar(float NewCurrentHp);


protected:
	//위젯이 모두 초기화가 된 후에 호출되는 함수.
	//위젯을 참조할 때 생성됐다는 것을 보장 받을 수 있음.
	virtual void NativeConstruct() override;
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
