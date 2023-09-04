// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PongPlayerController.h"

void APlayerPaddle::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	float moveAmt = Value.Get<float>();
	MovePaddle(moveAmt);
}

void APlayerPaddle::MoveEnd(const FInputActionValue& Value)
{
	MovePaddle(0);
}

void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPaddle::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerPaddle::MoveEnd);
	}

}

void APlayerPaddle::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APongPlayerController* PlayerController = Cast<APongPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
