// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterSpawner.h"
#include "Monsters.h"
#include "Revanent.h"
#include "Hunter.h"
#include "Shambler.h"

#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //sets the objective to not taken
    bObjectiveTaken = false;
    bTimeToSpawn = true;

    SetReplicates(true);
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //checks if the time to spawn is true
    if (bTimeToSpawn == true)
    {
        //if the objective is taken call revenant spawner else call basic spawner and sets the time to spawn bool to false
        if (bObjectiveTaken == true)
        {
            bTimeToSpawn = false;
            RevanentSpawn();
        }
        else
        {
            bTimeToSpawn = false;
            BasicSpawn();
        }
    }
}


void AMonsterSpawner::PlayerHasObjective()
{
    bObjectiveTaken = true;
}

void AMonsterSpawner::BasicSpawn()
{
    //creates the location to spawn monsters then spawns the assigned monster
    FVector SpawnLoc = GetActorLocation() + FVector(10.0f, 0.0f, 0.0f);
    AMonsters* NewSpawn = GetWorld()->SpawnActor<AMonsters>(MonsterToSpawn, SpawnLoc, FRotator::ZeroRotator);
    //after 5 seconds calls the spawn ready fucntion which sets the time to spawn bool to true
    GetWorldTimerManager().SetTimer(SpawnTimeHandler, this, &AMonsterSpawner::SpawnReady, 10.0f, false);
}


void AMonsterSpawner::RevanentSpawn()
{
    //creates the location to spawn monsters then spawns the assigned monster
    FVector SpawnLoc = GetActorLocation() + FVector(10.0f, 0.0f, 0.0f);
    AMonsters* NewSpawn = GetWorld()->SpawnActor<ARevanent>(RevanentsToSpawn, SpawnLoc, FRotator::ZeroRotator);
    //after 5 seconds calls the spawn ready fucntion which sets the time to spawn bool to true
    GetWorldTimerManager().SetTimer(SpawnTimeHandler, this, &AMonsterSpawner::SpawnReady, 20.0f, false);
}