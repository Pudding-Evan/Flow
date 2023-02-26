// Fill out your copyright notice in the Description page of Project Settings.


#include "Runtime/Character/FlowCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Flow/FlowGameModeBase.h"
#include "EnhancedInputComponent.h"
#include "Runtime/FlowGameplayTags.h"
#include "Runtime/Character/FlowCharacterMovementComponent.h"
#include "Runtime/Input/FlowCharacterInputDataAsset.h"
#include "Runtime/Input/FlowInputComponent.h"

AFlowCharacterBase::AFlowCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UFlowCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SprintArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(SpringArmComp);

	//UFlowCharacterMovementComponent* FlowMoveComp = CastChecked<UFlowCharacterMovementComponent>(GetCharacterMovement());
}

// Called when the game starts or when spawned
void AFlowCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlowCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AFlowCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(APlayerController* PC = CastChecked<APlayerController>(GetController()))
	{

		// Mapping
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(DAInputMapping->InputMappingContexts.Num() > 0)
			{
				for(auto CharacterIMCInfo : DAInputMapping->InputMappingContexts)
				{
					Subsystem->AddMappingContext(CharacterIMCInfo.InputMappingContext, CharacterIMCInfo.Priority);
				}
			}
		}

		// Action

		// Bind Move Action -
		if(UFlowCharacterMovementComponent* FlowMoveComp = CastChecked<UFlowCharacterMovementComponent>(GetCharacterMovement()))
		{
			if(UFlowInputComponent* FlowInputComp = CastChecked<UFlowInputComponent>(InputComponent))
			{
				FlowMoveComp->BindInputAction(FlowInputComp, InputConfig);
			}
		}
	}
}


