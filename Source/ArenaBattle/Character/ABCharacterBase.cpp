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

	//��Ÿ�� ���� ����.
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
	//�ִԳ�Ƽ���̸� ���� �Լ��� ȣ���.
	//�浹 ���� ����.
	FVector Start
		= GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	//������ ����(Ʈ���̽��� ����� �Ÿ�)
	const float AttackRange = 50.0f;
	FVector End = Start + AttackRange;

	//������ ����(Ʈ���̽��� ����� ������ ��)
	const float AttackRadius = 50.0f;

	// SCENE_QUERY_STAT-FName Ÿ���� �±װ� ���� ��ũ��(���� ���ο��� ���)
	// �ι�° ����: ������ ���·� �浹ü�� �������� ���� ����.
	// ����° ����: �浹 �������� ������ ���� ���. (�ڱ��ڽ� ����.)
	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(Attack),
		false,
		this
	);

	FHitResult OutHitResult;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity,
		CCHANNEL_ABACTION, FCollisionShape::MakeSphere(AttackRadius),Params);

	//�浹�� ������ ����� ó��.
	if (HitDetected == true)
	{
		//��������
		const float AttackDamage = 30.0f;

		//������ �̺�Ʈ(����ü)
		FDamageEvent DamageEvent;

		OutHitResult.GetActor()->TakeDamage(
			AttackDamage, DamageEvent,GetController(), this);
	}
#ifdef ENABLE_DRAW_DEBUG
	//ĸ���� �߽� ��ġ.
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	//ĸ�� ������ ���ݰ�.
	float CapsuleHalfHeight = AttackRange * 0.5;

	//���� ����(�׸��� ����)
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	
	//�浹 ����� (�ð��� ���� Ȱ��)
	DrawDebugCapsule(GetWorld(),CapsuleOrigin,CapsuleHalfHeight,AttackRadius
		,FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor,false,5.0f);

#endif
}

void AABCharacterBase::SetDead()
{
	//ĳ���� �����Ʈ ����.
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	//�״� �ִϸ��̼� ���.
	PlayDeadAnimation();
	//�ݸ��� ����.
	SetActorEnableCollision(false);
}

void AABCharacterBase::PlayDeadAnimation()
{
	//��Ÿ�� ���.
	UAnimInstance* AnimInstance
		= GetMesh()->GetAnimInstance();

	if (AnimInstance != nullptr)
	{
		//�̹� ������� ��Ÿ�ְ� ���� �� ������ ��� ����.
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


	//@Test: �ٷ� ���� ó��.
	SetDead();

	return DamageAmount;


}

void AABCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool Interrupted)
{
	//����� ������ �ٽ� ����պ� ��庹��.
	//�޺� ������ ����.
	ensureAlways(CurrentCombo != 0);

	CurrentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;

	//���� �ε��� �� ����(���Ʈ)
	ensureAlways(ComboActionData->EFFectiveFrameCount.IsValidIndex(ComboIndex));

	//������ ���¿� ������ ������ ���� ����� �޺� ���� ������ �ð� �� ���ϱ�.
	//�޺� ���� ������ �ð� �� ���ϱ�.
	//������ Ÿ�̸ӿ� ���� ���ؼ�

	float AttackSpeedRate = 1.0f; //�ִϸ��̼� ��� �ӵ�.

	//Ÿ�̸ӿ� ���� �ð� => �ٲ� �� �ִ� �ð��� ���������� ���� �� ����ӵ��� �ٽ� ���� ��.
	float ComboEffectTime =
		(ComboActionData->EFFectiveFrameCount[ComboIndex] / ComboActionData->FrameRate)
		/ AttackSpeedRate;

	//Ÿ�̸� ����.

	if (ComboEffectTime > 0)
	{
		//�ð��� ���尡 ������(�ð� �����ڸ� ����)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, 
			&AABCharacterBase::ComboCheck,ComboEffectTime,false);
	}
}

void AABCharacterBase::ComboCheck()
{
	//Ÿ�̸� �ڵ� �ʱ�ȭ(������ ����).
	ComboTimerHandle.Invalidate();

	//�޺� Ÿ�̸� �ð� ���� ���� �Է��� ���Դ��� Ȯ��.
	if (bHasNextComboCommand == true)
	{
		//��Ÿ�� ��ü ��������.
		UAnimInstance* AnimInstance
			= GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			//��Ÿ�� ���� ����.
			//ComboAttack1-> "ComboAttack2"
			CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

			//���� �̸� �����.
			FName NextSection =
				*FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);

			//��Ÿ�� ����.
			AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);

			//Ÿ�̸� �缳��.
			SetComboCheckTimer();
			//�޺�ó���� ����� �Է°��� �ʱ�ȭ.
			bHasNextComboCommand = false;
		}
	}
}

void AABCharacterBase::ProcessComboCommand()
{
	//ó�� ������ �� ó��.
	//��Ÿ�� ��� ��û.
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
	
	//������ �̹� ��� ���� ���� Ÿ�̸Ӹ� Ȯ���ؼ� �޺�Ÿ�̹� Ȯ��.
	//Ÿ�̸� �ڵ��� ��ȿ���� ���η� �Ǵܰ�����.
	if (ComboTimerHandle.IsValid() == false)
	{
		bHasNextComboCommand = false; //��ȿ���� ���� ��� bHasNextComboCommand�� false��
		//�� ��ȿ���� ������ ���� �޺��� �Ѿ �� ����
	}
	else
	{
		bHasNextComboCommand = true; //��ȿ�� ��� bHasNexComboCommand�� true��
		//�� ���࿡ ��ȿ�ϴٸ� ���� �޺��� �Ѿ �� ����
	}
	//== bHasNextComboCommand = ComboTimerHandle.IsVaild();
}

void AABCharacterBase::ComboActionBegin()
{
	//���� �޺��� 1�ܰ�� ����.
	CurrentCombo = 1;

	//�̵� ����. (�����Ʈ ��带 None���� ����.)
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//��Ÿ�� ���.
	if (AnimInstance != nullptr)
	{
		const float AttackSpeedRate = 1.0f;

		AnimInstance->Montage_Play(ComboActionMontage, 1.0f);
	}

	//��Ÿ�� ��������� �ٽ� �����̰�.
	//��Ÿ�� ����� ������ �� Ŭ������ Ư�� �Լ� ����.
	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboActionMontage);

	//�޺� Ÿ�̹� Ȯ�ο� Ÿ�̸� ����.
	//������ ������ Ÿ�̸��ڵ� ��ȿȭ(�ʱ�ȭ).
	ComboTimerHandle.Invalidate();
	//Ÿ�̸� ���� �� �޺� �ܰ�ó��.
	SetComboCheckTimer();
}
