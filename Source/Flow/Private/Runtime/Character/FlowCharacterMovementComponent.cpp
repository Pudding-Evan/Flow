// Fill out your copyright notice in the Description page of Project Settings.


// ReSharper disable CppMemberFunctionMayBeConst
#include "Runtime/Character/FlowCharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Curves/CurveVector.h"
#include "GameFramework/Character.h"
#include "Runtime/FlowGameplayTags.h"
#include "Runtime/Character/FlowCharacterBase.h"
#include "Runtime/Input/FlowInputComponent.h"
#include "Runtime/Utility/FlowMathLibrary.h"

UFlowCharacterMovementComponent::UFlowCharacterMovementComponent()
{
	MaxAcceleration = 1500.0f;
	BrakingFrictionFactor = 0.5f;
}

void UFlowCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	Gait = EGait::Walking;
	UpdateMovementSetting();
	APawn* Pawn = GetPawnOwner();
	Character = CastChecked<AFlowCharacterBase>(Pawn);
}

void UFlowCharacterMovementComponent::OnRegister()
{
	Super::OnRegister();

	Gait = EGait::Walking;

	const auto& ActorTransform{ GetActorTransform() };
	LocomotionState.Location = ActorTransform.GetLocation();
	LocomotionState.RotationQuaternion = ActorTransform.GetRotation();
	LocomotionState.Rotation = LocomotionState.RotationQuaternion.Rotator();
	LocomotionState.TargetYawAngle = LocomotionState.Rotation.Yaw;

	//UE_LOG(LogTemp, Log, TEXT("OnRegisterMovementComponent "));
}

void UFlowCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLocomotionState(DeltaTime);

	UpdateGroundedRotaion(DeltaTime);

}

void UFlowCharacterMovementComponent::BindInputAction(UFlowInputComponent* InputComponent, UFlowInputConfig* InputConfig)
{
	if (UFlowInputComponent* FlowInputComp = CastChecked<UFlowInputComponent>(InputComponent))
	{
		const FFlowGameplayTags& GameplayTags = FFlowGameplayTags::Get();

		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_Jump);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Stance, ETriggerEvent::Started, this, &ThisClass::Input_Stance);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Started, this, &ThisClass::Input_Sprint_Start);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Sprint, ETriggerEvent::Completed, this, &ThisClass::Input_Sprint_End);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Started, this, &ThisClass::Input_Crouch);
		FlowInputComp->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Completed, this, &ThisClass::Input_UnCrouch);

	}
}

void UFlowCharacterMovementComponent::Input_Move(const FInputActionValue& InputValue)
{
	APawn* Pawn = GetPawnOwner();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (!Controller) { return; }

	const FVector2D Value = InputValue.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector Direction = YawRotation.RotateVector(FVector::RightVector);
		//const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		Character->AddMovementInput(Direction, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector Direction = YawRotation.RotateVector(FVector::ForwardVector);
		//const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		Character->AddMovementInput(Direction, Value.Y);
	}
}

void UFlowCharacterMovementComponent::Input_LookMouse(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Character->AddControllerYawInput(Value.X);
	}

	if (Value.X != 0.0f)
	{
		Character->AddControllerPitchInput(-1 * Value.Y);
	}
}

void UFlowCharacterMovementComponent::Input_Jump(const FInputActionValue& InputValue)
{
	Character->Jump();
}

void UFlowCharacterMovementComponent::Input_Stance(const FInputActionValue& InputValue)
{
	switch (Gait)
	{
	case EGait::Walking:
		{
			IFlowCharacterStateInterface::Execute_SetGait(this, EGait::Running);
			break;
		}
	case EGait::Running:
		{
			IFlowCharacterStateInterface::Execute_SetGait(this, EGait::Walking);
			break;	
		}
	default:
		break;
	}
}

void UFlowCharacterMovementComponent::Input_Sprint_Start(const FInputActionValue& InputValue)
{
	IFlowCharacterStateInterface::Execute_SetGait(this, EGait::Sprinting);
}

void UFlowCharacterMovementComponent::Input_Sprint_End(const FInputActionValue& InputValue)
{
	IFlowCharacterStateInterface::Execute_SetGait(this, EGait::Running);
}

void UFlowCharacterMovementComponent::Input_Crouch(const FInputActionValue& InputValue)
{
	IFlowCharacterStateInterface::Execute_SetStance(this,EStance::Crouching);
	Character->Crouch();
}

void UFlowCharacterMovementComponent::Input_UnCrouch(const FInputActionValue& InputValue)
{
	IFlowCharacterStateInterface::Execute_SetStance(this,EStance::Standing);
	Character->UnCrouch();
}

