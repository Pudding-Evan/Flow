// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Runtime/Animation/FlowAnimSetting.h>

#include "CoreMinimal.h"
#include "FlowCharacterState.h"
#include "FlowCharacterStateInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FlowCharacterSetting.h"
#include "FlowCharacterMovementComponent.generated.h"

class UDataTable;
struct FMovementSeting_State;
struct FTableRowBase;
struct FInputActionValue;
class AFlowCharacterBase;
class UFlowInputComponent;
class UFlowInputConfig;
class UFlowCharacterInputDataAsset;


UCLASS()
class FLOW_API UFlowCharacterInfoSettingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "IMC")
	FMovementSeting_State MovementSettingState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm/s"))
	float MovingSpeedThreshold{ 50.0f };
	
};

/**
 * 
 */
UCLASS()
class FLOW_API UFlowCharacterMovementComponent :
	public UCharacterMovementComponent,
	public IFlowCharacterStateInterface
{
	GENERATED_BODY()

public:
	UFlowCharacterMovementComponent();
public:
	virtual void BeginPlay() override;
	//virtual void OnRegister() override;
	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void BindInputAction(UFlowInputComponent* InputComponent, UFlowInputConfig* InputConfig);

	void Input_Move(const FInputActionValue& InputValue) ;

	void Input_LookMouse(const FInputActionValue& InputValue) ;

	void Input_Jump(const FInputActionValue& InputValue) ;

	void Input_Stance(const FInputActionValue& InputValue);

	void Input_Sprint_Start(const FInputActionValue& InputValue);

	void Input_Sprint_End(const FInputActionValue& InputValue);

protected:

	UPROPERTY()
	AFlowCharacterBase* Character;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UFlowCharacterInfoSettingDataAsset> CharacterSettings;

public:

#pragma region Gait
	FGaitSetting GaitSetting;

	UPROPERTY(BlueprintReadWrite, Category = "Move|Locomotion")
	FFlowLocomotionState LocomotionState;

	void UpdateLocomotionState(const float DeltaTime);

	UPROPERTY(BlueprintReadWrite, Category = "Move|Gait")
	EGait PreGait{EGait::Walking};

	UPROPERTY(BlueprintReadWrite, Category = "Move|Gait")
	EGait Gait{EGait::Walking};
	 
	virtual EGait GetGait() const override { return Gait; }

	virtual void SetGait(EGait DesiredGait) override;

	virtual void OnGaitChanged(EGait NewActualGait) override;

	float CalculateGaitAmount() const;

#pragma endregion

	UPROPERTY(BlueprintReadWrite, Category = "Move|Rotation")
	EFlowRotaionMode RotationMode;

	EFlowRotaionMode GetRotationMode() const;

	void UpdateMovementSetting();

	FGaitSetting GetTargetMovementSetting();

	FFlowLocomotionState GetLocomotionState() const { return LocomotionState; }

	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;

public:
	template<typename T>
	static bool IsStateDifferent(T& NewValue, T& Value);

};

template <typename T>
bool UFlowCharacterMovementComponent::IsStateDifferent(T& NewValue, T& Value)
{
	return NewValue != Value;
}


inline EFlowRotaionMode UFlowCharacterMovementComponent::GetRotationMode() const
{
	return RotationMode;
}



