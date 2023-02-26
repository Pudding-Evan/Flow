#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowCharacterSetting.generated.h"

class UCurveVector;
USTRUCT(Blueprintable,BlueprintType)
struct FGaitSetting
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    float WalkSpeed{ 175.0f};

    UPROPERTY(EditDefaultsOnly)
    float RunSpeed{ 375.0f };

    UPROPERTY(EditDefaultsOnly)
    float SprintSpeed{ 650.0f };

    // Gait amount to acceleration, deceleration, and ground friction curve.
    // Gait amount ranges from 0 to 3, where 0 is stopped, 1 is walking, 2 is running, and 3 is sprinting.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UCurveVector> AccelerationAndDecelerationAndGroundFrictionCurve{ nullptr };
};

USTRUCT(Blueprintable,BlueprintType)
struct FMovementSetting_Stance
{
    GENERATED_BODY()
public:

    UPROPERTY(EditDefaultsOnly)
    FGaitSetting Stance;

    UPROPERTY(EditDefaultsOnly)
    FGaitSetting Crouch;
};

USTRUCT(Blueprintable, BlueprintType)
struct FLOW_API FMovementSeting_State
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FMovementSetting_Stance Normal;

    UPROPERTY(EditDefaultsOnly)
    FMovementSetting_Stance Aimming;
};
