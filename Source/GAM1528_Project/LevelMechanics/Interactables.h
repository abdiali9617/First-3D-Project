// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables.generated.h"

UCLASS()
class GAM1528_PROJECT_API AInteractables : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AInteractables();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collison)
        class UBoxComponent* BoxCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Mesh)
        class UStaticMeshComponent* StaticMesh;

    UPROPERTY()
        TArray<AActor*> MonsterSpawnerList;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(Reliable, Server, WithValidation)
        void IsTaken();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
       void NetDestroyObject();
};
