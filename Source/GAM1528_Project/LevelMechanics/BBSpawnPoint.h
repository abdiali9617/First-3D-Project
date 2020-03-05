// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBSpawnPoint.generated.h"

UCLASS()
class GAM1528_PROJECT_API ABBSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABBSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void SpawnObjective();
	
    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> ObjectiveSpawn;
};
