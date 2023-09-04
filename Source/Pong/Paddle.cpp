#include "Paddle.h"

APaddle::APaddle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APaddle::BeginPlay()
{
	Super::BeginPlay();

}

void APaddle::MovePaddle(float moveAmount)
{
	DesiredMoveAmount = FMath::Clamp(moveAmount, -1, 1);
}

void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	float newX = currentLocation.X + (DesiredMoveAmount * DeltaTime * MoveSpeed);
	newX = FMath::Clamp(newX, MinPos, MaxPos);
	SetActorLocation(FVector(newX, currentLocation.Y, currentLocation.Z));
}
