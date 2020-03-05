// Fill out your copyright notice in the Description page of Project Settings.

#include "BBSpawnPoint.h"
#include "Interactables.h"

// Sets default values
ABBSpawnPoint::ABBSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABBSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABBSpawnPoint::SpawnObjective()
{
    FVector SpawnLoc = GetActorLocation();

    AInteractables* NewSpawn = GetWorld()->SpawnActor<AInteractables>(ObjectiveSpawn, SpawnLoc, FRotator::ZeroRotator);
}
