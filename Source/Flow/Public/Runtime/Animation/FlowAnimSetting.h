#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FlowAnimSetting.generated.h"

USTRUCT(BlueprintType)
struct FAnimCurvesSetting
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

USTRUCT(BlueprintType)
struct FGroundedSetting
{
    GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Ground")
    float AnimatedWalkSpeed = 150.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Ground")
    float AnimatedRunSpeed = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data|Ground")
    float AnimatedSprintSpeed = 650.0f;
};