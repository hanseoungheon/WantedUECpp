// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ABCharacterControllData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	// 기본 컴포넌트 설정.
	// 컨트롤러 회전 값을 받아서 사용하는 옵션 비활성화.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// 무브먼트 설정.
	// 이동할 때 이동하는 방향과 회전 방향을 맞추는 옵션 설정.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;

	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	// 애셋 지정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> CharaterAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/ArenaBattle/Animation/ABP_ABCharacter.ABP_ABCharacter_C'"));
	if (CharaterAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharaterAnim.Class);
	}

	// SpringArm 컴포넌트 생성 및 설정.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	// Camera 컴포넌트 생성 및 설정.
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// 입력 설정.
	//static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextRef(TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default"));
	//if (DefaultMappingContextRef.Succeeded())
	//{
	//	DefaultMappingContext = DefaultMappingContextRef.Object;
	//}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShouldMoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (ShouldMoveActionRef.Succeeded())
	{
		ShoulderMoveAction = ShouldMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (LookActionRef.Succeeded())
	{
		ShoulderLookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QuaterMoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (QuaterMoveActionRef.Succeeded())
	{
		QuaterMoveAction = QuaterMoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Attack.IA_Attack"));
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeControlActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (ChangeControlActionRef.Succeeded())
	{
		ChangeControlAction = ChangeControlActionRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Quater;
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent 
		= Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent != nullptr)
	{
		//입력 바인딩하기

		EnhancedInputComponent->BindAction(ShoulderMoveAction,ETriggerEvent::Triggered,this,&AABCharacterPlayer::ShoulderMove); //움직임
		EnhancedInputComponent->BindAction(ShoulderLookAction,ETriggerEvent::Triggered,this,&AABCharacterPlayer::ShoulderLook); //쳐다보기
		EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this,&AABCharacterPlayer::QuaterMove); //점프
		EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this,&AABCharacterPlayer::ChangeCharacterControl); //점프
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ACharacter::Jump); //점프
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Attack);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ACharacter::StopJumping); //점프 종료
	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharcterControlType)
{
	//해시테이블 사용.
	UABCharacterControllData* NewCharacterControl 
		= CharacterControllManager[NewCharcterControlType];

	check(NewCharacterControl);

	//데이터 에셋을 전달해 함수 호출하기.
	SetCharacterControllData(NewCharacterControl);

	// 향상된 입력 시스템 객체 얻어오기.
	APlayerController* PlayerController
		= Cast<APlayerController>(GetController()); //플레이어 컨트롤러 가져오기.

	UEnhancedInputLocalPlayerSubsystem* InputSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

	//사용할 입력 매핑 컨텍스트 등록하기.
	if (InputSystem != nullptr)
	{
		//기존에 설정된 매핑제거.
		InputSystem->ClearAllMappings();

		//새로운 입력매핑 컨텍스트 추가.
		InputSystem->AddMappingContext(NewCharacterControl->InputMappingContext, 0);
	}
}

void AABCharacterPlayer::SetCharacterControllData(const UABCharacterControllData* InCharacterControllData)
{
	Super::SetCharacterControllData(InCharacterControllData);


	//SpringArm 관련 설정해주기.
	SpringArm->TargetArmLength
		= InCharacterControllData->TargetArmLength;

	SpringArm->SetRelativeRotation(InCharacterControllData->RelativeRotation);


	SpringArm->bDoCollisionTest
		= InCharacterControllData->bDoCollisionTest;

	SpringArm->bUsePawnControlRotation
		= InCharacterControllData->bUsePawnControlRotation;

	SpringArm->bInheritPitch
		= InCharacterControllData->bInheritPitch;

	SpringArm->bInheritYaw
		= InCharacterControllData->bInheritYaw;

	SpringArm->bInheritRoll
		= InCharacterControllData->bInheritRoll;

}

void AABCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//입력값 읽어오기.
	FVector2D Movement = Value.Get<FVector2D>();

	//이동할 방향 만들기.
	//카메라가 바라보는 방향(컨트롤러의 방향)을 기준으로 이동 방향 만들기.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
	
	//앞방향.
	FVector ForwardVector =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//오른쪽 방향.
	FVector RightVector =
		FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	//무브먼트에 입력 전달하기.
	//크기가 1인 숫자를 곱하는 행위는 기하학에서는 "회전"임.
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);

}


void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	//입력 값 읽어오기.
	FVector2D LookValue = Value.Get<FVector2D>();

	//컨트롤러에 회전 적용.
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput((- 1.0f)* LookValue.Y); //Y축 반전.

	//무브몬트에 입력 전달하기
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	FVector MoveDirection(Movement.Y, Movement.X, 0.0f);

	//입력의 크기.
	float MovementVectorSize = FMath::Min(1.0f,Movement.Size()); //1을 넘어가면 1.0을 고른다.

	//이동 단위 벡터 만들기.(1.4141414가 되니깐 1로 고정)
	MoveDirection.Normalize();

	//Controller->SetControlRotation(
	//);


	//이동적용시키기.
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AABCharacterPlayer::QuaterLook(const FInputActionValue& Value)
{
}

void AABCharacterPlayer::Attack()
{
	ProcessComboCommand();
}