void UFlowCharacterMovementComponent::UpdateGroundedRotaion(float DeltaTime)
{
	// Moving -
	if(LocomotionState.bMoving && !Character->HasAnyRootMotion())
	{
		// Rotation Mode
		static constexpr auto TargetVelocityInterpSpeed{ 800.0f };
		static constexpr auto TargetLookInterpSpeed{ 500.0f };
		static constexpr auto TargetAimingInterpSpeed{ 1000.f };
		static constexpr auto NormalActorInterpSpeed{ 12.0f };
		static constexpr auto FastActorInterpSpeed{ 20.0f };

		if(RotationMode == EFlowRotaionMode::VelocityDirection)
		{
			SmoothCharacterRotation(LocomotionState.VelocityYawAngle, DeltaTime, TargetVelocityInterpSpeed, NormalActorInterpSpeed);
			return;
		}

		if(RotationMode == EFlowRotaionMode::LookDirection)
		{
			if(Gait == EGait::Walking || Gait == EGait::Running)
			{
				SmoothCharacterRotation(LocomotionState.ViewYawAngle, DeltaTime, TargetLookInterpSpeed, NormalActorInterpSpeed);
			}
			else
			{
				SmoothCharacterRotation(LocomotionState.VelocityYawAngle, DeltaTime, TargetLookInterpSpeed, NormalActorInterpSpeed);
			}
			return;
		}

		if (RotationMode == EFlowRotaionMode::Aiming)
		{
			SmoothCharacterRotation(LocomotionState.VelocityYawAngle, DeltaTime, TargetAimingInterpSpeed, FastActorInterpSpeed);
			return;
		}

	}
	else
	{
		//TODO NotMoving Animation Rotate.

		
	}

}

void UFlowCharacterMovementComponent::SmoothCharacterRotation(const float TargetYawAngle, const float DeltaTime,
	float TargetInterpSpeed, float ActorInterpSpeed)
{
	LocomotionState.TargetYawAngle = TargetYawAngle;
	LocomotionState.SmoothTargetYawAngle = UFlowMathLibrary::InterpolateAngleConstant(LocomotionState.SmoothTargetYawAngle, TargetYawAngle, DeltaTime, TargetInterpSpeed);

	FRotator NewRotation{ LocomotionState.Rotation };

	NewRotation.Yaw = UFlowMathLibrary::ExponentialDecayAngle(FRotator::NormalizeAxis(NewRotation.Yaw), LocomotionState.SmoothTargetYawAngle, DeltaTime, ActorInterpSpeed);

	Character->SetActorRotation(NewRotation);


}


void UFlowCharacterMovementComponent::UpdateLocomotionState(const float DeltaTime)
{
	ensure(Character);

	LocomotionState.PreviousVelocity = LocomotionState.Velocity;
	LocomotionState.PreviousViewYawAngle = LocomotionState.ViewRotation.Yaw;

	LocomotionState.ViewRotation = Character->GetControlRotation();
	LocomotionState.ViewYawAngle = LocomotionState.ViewRotation.Yaw;

	//UE_LOG(LogTemp, Log, TEXT("View Yaw Angle : [%f]"), LocomotionState.ViewYawAngle);

	const auto& ActorTransform{ GetActorTransform() };

	LocomotionState.Location = ActorTransform.GetLocation();
	LocomotionState.RotationQuaternion = ActorTransform.GetRotation();
	LocomotionState.Rotation = LocomotionState.RotationQuaternion.Rotator();

	LocomotionState.Velocity = Character->GetVelocity();
	LocomotionState.Speed = UE_REAL_TO_FLOAT(LocomotionState.Velocity.Size2D());
	LocomotionState.bHasSpeed = LocomotionState.Speed >= 1.0f;

	if(LocomotionState.bHasSpeed)
	{
		LocomotionState.VelocityYawAngle = UE_REAL_TO_FLOAT(FMath::RadiansToDegrees(FMath::Atan2(LocomotionState.Velocity.Y , LocomotionState.Velocity.X))); // Get Angle Form X Axis;
	}

	//UE_LOG(LogTemp, Log, TEXT("Velocity Yaw Angle : [%f]"), LocomotionState.VelocityYawAngle)

	LocomotionState.Acceleration = (LocomotionState.Velocity - LocomotionState.PreviousVelocity) / DeltaTime;
	LocomotionState.MovementInputDirection = (GetCurrentAcceleration() / GetMaxAcceleration()).GetSafeNormal();;
	LocomotionState.bHasMovementInput = LocomotionState.MovementInputDirection.SizeSquared() > KINDA_SMALL_NUMBER; // 这里不做平方，所以更快。

	// 有速度且当前有加速度，或者速度大于临界值 ——> bMoving
	LocomotionState.bMoving = (LocomotionState.bHasMovementInput&& LocomotionState.Speed) || 
		(LocomotionState.Speed >  CharacterSettings->MovingSpeedThreshold);

}

