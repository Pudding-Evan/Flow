#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FFlowGameplayTags
{
public:

	static const FFlowGameplayTags& Get() { return GameplayTags; }

public:

	FGameplayTag InputTag_Move = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Move")));

	FGameplayTag InputTag_Look_Mouse = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.LookMouse")));

	FGameplayTag InputTag_Jump = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Jump")));

	FGameplayTag InputTag_Stance = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Stance")));

	FGameplayTag InputTag_Sprint = FGameplayTag::RequestGameplayTag(FName(TEXT("Input.Sprint")));
private:

	static FFlowGameplayTags GameplayTags;
};