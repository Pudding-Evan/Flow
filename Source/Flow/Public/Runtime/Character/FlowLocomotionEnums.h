#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowLocomotionEnums.generated.h"

UENUM(BlueprintType)
enum class EFlowRotaionMode : uint8
{
    LookDirection UMETA(DisplayName = "LookDirection"),
    VelocityDirection UMETA(DisplayName = "VelocityDirection")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
    None UMETA(DisplayName = "None"),
    Grounded UMETA(DisplayName = "Grounded"),
    InAir UMETA(DisplayName = "InAir")
};

UENUM(BlueprintType)
enum class EStance : uint8
{
    Standing UMETA(DisplayName = "Standing"),
    Crouching UMETA(DisplayName = "Crouching"),
};

UENUM(BlueprintType)
enum class EGait : uint8
{
    Walking UMETA(DisplayName = "Walking"),
    Running UMETA(DisplayName = "Running"),
    Sprinting UMETA(DisplayName = "Sprinting")
};
