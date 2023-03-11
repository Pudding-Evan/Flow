// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Animation/FlowAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Character/FlowCharacterBase.h"
#include "Runtime/Character/FlowCharacterMovementComponent.h"

void UFlowAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	UE_LOG(LogTemp, Log, TEXT("NativeInitializeAnimation "));

	Character = Cast<AFlowCharacterBase>(GetOwningActor());
	if(Character)
	{
		CharMoveComp = Cast<UFlowCharacterMovementComponent>(Character->GetMovementComponent());
	}

#if WITH_EDITOR
	if (!GetWorld()->IsGameWorld() && !IsValid(Character))
	{
		// Use default objects for editor preview.

		Character = GetMutableDefault<AFlowCharacterBase>();
		CharMoveComp = GetMutableDefault<UFlowCharacterMovementComponent>();
	}
#endif
	
}

void UFlowAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// Ensure的代码写法 -> 用来可以输出Log的部分。

	if(IsValid(Character) || IsValid(CharMoveComp))
	{
		return;
	}

	// TODO TeleportType

}

void UFlowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//DECLARE_SCOPE_CYCLE_COUNTER(TEXT("UAlsAnimationInstance::NativeUpdateAnimation()"),
	//STAT_UAlsAnimationInstance_NativeUpdateAnimation, STATGROUP_Als)
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character) || !IsValid(CharMoveComp))
	{
		return;
	}

	RefreshLocomotionOnGameThread();

}

void UFlowAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	RefreshMovementState(DeltaSeconds);


}

UFlowCharacterMovementComponent* UFlowAnimInstance::GetCharacterMovementComponent() const
{
	APawn* Pawn = TryGetPawnOwner();

	if(Pawn &&  Pawn->GetMovementComponent())
	{
		return Cast<UFlowCharacterMovementComponent>(Pawn->GetMovementComponent());
	}

	return NULL;
}

void UFlowAnimInstance::RefreshLocomotionOnGameThread()
{

	const FFlowLocomotionState& CharLocomotionState = CharMoveComp->GetLocomotionState();
	LocomotionState.Speed = CharLocomotionState.Speed;
	LocomotionState.Velocity = CharLocomotionState.Velocity;
	LocomotionState.bMoving = CharLocomotionState.bMoving;
	LocomotionState.Acceleration = CharLocomotionState.Acceleration;

	LocomotionState.VelocityYawAngle = CharLocomotionState.VelocityYawAngle;
	LocomotionState.ViewYawAngle = CharLocomotionState.ViewYawAngle;

	LocomotionState.Location = CharLocomotionState.Location;
	LocomotionState.Rotation = CharLocomotionState.Rotation;
	LocomotionState.RotationQuaternion = CharLocomotionState.RotationQuaternion;

	LocomotionState.MaxAcceleration = CharMoveComp->GetMaxAcceleration();
	LocomotionState.MaxBrakingDeceleration = CharMoveComp->GetMaxBrakingDeceleration();

}

void UFlowAnimInstance::RefreshMovementState(float DeltaSeconds)
{

	GroundedState.WalkRunBlendAmount = CalculateWalkOrRunBlend();
	GroundedState.StrideBlendAmount = CalculateStrideBlend();
	GroundedState.StandingPlayRate = CalculatePlayRateBlend();

	RefreshSprint(DeltaSeconds);
	RefreshMovementDirection();
	RefreshVelocityBlendState();
}

