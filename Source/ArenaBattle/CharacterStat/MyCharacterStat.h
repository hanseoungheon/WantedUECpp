// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCharacterStat.generated.h"

//체력 변경 관련 이벤트를 알리기 위한 델리게이트.
//1.체력을 모두 소진했을 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE(FOnHpzeroDelegate)
//2.체력이 변경됐을 때 발행할 델리게이트.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UMyCharacterStat : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCharacterStat();

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//Getter
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }

	//데미지 적용 함수.
	float ApplyDamage(float InDamage);

protected:
	//체력이 변경됐을 때 실행할 함수.
	void SetHp(float NewHp);
public:
	//발행할 이벤트.
	FOnHpzeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

protected:
	//최대 체력.
	UPROPERTY(VisibleAnywhere, Category = Stat)
	float MaxHp;

	//현재 체력.
	//현재 체력은 게임을 진행할때 마다 변경되기 때문에 저장이 필요하지않음.
	//그렇기에 임시값으로 지정해둘 수 있음 => Transient : 임시.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
};