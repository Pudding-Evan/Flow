// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FlowAnimState.h"
#include "FlowAnimSetting.h"
#include "Runtime/Interface/FlowCharacterStateInterface.h"
#include "FlowAnimInstance.generated.h"

class AFlowCharacterBase;

USTRUCT(BlueprintType)
struct FAnimCurves
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Walk{nullptr};

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Run{ nullptr };

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UCurveFloat> StrideBlend_N_Crouch{ nullptr };
};

class UFlowCharacterMovementComponent;
/**
 * 
 */
UCLASS(Transient, Blueprintable)
class FLOW_API UFlowAnimInstance :
	public UAnimInstance,
    public IFlowCharacterStateInterface
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

    // Interface

    UPROPERTY(BlueprintReadWrite, Category = "Base")
    EGait Gait;

    virtual EGait GetGait() const override { return Gait; }

    virtual void SetGait_Implementation(EGait DesiredGait) override;

    UPROPERTY(BlueprintReadWrite, Category = "Base")
    EFlowRotaionMode RotationMode;

    virtual EFlowRotaionMode GetRotationMode() const override { return RotationMode; }

    virtual void SetRotationMode_Implementation(EFlowRotaionMode DesiredRotationMode) override;

    UPROPERTY(BlueprintReadWrite, Category = "Base")
    EStance Stance;

    virtual EStance GetStance() const override { return Stance; }

    virtual void SetStance_Implementation(EStance DesiredStance) override;


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
