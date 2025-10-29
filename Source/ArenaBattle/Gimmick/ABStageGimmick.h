// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABStageGimmick();

protected:

public:

	//Stage Sections
protected:
	//스테이지 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AlloPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent>Stage;

	//입장 처리를 위한 박스 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AlloPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent>StageTrigger;

	//박스 컴포넌트의 오버랩 이벤트.
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	//Gate Section.
protected:

	//문은 4개가 필요함. 방향에 따라 분리하기 위해서 Map으로 구성.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AlloPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	//각 문에 배치할 박스 컴포넌트.
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AlloPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	//각 문의 충돌 이벤트가 발생했을 때 실행될 함수.
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
