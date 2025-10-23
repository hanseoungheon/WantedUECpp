// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHit.h"
#include "Interface/ABAnimationAttackInterface.h"
#include "AnimNotify_AttackHit.h"

void UAnimNotify_AttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp,Animation,EventReference);

	//ĳ�������� ���������϶�� ����.

	if (MeshComp != nullptr)
	{
		IABAnimationAttackInterface* AttackPawn
		= Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());

		if (AttackPawn != nullptr)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
