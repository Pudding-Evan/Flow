// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlowCharacterState.h"
#include "GameFramework/Character.h"
#include "FlowCharacterBase.generated.h"

class UFlowInputConfig;
class UInputAction;
class UFlowCharacterInputDataAsset;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;

struct FInputActionValue;
struct FFlowLocomotionState;

UCLASS()
class FLOW_API AFlowCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlowCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCameraComponent* CameraComp;

#pragma region Input

	UPROPERTY(EditDefaultsOnly,Category = "Input|Config")
	UFlowCharacterInputDataAsset* DAInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Config")
	TObjectPtr<UFlowInputConfig> InputConfig;

#pragma endregion



};
