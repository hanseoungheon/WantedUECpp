// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "Interface/ABCharacterItemInterface.h"
#include "ABCharacterBase.generated.h"
//캐릭터 컨트롤 타입을 지정하는 열거형.

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UABItemData* /*InItemData*/);

//델리게이트 다수를 배열로 관리하기 위한 구조체.

USTRUCT()
struct FTakeItemDelegateWrappers
{
	GENERATED_BODY()

	FTakeItemDelegateWrappers()
	{
	}

	FTakeItemDelegateWrappers(
		const FOnTakeItemDelegate& InItemDelegate)
		: ItemDelegate(InItemDelegate)
	{
	}

	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface,
	public IABCharacterWidgetInterface, public IABCharacterItemInterface
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AABCharacterBase();
	
	//컴포넌트 초기화 끝나면 호출되는 이벤트.
	virtual void PostInitializeComponents() override;

	virtual void AttackHitCheck() override;

	//IABCHaracterWidget 함수 구현.
	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;

	//IABCharacterItemIntercace 함수 구현.
	virtual void TakeItem(class UABItemData* InItemData) override;

	//콤보공격이 끝날떄 실행되는함수,
	void ComboActionEnd(UAnimMontage* TargetMontage, bool Interrupted);

	//콤보 타이밍 계산 처리에 사용할 함수.
	void SetComboCheckTimer();

	//콤보 타이밍 처리 함수.
	//타이머에 설정된 시간이 경과했을 때 실행됨.
	//이 시간전에 공격 입력이 제대로 들어왔는지 판단 후 콤보 처리 분기.
	void ComboCheck();

	//Dead 섹션.
protected:
	//죽음 설정 함수.
	//데미지 처리 후 체력 소진되면 죽음.
	//죽음 상태 설정.
	virtual void SetDead();

	//죽음 애니메이션 재생.
	void PlayDeadAnimation();
protected:
	//컨트롤 데이터 설정함수.
	virtual void SetCharacterControllData(const class UABCharacterControllData* InCharacterControllData);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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

	//콤보 처리를 위한 데이터를 가지는 데이터 에셋.
	UPROPERTY(VisibleAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboAttackData> ComboActionData;

	//죽는 몽타주.
	UPROPERTY(VisibleAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	//죽은 뒤에 대기 시간. (이 시간 이후에 캐릭터 삭제함)
	float DeadEventDelayTime = 5.0f;

	//현재 콤보 단계 추적 변수.
	UPROPERTY(VisibleAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	uint32 CurrentCombo = 0;

	//콤보판정에 사용할 타이머.
	FTimerHandle ComboTimerHandle;

	// 콤보 점프를 판정할 때 사용할 Bool 변수
	UPROPERTY(VisibleAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	bool bHasNextComboCommand = false;
	
	//Stat 섹션.
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = CharacterControl, 
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMyCharacterStat> Stat;

	//Widget 섹션.
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterControl, 
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;


	//ItemSection
protected:
	//무기 획득시 사용할 스켈레탈 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrappers> TakeItemActions;

	virtual void DrinkPotion(class UABItemData* InItemData);
	virtual void EquipWeapon(class UABItemData* InItemData);
	virtual void ReadScroll(class UABItemData* InItemData);

};
