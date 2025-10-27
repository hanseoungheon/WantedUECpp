// Fill out your copyright notice in the Description page of Project Settings.


#include "ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"

// Sets default values
AABItemBox::AABItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	//콜리전 프로필 설정.
	Trigger->SetCollisionProfileName(CPROPILE_ABTRIGGER);

	//콜리전 크기 설정.
	Trigger->SetBoxExtent(FVector(45.0f, 45.0f, 30.0f));

	//BeginOverlap 델리게이트에 함수 등록.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);

	//메시 에셋 로드 및 설정.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));

	if (BoxMeshRef.Succeeded() == true)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}



	//메시 위치 조정.
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	//파티클 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));

	if (EffectRef.Succeeded() == true)
	{
		Effect->SetTemplate(EffectRef.Object);

		Effect->bAutoActivate = false;
	}
}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//아이템 습득 후 처리 진행.

	if (Item == nullptr)
	{
		Destroy();
		return;
	}

	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);

	if (OverlappingPawn != nullptr)
	{
		OverlappingPawn->TakeItem(Item);
	}

	//파티클 재생.
	Effect->Activate();

	//메시 비활성화.
	Mesh->SetHiddenInGame(true);

	//콜리전 비활성화.
	SetActorEnableCollision(false);

	//파티클 재생 끝나면 발행되는 델리게이트에 등록.
	//파티클 재생 종료 후에 아이템 액터 제거 목록.
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);
}
 
void AABItemBox::OnEffectFinished(UParticleSystemComponent* PSystems)
{
	Destroy();
}



