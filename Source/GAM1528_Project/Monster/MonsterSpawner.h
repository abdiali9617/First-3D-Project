// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"


UCLASS()
class GAM1528_PROJECT_API AMonsterSpawner : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMonsterSpawner();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //to spawn one of the 2 basic monsters 
    UFUNCTION()
        void BasicSpawn();

    //to spawn the third and most dangerous monster
    UFUNCTION()
        void RevanentSpawn();

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void PlayerHasObjective();

    AMonsterSpawner* GetClass() { return this; }

    //sets cooldown to spawn to true
    void SpawnReady() { bTimeToSpawn = true; }

private:

    //variable to select which monster to spawn
    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> MonsterToSpawn;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> RevanentsToSpawn;

   UPROPERTY()
       bool bObjectiveTaken;

   bool bTimeToSpawn;

   FTimerHandle SpawnTimeHandler;

};
