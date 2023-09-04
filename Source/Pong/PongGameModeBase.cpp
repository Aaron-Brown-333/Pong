// Copyright Epic Games, Inc. All Rights Reserved.


#include "PongGameModeBase.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "AIPaddle.h"
#include "PlayerPaddle.h"
#include "PongPlayerController.h"
#include "PongBall.h"

void APongGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PongGameState = GetGameState<APongGameState>();
    GameBall = GetWorld()->SpawnActor<APongBall>(BallBP, BallLocation, FRotator::ZeroRotator);
    BallMesh = Cast<UStaticMeshComponent>(GameBall->GetRootComponent());
    BallMesh->OnComponentHit.AddDynamic(this, &APongGameModeBase::OnPaddleHit);

    StartGame();
}

APongGameModeBase::APongGameModeBase()
{
    GameStateClass = APongGameState::StaticClass();
}

void APongGameModeBase::StartGame()
{
    if (PongGameState) {
        PongGameState->Player1Score = 0;
        PongGameState->Player2Score = 0;
    }
}

void APongGameModeBase::ResetBall() {
    BallMesh->SetWorldLocation(FVector(0, 0, 10));
    BallMesh->SetPhysicsLinearVelocity(FVector(0,0,0));
}


void APongGameModeBase::OnPointScored(int scoringPlayer)
{
    // Initialize.
    ResetBall();
    ResetPredictedExitPoint();

    // Update player score.
    UpdatePlayerScore(scoringPlayer);

    // Check for a win condition.
    int WinningPlayer = HasPlayerWon(1) ? 1 : HasPlayerWon(2) ? 2 : 0;
    if (WinningPlayer) {
        ShowGameOver(WinningPlayer);
    }
    else {
        ShowPointScored(scoringPlayer);
    }
}

void APongGameModeBase::ResetPredictedExitPoint()
{
    if (!PongGameState) {
        return;
    }

    PongGameState->PredictedExitPoint = FVector(0, 0, 0);
}

void APongGameModeBase::UpdatePlayerScore(int scoringPlayer)
{
    if (!PongGameState) {
        return;
    }

    if (scoringPlayer == 1) {
        PongGameState->Player1Score = PongGameState->Player1Score + 1;
    }
    else {
        PongGameState->Player2Score = PongGameState->Player2Score + 1;
    }
}

bool APongGameModeBase::HasPlayerWon(int player)
{
    if (!PongGameState) {
        return false;
    }

    int player1Score = PongGameState->Player1Score;
    int player2Score = PongGameState->Player2Score;

    if (player == 1) {
        return (player1Score >= WinningScore && player1Score >= (player2Score + 2));
    }
    else {
        return (player2Score >= WinningScore && player2Score >= (player1Score + 2));
    }
}

void APongGameModeBase::LaunchBall(int scoringPlayer)
{
    // Set the initial velocity
    FVector InitialDirection = (scoringPlayer == 2) ? FVector(0, -1, 0) : FVector(0, 1, 0);
    float InitialSpeed = 300.0f;
    BallMesh->SetWorldLocation(FVector(0, 0, 10));
    BallMesh->SetPhysicsLinearVelocity(InitialDirection * InitialSpeed);
}

void APongGameModeBase::OnPaddleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (!OtherActor->IsA(APaddle::StaticClass()))
    {
        return;
    }
    FVector currentPos = GameBall->GetActorLocation();
    FVector currentVelocity = GameBall->GetVelocity();
    FVector futurePos;
    float minX = -150.0f;
    float maxX = 150.0f;
    float maxY = 240.0f;
    float minY = -240.0f;

    PredictedExitPoint = FVector(0,0,0);
    FString ExitWall;

    int maxBounces = 4;
    int currentBounce = 0;
    bool isExit = false;
    while (currentBounce < maxBounces) {

        // Time to reach each wall
        float timeToReachMinX = (minX - currentPos.X) / currentVelocity.X;
        float timeToReachMaxX = (maxX - currentPos.X) / currentVelocity.X;
        float timeToReachMinY = (minY - currentPos.Y) / currentVelocity.Y;
        float timeToReachMaxY = (maxY - currentPos.Y) / currentVelocity.Y;

        // Initialize the minimum time to a high value
        float minTime = FLT_MAX;
        // Check each wall and see which one is hit first
        if (currentVelocity.X < 0 && timeToReachMinX > 0 && timeToReachMinX < minTime) {
            minTime = timeToReachMinX;
        }
        if (currentVelocity.X > 0 && timeToReachMaxX > 0 && timeToReachMaxX < minTime) {
            minTime = timeToReachMaxX;
        }
        if (currentVelocity.Y < 0 && timeToReachMinY > 0 && timeToReachMinY < minTime) {
            minTime = timeToReachMinY;
            isExit = true;
        }
        if (currentVelocity.Y > 0 && timeToReachMaxY > 0 && timeToReachMaxY < minTime) {
            minTime = timeToReachMaxY;
            isExit = true;
        }

        // Predict future position based on the time to hit the wall
        futurePos = currentPos + currentVelocity * minTime;

        //Draw debug line from current position to future position
        DrawDebugLine(
            GetWorld(),
            currentPos,
            futurePos,
            FColor::Green,
            false,
            1,
            0,
            5
        );

        if (isExit) {
            PredictedExitPoint = futurePos;

            if (PongGameState)
            {
                PongGameState->PredictedExitPoint = futurePos;
            }
            break;
        }

        currentPos = futurePos;
        currentVelocity.X = -currentVelocity.X;
        currentBounce++;
    }
}

