// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class GAM1528_PROJECT_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
	
public: 
    void BeginPlay() override;

    //aqquires all target points in the world
    UPROPERTY()
        TArray<AActor*> PatrolPoints;

    //randomly selected target point
    UFUNCTION()
        ATargetPoint* GetRandomPatrolPoints();

    //move to the randomly selected target point
    UFUNCTION()
        void GoToRandomPoint();

    //reset move
    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
    FTimerHandle TimeHandler;

};
