// Fill out your copyright notice in the Description page of Project Settings.

#include "Hunter.h"
#include "NavigationSystem.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "MonsterAIController.h"


void AHunter::HasSeen(APawn* SeenPlayer)
{
    if (SeenPlayer == nullptr)
    {
        //TargetedPlayer = nullptr;
        return;
    }

    TargetedPlayer = SeenPlayer;

    SetState(EMonsterStates::Chase);
}

void AHunter::HasHeard(APawn * LoudPlayer, const FVector & SoundLocaction, float volume)
{
    if (TargetedPlayer == nullptr)
    {
        /*FVector Dir = SoundLocaction - GetActorLocation();
        Dir.Normalize();
        FRotator FaceSound = FRotationMatrix::MakeFromX(Dir).Rotator();
        FaceSound.Pitch = 0.0f;
        FaceSound.Roll = 0.0f;
        SetActorRotation(FaceSound);
        GetWorld()->GetTimerManager().ClearTimer(HeardNoiseTimeHandler);
        GetWorld()->GetTimerManager().SetTimer(HeardNoiseTimeHandler, this, &AMonsters::SetAlert, 5.0f);*/
    }
}

void AHunter::Chase()
{
    AAIController* controller = Cast<AAIController>(GetController());

    if (CurrentMonsterLocation != TargetedPlayer)
    {
        CurrentMonsterLocation = TargetedPlayer;
    }

    UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, CurrentMonsterLocation);
}


