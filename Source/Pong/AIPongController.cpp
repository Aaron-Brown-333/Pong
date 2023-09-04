// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPongController.h"
#include "EngineUtils.h"
#include "Paddle.h"
#include "PongBall.h"
#include "PongGameState.h"
#include "Engine/World.h"

#include "PongGameModeBase.h"

void AAIPongController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APaddle* ControlledPaddle = Cast<APaddle>(GetPawn());
    FVector PawnLoc = ControlledPaddle->GetActorLocation();

    APongBall* Ball = nullptr;
    for (TActorIterator<APongBall> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        Ball = *ActorItr;
    }

    if (!Ball) {
        DesiredLocation.X = 0;
    }

    APongGameState* GameState = GetWorld()->GetGameState<APongGameState>();

    if (GameState)
    {
        DesiredLocation = GameState->PredictedExitPoint;
    }

    if (Ball && DesiredLocation.Y == 0) {
        DesiredLocation = Ball->GetActorLocation();
    }

    //if (Ball) {
    //    float direction = Ball->GetVelocity().X;
    //    float relativePosition = Ball->GetActorLocation().X - PawnLoc.X;
    //    UE_LOG(LogTemp, Error, TEXT(""), direction, relativePosition);
    //    if (FMath::Sign(direction) != FMath::Sign(relativePosition)) {
    //        DesiredLocation = FVector(0, 0, 0);
    //    }

    //}

 
    float dist = FMath::Abs(DesiredLocation.X - PawnLoc.X);
    if (dist < 5) {
        ControlledPaddle->MovePaddle(0);
        return;
    }
    float DesiredDirection = DesiredLocation.X > PawnLoc.X ? 1 : -1;
    ControlledPaddle->MovePaddle(DesiredDirection);

}

void AAIPongController::BeginPlay()
{
    Super::BeginPlay();
}
