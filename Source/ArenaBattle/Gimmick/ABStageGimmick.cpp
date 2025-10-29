// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"

// Sets default values
AABStageGimmick::AABStageGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Stage Section.
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	//@Todo: 스테이지 메시 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE"));
	if (StageMeshRef.Succeeded() == true)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	//박스 콜리전 컴포넌트 설정.
	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetupAttachment(Stage);
	//박스 콜리전 크기 설정.
	StageTrigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	//상대 위치 설정.
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	//콜리전 설정.
	StageTrigger->SetCollisionProfileName(CPROPILE_ABTRIGGER);
	//오버랩 이벤트에 함수 등록.
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, 
		&AABStageGimmick::OnStageTriggerBeginOverlap);

	//Gate Section.
	//문 위치를 나타내는 소켓 이름 값.
	static FName GateSockets[] =
	{
		TEXT("+XGate"),
		TEXT("-XGate"),
		TEXT("+YGate"),
		TEXT("-YGate")
	};

	//@Todo: 메시 로드 및 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		GateMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE"));

	for (const FName& GateSocket : GateSockets)
	{
		UStaticMeshComponent* Gate
			= CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);

		if (GateMeshRef.Succeeded() == true)
		{
			Gate->SetStaticMesh(GateMeshRef.Object);
		}

		//계층 설정.
		Gate->SetupAttachment(Stage, GateSocket);

		//상대 위치 설정.
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f));

		//상대 회전 설정.
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

		//Gate의 콜리전.
		FName TriggerName = *FString::Printf(TEXT("%sTrigger"), *GateSocket.ToString());
		//FName TriggetName = *GateSocket.ToString().Append(TEXT("Trigger"));

		UBoxComponent* GateTrigger
			= CreateDefaultSubobject<UBoxComponent>(TriggerName);

		//계층 설정.
		GateTrigger->SetupAttachment(Stage, GateSocket);
		//크기 지정.
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		//상대 위치 설정.
		GateTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
		//콜리전 설정.
		GateTrigger->SetCollisionProfileName(CPROPILE_ABTRIGGER);
		//오버랩 이벤트에 함수 등록.
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);

		//배열에 추가.
		GateTriggers.Add(GateTrigger);
		
		//배열에 추가.
		Gates.Add(GateSocket,Gate);
	}
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
