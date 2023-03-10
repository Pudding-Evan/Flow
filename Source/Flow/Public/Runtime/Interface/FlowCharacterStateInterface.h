// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlowCharacterStateEnums.h"
#include "FlowCharacterStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable, BlueprintType)
class UFlowCharacterStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FLOW_API IFlowCharacterStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual EGait GetGait() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void SetGait(EGait DesiredGait);

	virtual EFlowRotaionMode GetRotationMode() const = 0;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "State")
	void SetRotationMode(EFlowRotaionMode DesiredRotationMode);

	virtual EStance GetStance() const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "State")
	void SetStance(EStance DesiredStance);

};
