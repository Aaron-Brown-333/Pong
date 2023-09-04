// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerController.h"
#include "Camera/CameraActor.h"

void APongPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //Setup player camera
    ACameraActor* CameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
    CameraActor->SetActorLocation(FVector(0, 0, 300));
    CameraActor->SetActorRotation(FRotator(-90, 0, 0));
    SetViewTarget(CameraActor);
}
