// Fill out your copyright notice in the Description page of Project Settings.


#include "PongBall.h"
#include "Paddle.h"

// Sets default values
APongBall::APongBall()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create the mesh component
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);

    // Setup mesh properties here
    BallMesh->SetSimulatePhysics(true);
    BallMesh->BodyInstance.LinearDamping = 0.0f;
    BallMesh->BodyInstance.AngularDamping = 0.0f;

    BallMesh->SetGenerateOverlapEvents(true);
}

void APongBall::OnPaddleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    // Only proceed if the OtherActor is a paddle
    if (!OtherActor->IsA(APaddle::StaticClass()))
    {
        OnBallHit.Broadcast(0);
        return;
    }
    OnBallHit.Broadcast(1);

    FVector CurrentVelocity = BallMesh->GetPhysicsLinearVelocity();
    FVector NormalizedVelocity = CurrentVelocity.GetSafeNormal();

    FVector LocalHitLocation = OtherComp->GetComponentTransform().InverseTransformPosition(Hit.Location);

    // Assuming that the paddle's local coordinates range from -BoxExtent to +BoxExtent
    // Normalize to a value between -1 and 1 to get the reflection factor
    float ReflectionFactor = LocalHitLocation.X / OtherComp->Bounds.BoxExtent.X;

    // Change the Z component based on the reflection factor
    FVector NewDirection = FVector(ReflectionFactor / 1.25, NormalizedVelocity.Y, 0).GetSafeNormal();

    // Speed Boost: Increase speed by 1.1x (for example)
    float NewSpeed = FMath::Min(MaxMoveSpeed, CurrentVelocity.Size() * 1.15f);

    // Set the new velocity
    BallMesh->SetPhysicsLinearVelocity(NewDirection * NewSpeed);
    CurrentVelocity = BallMesh->GetPhysicsLinearVelocity();
    FVector CurrentPosition = GetActorLocation();

    // No Spin: Set angular velocity to zero
    BallMesh->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
}


// Called when the game starts or when spawned
void APongBall::BeginPlay()
{
	Super::BeginPlay();
    BallMesh->OnComponentHit.AddDynamic(this, &APongBall::OnPaddleHit);
}
