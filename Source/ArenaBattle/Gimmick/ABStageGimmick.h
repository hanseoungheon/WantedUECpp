// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate)

UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,	
	NEXT
};



USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()

	FStageChangedDelegateWrapper()
	{ 
	}

	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate)
	{
	}

	FOnStageChangedDelegate StageDelegate;
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();
		
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
public:

	//Stage Sections
protected:
	//스테이지 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent>Stage;

	//입장 처리를 위한 박스 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent>StageTrigger;

	UPROPERTY(EditAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState);

	//박스 컴포넌트의 오버랩 이벤트.
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//Gate Section.
protected:

	//문은 4개가 필요함. 방향에 따라 분리하기 위해서 Map으로 구성.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	//각 문에 배치할 박스 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	//각 문의 충돌 이벤트가 발생했을 때 실행될 함수.
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates();

	void CloseAllGates();

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();
};
