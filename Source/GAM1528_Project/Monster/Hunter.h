// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monsters.h"
#include "Hunter.generated.h"

/**
 *
 */
UCLASS()
class GAM1528_PROJECT_API AHunter : public AMonsters
{
    GENERATED_BODY()
public:

protected:

    //override seeing and hearing to not obstruct its different behaviours towards the player
    void HasSeen(APawn* SeenPlayer) override;

    void HasHeard(APawn* LoudPlayer, const FVector& SoundLocaction, float volume) override;

    void Chase() override;
};
