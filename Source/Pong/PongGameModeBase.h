#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongBall.h"
#include "PongGameState.h"

#include "PongGameModeBase.generated.h"

UCLASS()
class PONG_API APongGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    APongGameModeBase();

    UFUNCTION()
    void OnPaddleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void LaunchBall(int scoringPlayer);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    void OnPointScored(int scoringPlayer);
    
    // Blueprint Implementable Events
    UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
    void ShowPointScored(int scoringPlayer);

    UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
    void ShowGameOver(int scoringPlayer);

    // BeginPlay Override
    virtual void BeginPlay() override;

protected:
    // Editor Properties
    UPROPERTY(EditDefaultsOnly, Category = "Setup")
    TSubclassOf<class APongBall> BallBP;

    UPROPERTY(EditAnywhere, Category = "Rules")
    int WinningScore;

    UPROPERTY(EditAnywhere, Category = "Rules")
    int MaxBouncePrediction = 5;

    UPROPERTY(EditAnywhere, Category = "CurrentGame")
    FVector BallLocation = FVector(0, 0, 10);

    UPROPERTY(EditAnywhere, Category = "CurrentGame")
    FVector PredictedExitPoint = FVector(0, 0, 10);

private:
    // Private Variables
    APongBall* GameBall;
    UStaticMeshComponent* BallMesh;
    APongGameState* PongGameState;

    // Helper Functions
    void ResetBall();
    bool HasPlayerWon(int player);
    FCollisionQueryParams GetCollisionParams();
    void PredictBallPath();
};
