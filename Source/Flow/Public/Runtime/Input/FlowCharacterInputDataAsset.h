// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FlowCharacterInputDataAsset.generated.h"

class UInputMappingContext;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterIMCInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 Priority;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext{nullptr};
	
};

UCLASS()
class FLOW_API UFlowCharacterInputDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "IMC")
	TArray<FCharacterIMCInfo> InputMappingContexts;
};
