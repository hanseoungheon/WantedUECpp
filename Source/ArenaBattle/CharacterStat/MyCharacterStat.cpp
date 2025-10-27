// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/MyCharacterStat.h"


// Sets default values for this component's properties
UMyCharacterStat::UMyCharacterStat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UMyCharacterStat::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetHp(MaxHp);
}

float UMyCharacterStat::ApplyDamage(float InDamage)
{
	//데미지 처리 계산을 위해 이전 체력 저장하기.
	const float PrevHp = CurrentHp;

	//데미지값에 음수가 전달되는것 방지해서 변수로 만들기.
	//Clamp는 범위 지정용
	const float ActualDamage = FMath::Clamp(InDamage, 0.0f, InDamage); // - 값 방지.
	// => FMath::Max(InDamage,0.0f); 둘중 더 큰거 반환. 음수일경우 0 반환.

	//데미지 처리하기.
	SetHp(PrevHp - ActualDamage);

	//사망했는지 확인
	//KINDA_SMALL_NUMBER => 0으로 취급하는 매우 작은 수.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		//캐릭터 죽음처리.
		//지금 구조에서는 하위 계층인 컴포넌트가 직접
		//캐릭터에 접근해서 체력이 소진됐음을 알려야 함.
		//이를 방지하기 위해 델리게이트를 사용.
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UMyCharacterStat::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp(NewHp, 0.0f, MaxHp);

	//체력 변경 이벤트 발행.
	OnHpChanged.Broadcast(CurrentHp);
}