void UFlowCharacterMovementComponent::SetGait_Implementation(EGait DesiredGait)
{
	if (IsStateDifferent(DesiredGait, Gait))
	{
		PreGait = Gait;
		Gait = DesiredGait;
		UpdateMovementSetting();

		OnGaitChanged(DesiredGait);
	}
}

void UFlowCharacterMovementComponent::OnGaitChanged_Implementation(EGait NewActualGait)
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance->Implements<UFlowCharacterStateInterface>())
	{
		IFlowCharacterStateInterface::Execute_SetGait(AnimInstance,NewActualGait);
	}
}

float UFlowCharacterMovementComponent::CalculateGaitAmount() const
{
	const auto Speed{ UE_REAL_TO_FLOAT(Velocity.Size2D()) };
	if(Speed <= GaitSetting.WalkSpeed)
	{
		static const FVector2f GaitAmount{ 0.0f, 1.0f };
		return FMath::GetMappedRangeValueClamped({ 0.0f, GaitSetting.WalkSpeed }, GaitAmount, Speed);
	}

	if (Speed <= GaitSetting.RunSpeed)
	{
		static const FVector2f GaitAmount{ 1.0f, 2.0f };
		return FMath::GetMappedRangeValueClamped({ GaitSetting.WalkSpeed, GaitSetting.RunSpeed }, GaitAmount, Speed);
	}

	static const FVector2f GaitAmount{ 2.0f, 3.0f };
	return FMath::GetMappedRangeValueClamped({ GaitSetting.RunSpeed, GaitSetting.SprintSpeed }, GaitAmount, Speed);
}

void UFlowCharacterMovementComponent::SetRotationMode_Implementation(EFlowRotaionMode DesiredRotationMode)
{
	if (IsStateDifferent(DesiredRotationMode, RotationMode))
	{
		PreRotationMode = RotationMode;
		RotationMode = DesiredRotationMode;

		OnRotationModeChanged(DesiredRotationMode);
	}
}

void UFlowCharacterMovementComponent::OnRotationModeChanged_Implementation(EFlowRotaionMode NewRotationMode)
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance->Implements<UFlowCharacterStateInterface>())
	{
		IFlowCharacterStateInterface::Execute_SetRotationMode(AnimInstance,NewRotationMode);
	}
}

void UFlowCharacterMovementComponent::SetStance_Implementation(EStance DesiredStance)
{
	if (IsStateDifferent(DesiredStance, Stance))
	{
		PreStance = Stance;
		Stance = DesiredStance;

		OnStanceChanged(DesiredStance);
	}
}

void UFlowCharacterMovementComponent::OnStanceChanged_Implementation(EStance NewStance)
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance->Implements<UFlowCharacterStateInterface>())
	{
		IFlowCharacterStateInterface::Execute_SetStance(AnimInstance, NewStance);
	}
}

void UFlowCharacterMovementComponent::UpdateMovementSetting()
{
	GaitSetting = GetTargetMovementSetting();
	float TargetMaxWalkSpeed = GaitSetting.WalkSpeed;
	switch(Gait)
	{
	case EGait::Walking:
		TargetMaxWalkSpeed = GaitSetting.WalkSpeed;
		break;
	case EGait::Running:
		TargetMaxWalkSpeed = GaitSetting.RunSpeed;
		break;
	case EGait::Sprinting:
		TargetMaxWalkSpeed = GaitSetting.SprintSpeed;
		break;
	}

	MaxWalkSpeed = TargetMaxWalkSpeed;
	MaxWalkSpeedCrouched = TargetMaxWalkSpeed;
}

FGaitSetting UFlowCharacterMovementComponent::GetTargetMovementSetting()
{
	ensure(CharacterSettings);
	// TODO
	return CharacterSettings->MovementSettingState.Normal.Stance;
}

float UFlowCharacterMovementComponent::GetMaxAcceleration() const
{
	return IsValid(GaitSetting.AccelerationAndDecelerationAndGroundFrictionCurve)
		? GaitSetting.AccelerationAndDecelerationAndGroundFrictionCurve->FloatCurves[0].Eval(CalculateGaitAmount())
		: Super::GetMaxAcceleration();
}

float UFlowCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	return IsValid(GaitSetting.AccelerationAndDecelerationAndGroundFrictionCurve)
		? GaitSetting.AccelerationAndDecelerationAndGroundFrictionCurve->FloatCurves[1].Eval(CalculateGaitAmount())
		: Super::GetMaxBrakingDeceleration();
}
