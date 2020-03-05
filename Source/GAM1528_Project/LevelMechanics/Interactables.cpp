// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables.h"
#include "BBSpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Monster/MonsterSpawner.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInteractables::AInteractables()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BoxCollision->SetCollisionProfileName("BlockAll");
    BoxCollision->SetEnableGravity(true);
    BoxCollision->SetSimulatePhysics(true);
    RootComponent = BoxCollision;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMesh->SetCollisionProfileName("NoCollision");
    StaticMesh->SetupAttachment(RootComponent);

    Tags.Add("Pickup");

    SetReplicates(true);
    SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AInteractables::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AInteractables::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AInteractables::NetDestroyObject_Implementation()
{
    //destroys object on server
    this->Destroy();
}

bool AInteractables::NetDestroyObject_Validate()
{
    return true;
}

void AInteractables::IsTaken_Implementation()
{
    //AMonsterSpawner::PlayerHasObjective;
    //Gets all monster spawners in the world and passes them into an array
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawner::StaticClass(), MonsterSpawnerList);
    for (int i = 0; i < MonsterSpawnerList.Num(); i++)
    {
    //cycles through each element in the array and cast it as a monster spawner
        AMonsterSpawner* MSCaller = Cast<AMonsterSpawner>(MonsterSpawnerList[i]);
        //calls each spawners player has objective function
        MSCaller->PlayerHasObjective();
    }

    NetDestroyObject();
}

bool AInteractables::IsTaken_Validate()
{
    return true;
}
