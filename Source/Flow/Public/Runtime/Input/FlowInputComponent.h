// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "FlowInputConfig.h"
#include "GameplayTagContainer.h"
#include "FlowInputComponent.generated.h"

struct FGameplayTag;
class UFlowInputConfig;
/**
 * 
 */
UCLASS()
class FLOW_API UFlowInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass,typename FuncType>
	void BindNativeAction(UFlowInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

};

template <class UserClass, typename FuncType>
void UFlowInputComponent::BindNativeAction(UFlowInputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);

	if(const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

