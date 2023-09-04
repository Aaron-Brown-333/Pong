#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallHit, int, IsPaddle);


UCLASS()
class PONG_API APongBall : public AActor
{

	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BallMesh;

public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float InitialMoveSpeed = 300;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxMoveSpeed = 600;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnBallHit OnBallHit;

	APongBall();

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnPaddleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
