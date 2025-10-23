// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControllData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ABComboAttackData.h"
#include "Physics/ABCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(CPROPILE_ABCAPSULE);

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UABCharacterControllData>
		ShoulderDataRef(TEXT("/Game/ArenaBattle/CharacterController/ABC_Shoulder.ABC_Shoulder"));

	if (ShoulderDataRef.Succeeded() == true)
	{
		CharacterControllManager.Add(
			ECharacterControlType::Shoulder,
			ShoulderDataRef.Object
		);
	}

	static ConstructorHelpers::FObjectFinder<UABCharacterControllData>
		QuaterDataRef(TEXT("/Game/ArenaBattle/CharacterController/ABC_Quater.ABC_Quater"));

	if (QuaterDataRef.Succeeded() == true)
	{
		CharacterControllManager.Add(
			ECharacterControlType::Quater,
			QuaterDataRef.Object
		);
	}

	//몽타주 에셋 설정.
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack"));

	if (ComboActionMontageRef.Succeeded() == true)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UABComboAttackData> ComboActionDataRef(TEXT("/Game/ArenaBattle/ComboAction/ABA_ComboAction.ABA_ComboAction"));

	if (ComboActionDataRef.Succeeded() == true)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		DeadMontageRef(TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead"));

	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

void AABCharacterBase::AttackHitCheck()
{
	//애님노티파이를 통해 함수가 호출됨.
	//충돌 판정 로직.
	FVector Start
		= GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	//공격의 길이(트레이스에 사용할 거리)
	const float AttackRange = 50.0f;
	FVector End = Start + AttackRange;

	//공격의 범위(트레이스에 사용할 반지름 값)
	const float AttackRadius = 50.0f;

	// SCENE_QUERY_STAT-FName 타입의 태그값 생성 매크로(엔진 내부에서 사용)
	// 두번째 인자: 복잡한 형태로 충돌체를 감지할지 여부 지정.
	// 세번째 인자: 충돌 판정에서 제외할 액터 목록. (자기자신 제외.)
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false,
		this
	);

	FHitResult OutHitResult;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity,
		CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius),Params);

	//충돌이 감지된 경우의 처리.
	if (HitDetected == true)
	{
		//데미지양
		const float AttackDamage = 30.0f;

		//데미지 이벤트(구조체)
		FDamageEvent DamageEvent;

		OutHitResult.GetActor()->TakeDamage(
			AttackDamage, DamageEvent,GetController(), this);
	}
#ifdef ENABLE_DRAW_DEBUG
	//캡슐의 중심 위치.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	//캡슐 높이의 절반값.
	float CapsuleHalfHeight = AttackRange * 0.5;

	//색상 지정(그리기 색상)
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	
	//충돌 디버그 (시각적 도구 활용)
	DrawDebugCapsule(GetWorld(),CapsuleOrigin,CapsuleHalfHeight,AttackRadius
		,FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor,false,5.0f);

#endif
}

void AABCharacterBase::SetDead()
{
	//캐릭터 무브먼트 끄기.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//죽는 애니메이션 재생.
	PlayDeadAnimation();
	//콜리전 끄기.
	SetActorEnableCollision(false);
}

void AABCharacterBase::PlayDeadAnimation()
{
	//몽타주 재생.
	UAnimInstance* AnimInstance
		= GetMesh()->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		//이미 재생중인 몽타주가 있을 수 있으니 모두 종료.
		AnimInstance->StopAllMontages(0.0f);

		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(
			DeadMontage,
			PlayRate);


	}
}

