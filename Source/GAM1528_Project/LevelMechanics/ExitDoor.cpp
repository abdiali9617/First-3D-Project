// Fill out your copyright notice in the Description page of Project Settings.

#include "ExitDoor.h"
#include "BBSpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AExitDoor::AExitDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DoorMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Door"));
    DoorMesh->SetupAttachment(RootComponent);

    Tags.Add("Door");
}

// Called when the game starts or when spawned
void AExitDoor::BeginPlay()
{
	Super::BeginPlay();

    //call spawn at location if role is equal to authority
    if (Role == ROLE_Authority)
        SpawnAtLocation();
	
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitDoor::SpawnAtLocation_Implementation()
{
    //gets all actors of black box spawn point and passes them into array
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABBSpawnPoint::StaticClass(), SpawnPoints);

    //chooses a random one of all avalible points
    auto index = FMath::RandRange(0, SpawnPoints.Num() - 1);
    //Cast the chose random spawn point
    ABBSpawnPoint* ObjectiveSpawnPoint = Cast<ABBSpawnPoint>(SpawnPoints[index]);
    //call the casted objects spawn object function
    ObjectiveSpawnPoint->SpawnObjective();
}

bool AExitDoor::SpawnAtLocation_Validate()
{
    return true;
}