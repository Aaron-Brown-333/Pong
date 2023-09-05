// Copyright Epic Games, Inc. All Rights Reserved.


#include "PongGameModeBase.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "AIPaddle.h"
#include "PlayerPaddle.h"
#include "PongPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PongBall.h"


APongGameModeBase::APongGameModeBase()
{
    GameStateClass = APongGameState::StaticClass();
}

void APongGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PongGameState = GetGameState<APongGameState>();
    GameBall = GetWorld()->SpawnActor<APongBall>(BallBP, BallLocation, FRotator::ZeroRotator);
    BallMesh = Cast<UStaticMeshComponent>(GameBall->GetRootComponent());
    BallMesh->OnComponentHit.AddDynamic(this, &APongGameModeBase::OnPaddleHit);

    PongGameState->ResetPlayerScores();
    
}

void APongGameModeBase::ResetBall() {
    BallMesh->SetWorldLocation(FVector(0, 0, 10));
    BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
}


void APongGameModeBase::OnPointScored(int scoringPlayer)
{
    // Initialize.
    ResetBall();
    PongGameState ? PongGameState->ResetPredictedExitPoint() : void();
    PongGameState ? PongGameState->IncrementPlayerScore(scoringPlayer) : void();

    // Check for a win condition.
    if (HasPlayerWon(scoringPlayer)) {
        ShowGameOver(scoringPlayer);
    }
    else {
        ShowPointScored(scoringPlayer);
    }
}

bool APongGameModeBase::HasPlayerWon(int player)
{
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

FCollisionQueryParams APongGameModeBase::GetCollisionParams() {
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GameBall); // Ignore the ball itself during the sweep

    // Find all actors of class APaddle
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaddle::StaticClass(), FoundActors);

    // Add them to the ignore list
    for (AActor* Actor : FoundActors)
    {
        CollisionParams.AddIgnoredActor(Actor);
    }
    return CollisionParams;
  
}

void APongGameModeBase::PredictBallPath()
{
    FVector Start, End;
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams = GetCollisionParams();

    Start = GameBall->GetActorLocation();
    FVector ForwardVector = GameBall->GetVelocity().GetSafeNormal();
    End = ((ForwardVector * 2000.f) + Start);

    int bounceCount = 0;
    while (bounceCount < MaxBouncePrediction)  // Loop until a goal box is hit or we reach MaxBouncePrediction times
    {
        End = ((ForwardVector * 2000.f) + Start);
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

        if (bHit)
        {
            DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Green, false, 1, 0, 1);
            if (HitResult.GetActor()->ActorHasTag("GoalBox"))
            {
                PongGameState->PredictedExitPoint = HitResult.Location;
                return;
            }
            // Otherwise, we hit a wall, so let's prepare for the next iteration
            ForwardVector = ForwardVector - 2 * (ForwardVector | HitResult.Normal) * HitResult.Normal;
            Start = HitResult.Location + ForwardVector * 10.0f;
        }
        else
        {
            // Nothing was hit, something went wrong, return zero vector
            PongGameState->ResetPredictedExitPoint();
            return;
        }
        bounceCount++;
    }
    PongGameState->ResetPredictedExitPoint();
}


void APongGameModeBase::OnPaddleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    if (!OtherActor->IsA(APaddle::StaticClass()))
    {
        return;
    }
    PredictBallPath();
}
