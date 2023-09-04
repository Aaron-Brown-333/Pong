// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "AIPongController.generated.h"

/**
 * 
 */
UCLASS()
class PONG_API AAIPongController : public AAIController
{
	GENERATED_BODY()
	
public:
	FVector DesiredLocation = FVector(0, 0, 0);

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
