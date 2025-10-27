// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHPBar_Widget.h"
#include "Components/ProgressBar.h"

UABHPBar_Widget::UABHPBar_Widget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//캐릭터를 통해서 제대로 설정되야함.
	//시작될 때 음수로 설정해서 나중에 사용할 때 제대로 값이 설정됐는지 디버그하기 위한 용도.
	MaxHp = -1.0f;
}

void UABHPBar_Widget::UpdateHpBar(float NewCurrentHp)
{
	//값 검증용 디버그체커?
	ensureAlways(MaxHp > 0.0f);

	//프로그레스바 설정 여부확인.
	if (HpProgressBar != nullptr)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}

void UABHPBar_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	//위젯 참조를 위한 코드 작성.
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));

	ensureAlways(HpProgressBar);
}
