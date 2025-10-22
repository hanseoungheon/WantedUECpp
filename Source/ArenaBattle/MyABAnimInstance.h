// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UMyABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyABAnimInstance();

protected:
    // 애니메이션 초기화할 때 실행되는 함수.
    virtual void NativeInitializeAnimation() override;

    // 애니메이션 업데이트할 때 프레임 마다 실행되는 함수.
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;


    // 애니메이션 블루프린트에서 사용할 변수.
protected:

    // 참조 변수.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class ACharacter> Owner;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class UCharacterMovementComponent> Movement;

    //이동속도 값. (무브먼트 컴포넌트에서 읽기.)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    FVector Velocity;

    //이동속력
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float GroundSpeed;

    //이동하는지 멈춰있는지 확인하는 변수.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsIdle : 1;

    //이동 여부를 판단할 때 사용할 문턱 값.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float MovingThreshold;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsFalling : 1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsJumping : 1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float JumpThershold;
};
