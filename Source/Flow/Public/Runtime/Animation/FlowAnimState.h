#pragma once

#include "CoreMinimal.h"
#include "Runtime/Character/FlowLocomotionEnums.h"
#include "FlowAnimState.generated.h"

USTRUCT(BlueprintType)
struct FLOW_API FFlowLocomotionAnimState
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
	bool bMoving{ false };

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    FVector Velocity{ForceInit};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    float VelocityYawAngle{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    float ViewYawAngle{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    float Speed{0.0f};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    FVector Acceleration{ForceInit};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    float MaxAcceleration{0.0f};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    float MaxBrakingDeceleration{0.0f};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    FVector Location{ForceInit};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    FRotator Rotation{ForceInit};

    UPROPERTY(BlueprintReadWrite, Category = "Locomotion")
    FQuat RotationQuaternion{ForceInit};


};

UENUM(BlueprintType)
enum class EFlowMovementDirection : uint8
{
    Forward,
    Backward,
    Left,
    Right
};

USTRUCT(BlueprintType)
struct FLOW_API FFlowMovementDirectionState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
    EFlowMovementDirection MovementDirection{EFlowMovementDirection::Forward};

private:
	UPROPERTY(BlueprintReadWrite, Category = "Direction", Meta = (AllowPrivateAccess))
	bool bForward{true};

	UPROPERTY(BlueprintReadWrite, Category = "Direction", Meta = (AllowPrivateAccess))
	bool bBackward{false};

	UPROPERTY(BlueprintReadWrite, Category = "Direction", Meta = (AllowPrivateAccess))
	bool bLeft{false};

	UPROPERTY(BlueprintReadWrite, Category = "Direction", Meta = (AllowPrivateAccess))
	bool bRight{false};

public:

	void operator=(const EFlowMovementDirection NewMovementDirection)
	{
        MovementDirection = NewMovementDirection;
        bForward = MovementDirection == EFlowMovementDirection::Forward;
        bBackward = MovementDirection == EFlowMovementDirection::Backward;
        bLeft = MovementDirection == EFlowMovementDirection::Left;
        bRight = MovementDirection == EFlowMovementDirection::Right;
	}
};

USTRUCT(BlueprintType)
struct FLOW_API FFlowVelocityBlendState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "VelocityBlend")
    float ForwardAmount{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "VelocityBlend")
    float BackwardAmount{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "VelocityBlend")
    float LeftAmount{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "VelocityBlend")
    float RightAmount{ 0.0f };

};


USTRUCT(BlueprintType)
struct FLOW_API FFlowGroundedState
{
	GENERATED_BODY()

    UPROPERTY( BlueprintReadWrite, Category = "Ground", Meta = (ClampMin = 0, ClampMax = 1))
    float WalkRunBlendAmount{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Ground", Meta = (ClampMin = 0, ClampMax = 1))
    float StrideBlendAmount{ 0.0f };

    UPROPERTY( BlueprintReadWrite, Category = "Ground", Meta = (ClampMin = 0, ForceUnits = "x"))
    float StandingPlayRate{ 1.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Ground", Meta = (ClampMin = 0, ForceUnits = "s"))
    float SprintTime{ 0.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Ground", Meta = (ClampMin = 0, ForceUnits = "x"))
    float SprintAccelerationAmount{ 1.0f };

    UPROPERTY(BlueprintReadWrite, Category = "Ground")
    FFlowMovementDirectionState MovementDirection;

    UPROPERTY(BlueprintReadWrite, Category = "Ground")
    FFlowVelocityBlendState VelocityBlendState;

};
