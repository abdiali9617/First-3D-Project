// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitDoor.generated.h"

UCLASS()
class GAM1528_PROJECT_API AExitDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
        class UBoxComponent* DoorMesh;

    UPROPERTY()
        TArray<AActor*> SpawnPoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void SpawnAtLocation();
	
};
