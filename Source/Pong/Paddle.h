// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Paddle.generated.h"

UCLASS()
class PONG_API APaddle : public APawn
{
	GENERATED_BODY()

	float DesiredMoveAmount = 0;

public:
	APaddle();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 100;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxPos = 140;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinPos = -140;

	void MovePaddle(float moveAmount);
	virtual void Tick(float DeltaTime) override;

};
