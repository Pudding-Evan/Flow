#pragma once

#include "CoreMinimal.h"
#include "FlowCharacterState.generated.h"


USTRUCT(BlueprintType)
struct FLOW_API FFlowLocomotionState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
    bool bMoving{false};

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
    FVector Velocity{ForceInit};

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
    float VelocityYawAngle{0.0f}; // Velocity.Y / Velocity.X

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    FVector PreviousVelocity{ForceInit};

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
    float Speed{0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bHasSpeed{ false };

    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
    FVector Acceleration{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FVector MovementInputDirection{ 0.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FRotator LastMovementInputRotation{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bHasMovementInput{false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FRotator ViewRotation{ ForceInit };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    float ViewYawAngle{ 0.0f }; // Rotation Yaw Angle

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cache")
    float PreviousViewYawAngle{ 0.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FVector Location{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    FRotator Rotation{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FQuat RotationQuaternion{ForceInit};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
    float TargetYawAngle{ 0.0f };

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))
    float SmoothTargetYawAngle{ 0.0f };

    
};

