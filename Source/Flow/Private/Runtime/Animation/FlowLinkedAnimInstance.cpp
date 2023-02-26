// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Animation/FlowLinkedAnimInstance.h"
#include "Runtime/Animation/FlowAnimInstance.h"
#include "Runtime/Character/FlowCharacterBase.h"


UFlowLinkedAnimInstance::UFlowLinkedAnimInstance()
{

	//TODO What that means ?

	//RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	//bUseMainInstanceMontageEvaluationData = true;
}

void UFlowLinkedAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Parent = Cast<UFlowAnimInstance>(GetSkelMeshComponent()->GetAnimInstance());

	Character = Cast<AFlowCharacterBase>(GetOwningActor());

//#if WITH_EDITOR
//	if (!GetWorld()->IsGameWorld())
//	{
//		// Use default objects for editor preview.
//
//		if (!Parent.IsValid())
//		{
//			Parent = GetMutableDefault<UFlowAnimInstance>();
//		}
//
//		if (!IsValid(Character))
//		{
//			Character = GetMutableDefault<AFlowCharacterBase>();
//		}
//	}
//#endif
}

void UFlowLinkedAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

