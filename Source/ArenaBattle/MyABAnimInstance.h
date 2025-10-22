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
    // �ִϸ��̼� �ʱ�ȭ�� �� ����Ǵ� �Լ�.
    virtual void NativeInitializeAnimation() override;

    // �ִϸ��̼� ������Ʈ�� �� ������ ���� ����Ǵ� �Լ�.
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;


    // �ִϸ��̼� �������Ʈ���� ����� ����.
protected:

    // ���� ����.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class ACharacter> Owner;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    TObjectPtr<class UCharacterMovementComponent> Movement;

    //�̵��ӵ� ��. (�����Ʈ ������Ʈ���� �б�.)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    FVector Velocity;

    //�̵��ӷ�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float GroundSpeed;

    //�̵��ϴ��� �����ִ��� Ȯ���ϴ� ����.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsIdle : 1;

    //�̵� ���θ� �Ǵ��� �� ����� ���� ��.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float MovingThreshold;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsFalling : 1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    uint8 bIsJumping : 1;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float JumpThershold;
};
