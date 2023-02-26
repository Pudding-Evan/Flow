// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "FlowAnimState.h"
#include "FlowAnimSetting.h"
#include "FlowAnimInstance.generated.h"


class AFlowCharacterBase;

USTRUCT(BlueprintType)
struct FAnimCurves
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Walk;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Run;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Crouch;
};

class UFlowCharacterMovementComponent;
/**
 * 
 */
UCLASS(Transient, Blueprintable)
class FLOW_API UFlowAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

    /*
     * State
     */

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
    TObjectPtr<AFlowCharacterBase> Character;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
    TObjectPtr<UFlowCharacterMovementComponent> CharMoveComp;

    UPROPERTY(BlueprintReadWrite, Category = "Data|Grounded")
    FFlowGroundedState GroundedState;

    UPROPERTY(BlueprintReadWrite, Category = "Data|Ground")
    FFlowLocomotionAnimState LocomotionState;

    UPROPERTY(BlueprintReadWrite, Category = "Base")
    EGait Gait;

    UPROPERTY(BlueprintReadWrite, Category = "Data|Rotation")
    EFlowRotaionMode RotationMode;


    /*
     * Setting
     */

    UPROPERTY(BlueprintReadWrite, Category = "Data|Setting")
    FGroundedSetting GroundedSetting;


public:

    virtual void NativeInitializeAnimation() override;

    virtual void NativeBeginPlay() override;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Ground")
    FAnimCurves AnimInstanceCurves;

    UFUNCTION(BlueprintCallable, Category = "Default")
    UFlowCharacterMovementComponent* GetCharacterMovementComponent() const;

protected:

    void RefreshLocomotionOnGameThread();

protected:
    void RefreshMovementState(float DeltaSeconds);

private:
    void RefreshSprint(float DeltaSeconds);
    void RefreshMovementDirection();

    void RefreshVelocityBlendState();

    float CalculateWalkOrRunBlend() const;
    float CalculateStrideBlend() const;
    float CalculatePlayRateBlend() const;


	static EFlowMovementDirection CalculateMovementDirection(const float Angle,const float ForwardHalfAngle,const float AngleThreshold);

};
