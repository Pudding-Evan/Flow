// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FlowLinkedAnimInstance.generated.h"

class UFlowAnimInstance;
class AFlowCharacterBase;

/**
 * 
 */
UCLASS()
class FLOW_API UFlowLinkedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TWeakObjectPtr<UFlowAnimInstance> Parent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)
	TObjectPtr<AFlowCharacterBase> Character;

public:
	UFlowLinkedAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

protected:
	// Be very careful when using this function to read your custom variables using the property access system. It is
	// safe to use this function to read variables that change only inside UAlsAnimationInstance::NativeUpdateAnimation()
	// because it is guaranteed that this function will be called before parallel animation evaluation. Reading variables
	// that change in other functions can be dangerous because they can be changed in the game thread at the same
	// time as being read in the worker thread, which can lead to undefined behavior or even a crash. If you're not
	// sure what you're doing, then it's better to access your custom variables through the "Parent" variable.
	UFUNCTION(BlueprintCallable, Category = "ALS|Als Linked Animation Instance", Meta = (BlueprintProtected, BlueprintThreadSafe))
	UFlowAnimInstance* GetParentUnsafe() const;
};


inline UFlowAnimInstance* UFlowLinkedAnimInstance::GetParentUnsafe() const
{
	return Parent.Get();
}