void AABCharacterBase::SetCharacterControllData(const UABCharacterControllData* InCharacterControllData)
{
	bUseControllerRotationYaw
		= InCharacterControllData->bUseControllerRotationYaw;

	// CharacterMovement.
	GetCharacterMovement()->bOrientRotationToMovement
		= InCharacterControllData->bOrientRotationtoMovement;

	GetCharacterMovement()->bUseControllerDesiredRotation
		= InCharacterControllData->bUseControllerDesiredRotation;

	GetCharacterMovement()->RotationRate
		= InCharacterControllData->RotationRate;

}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator,DamageCauser);


	//@Test: 바로 죽음 처리.
	SetDead();

	return DamageAmount;


}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	//재생이 끝나서 다시 무브먼브 모드복구.
	//콤보 데이터 리셋.
	ensureAlways(CurrentCombo != 0);

	CurrentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;

	//계산된 인덱스 값 검증(어썰트)
	ensureAlways(ComboActionData->EFFectiveFrameCount.IsValidIndex(ComboIndex));

	//데이터 에셋에 설정된 프레임 값을 사용해 콤보 판정 구간의 시간 값 구하기.
	//콤보 판정 구간의 시간 값 구하기.
	//이유는 타이머에 쓰기 위해서

	float AttackSpeedRate = 1.0f; //애니메이션 재생 속도.

	//타이머에 쓰일 시간 => 바꿀 수 있는 시간을 프레임으로 나눈 후 재생속도로 다시 나눈 값.
	float ComboEffectTime =
		(ComboActionData->EFFectiveFrameCount[ComboIndex] / ComboActionData->FrameRate)
		/ AttackSpeedRate;

	//타이머 설정.

	if (ComboEffectTime > 0)
	{
		//시간은 월드가 관리함(시간 관리자를 통해)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, 
			&AABCharacterBase::ComboCheck,ComboEffectTime,false);
	}
}

void AABCharacterBase::ComboCheck()
{
	//타이머 핸들 초기화(재사용을 위해).
	ComboTimerHandle.Invalidate();

	//콤보 타이머 시간 전에 공격 입력이 들어왔는지 확인.
	if (bHasNextComboCommand == true)
	{
		//몽타주 객체 가져오기.
		UAnimInstance* AnimInstance
			= GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			//몽타주 섹션 점프.
			//ComboAttack1-> "ComboAttack2"
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

			//섹션 이름 만들기.
			FName NextSection =
				*FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

			//몽타주 점프.
			AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

			//타이머 재설정.
			SetComboCheckTimer();
			//콤보처리에 사용한 입력값도 초기화.
			bHasNextComboCommand = false;
		}
	}
}

void AABCharacterBase::ProcessComboCommand()
{
	//처음 공격할 때 처리.
	//몽타주 재생 요청.
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
	
	//공격이 이미 재생 중일 때는 타이머를 확인해서 콤보타이밍 확인.
	//타이머 핸들이 유효한지 여부로 판단가능함.
	if (ComboTimerHandle.IsValid() == false)
	{
		bHasNextComboCommand = false; //유효하지 않을 경우 bHasNextComboCommand는 false로
		//즉 유효하지 않으면 다음 콤보로 넘어갈 수 없음
	}
	else
	{
		bHasNextComboCommand = true; //유효할 경우 bHasNexComboCommand는 true로
		//즉 만약에 유효하다면 다음 콤보로 넘어갈 수 있음
	}
	//== bHasNextComboCommand = ComboTimerHandle.IsVaild();
}

void AABCharacterBase::ComboActionBegin()
{
	//현재 콤보를 1단계로 설정.
	CurrentCombo = 1;

	//이동 막기. (무브먼트 모드를 None으로 설정.)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//몽타주 재생.
	if (AnimInstance != nullptr)
	{
		const float AttackSpeedRate = 1.0f;

		AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
	}

	//몽타주 재생끝나면 다시 움직이게.
	//몽타주 재생이 끝나면 이 클래스의 특정 함수 실행.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboActionMontage);

	//콤보 타이밍 확인용 타이머 설정.
	//기존에 설정된 타이머핸들 무효화(초기화).
	ComboTimerHandle.Invalidate();
	//타이머 설정 및 콤보 단계처리.
	SetComboCheckTimer();
}