void UFlowAnimInstance::RefreshSprint(float DeltaSeconds)
{
	if (!LocomotionState.bMoving) return;

	FVector ReletiveAccleration = LocomotionState.RotationQuaternion.UnrotateVector(LocomotionState.Acceleration);
	FVector RelativeAccelerationAmount;
	if ((LocomotionState.Acceleration | LocomotionState.Velocity) >= 0.0f) // Dot -> 是否同向
	{
		RelativeAccelerationAmount = UKismetMathLibrary::Vector_ClampSizeMax(ReletiveAccleration, LocomotionState.MaxAcceleration)
			/ LocomotionState.MaxAcceleration;
	}
	else
	{
		RelativeAccelerationAmount = UKismetMathLibrary::Vector_ClampSizeMax(ReletiveAccleration, LocomotionState.MaxBrakingDeceleration)
			/ LocomotionState.MaxBrakingDeceleration;
	}

	if (Gait != EGait::Sprinting)
	{
		GroundedState.SprintTime = 0;
		GroundedState.SprintAccelerationAmount = 0;
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ReletiveAccelerationAmount : X = [%f]"), RelativeAccelerationAmount.X);

	static constexpr auto TimeThreshold{ 0.5f };
	GroundedState.SprintTime = GroundedState.SprintTime + DeltaSeconds;
	GroundedState.SprintAccelerationAmount = RelativeAccelerationAmount.X;
	
}

void UFlowAnimInstance::RefreshMovementDirection()
{
	if(Gait == EGait::Sprinting)
	{
		GroundedState.MovementDirection = EFlowMovementDirection::Forward;
	}

	static constexpr auto ForwardHalfAngle{ 70.0f };

	EFlowMovementDirection Direction = CalculateMovementDirection(
		FRotator::NormalizeAxis(LocomotionState.VelocityYawAngle - LocomotionState.ViewYawAngle), // (-180,180]
		ForwardHalfAngle,
		5.0f);

	GroundedState.MovementDirection = Direction;


}

float UFlowAnimInstance::CalculateWalkOrRunBlend() const
{
	if(Gait == EGait::Walking)
	{
		return 0.0f;
	}
	else
	{
		return 1.0f;
	}
}

float UFlowAnimInstance::CalculateStrideBlend() const
{
	float StrideAlpha = 0.0f;
	float Speed = LocomotionState.Speed;

	float WalkAlpha = AnimInstanceCurves.StrideBlend_N_Walk->GetFloatValue(Speed);
	float RunAlpha = AnimInstanceCurves.StrideBlend_N_Run->GetFloatValue(Speed);
	float CrouchAlpha = AnimInstanceCurves.StrideBlend_N_Crouch->GetFloatValue(Speed);
	float GaitCurveValue = GetCurveValue(FName("Weight_Gait")); // 硬编码 TODO : ALS C++ 版本怎么做的

	float Vlerp = FMath::Clamp(0.0f, 1.0f, GaitCurveValue - 1.0f);
	StrideAlpha = FMath::Lerp(WalkAlpha, RunAlpha, Vlerp);

	// TODO : Crouch Blend

	return StrideAlpha;
}

float UFlowAnimInstance::CalculatePlayRateBlend() const
{
	float GaitCurveValue = GetCurveValue(FName("Weight_Gait")); // 硬编码 TODO : ALS C++ 版本怎么做的
	float Vlerp = FMath::Clamp(GaitCurveValue - 1.0f,0.0f, 1.0f);
	float Speed = LocomotionState.Speed;
	float PlayRate =  FMath::Lerp(Speed / GroundedSetting.AnimatedWalkSpeed, Speed / GroundedSetting.AnimatedRunSpeed, Vlerp);

	Vlerp = FMath::Clamp( GaitCurveValue - 2.0f,0.0f, 1.0f);
	PlayRate = FMath::Lerp(PlayRate, Speed / GroundedSetting.AnimatedSprintSpeed, Vlerp);

	return PlayRate;
}

void UFlowAnimInstance::RefreshVelocityBlendState()
{
	const FVector RelativeVelocityDirection{
		FVector{LocomotionState.RotationQuaternion.UnrotateVector(LocomotionState.Velocity)}.GetSafeNormal()
	};

	const FVector RelativeDirection{
		RelativeVelocityDirection /
		(FMath::Abs(RelativeVelocityDirection.X) + FMath::Abs(RelativeVelocityDirection.Y) + FMath::Abs(RelativeVelocityDirection.Z))
	};

	// 分量
	GroundedState.VelocityBlendState.ForwardAmount = FMath::Clamp(RelativeDirection.X, 0.0f, 1.0f);
	GroundedState.VelocityBlendState.BackwardAmount = FMath::Abs(FMath::Clamp(RelativeDirection.X, -1.0f, 0.0f));
	GroundedState.VelocityBlendState.LeftAmount = FMath::Abs(FMath::Clamp(RelativeDirection.Y, -1.0f, 0.0f));
	GroundedState.VelocityBlendState.RightAmount = FMath::Clamp(RelativeDirection.Y, 0.0f, 1.0f);

	//TODO InterpTo
}

EFlowMovementDirection UFlowAnimInstance::CalculateMovementDirection(const float Angle, const float ForwardHalfAngle,
                                                                     const float AngleThreshold)
{
	/*
	 *  [-75,75]	forward
	 *	[65,110]	Right
	 *	[-65,-125]	Left
	 *				BackWard
	 */			

	if(Angle >= -ForwardHalfAngle - AngleThreshold && Angle <= ForwardHalfAngle + AngleThreshold)
	{
		return EFlowMovementDirection::Forward;
	}

	if (Angle >= ForwardHalfAngle - AngleThreshold && Angle <= 180.0f - ForwardHalfAngle + AngleThreshold)
	{
		return EFlowMovementDirection::Right;
	}

	if (Angle <= -(ForwardHalfAngle - AngleThreshold) && Angle >= -(180.0f - ForwardHalfAngle + AngleThreshold))
	{
		return EFlowMovementDirection::Left;
	}

	return EFlowMovementDirection::Backward;

}

void UFlowAnimInstance::SetGait_Implementation(EGait DesiredGait)
{
	Gait = DesiredGait;
}

void UFlowAnimInstance::SetRotationMode_Implementation(EFlowRotaionMode DesiredRotationMode)
{
	RotationMode = DesiredRotationMode;
}

void UFlowAnimInstance::SetStance_Implementation(EStance DesiredStance)
{
	Stance = DesiredStance;
	LocomotionState.bIsCourch = Stance == EStance::Crouching ? true : false;
}
