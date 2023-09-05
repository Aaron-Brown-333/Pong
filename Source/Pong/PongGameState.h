#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameState.generated.h"

UCLASS()
class PONG_API APongGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    // Your FVector variable.
    UPROPERTY(BlueprintReadOnly, Category = "Pong")
    FVector PredictedExitPoint = FVector(0,0,0);

    UPROPERTY(BlueprintReadOnly, Category = "Pong")
    int Player1Score = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Pong")
    int Player2Score = 0;



    void IncrementPlayerScore(int player);

    void ResetPlayerScores();

    void ResetPredictedExitPoint();

};
