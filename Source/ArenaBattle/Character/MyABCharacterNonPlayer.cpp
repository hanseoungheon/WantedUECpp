// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyABCharacterNonPlayer.h"
#include "MyABCharacterNonPlayer.h"

AMyABCharacterNonPlayer::AMyABCharacterNonPlayer()
{
}

void AMyABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	//시간대기에 사용될 타이머핸들.
	FTimerHandle DeadTimerHandle;

	//타이머 델리게이트(람다함수)
	//설정한 시간이 경과했을때 실행할 델리게이트.
	auto Delegate = FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	);

	//타이머를 이용해서 삭제 처리.
	GetWorld()->GetTimerManager().SetTimer(
		DeadTimerHandle,
		Delegate,
		DeadEventDelayTime,
		false //루프 여부.
	);

}
