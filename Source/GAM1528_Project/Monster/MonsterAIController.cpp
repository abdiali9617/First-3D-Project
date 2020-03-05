// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAIController.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    //gets all actors of class, target points in this case
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), PatrolPoints);

    GoToRandomPoint();
}

ATargetPoint* AMonsterAIController::GetRandomPatrolPoints()
{
    //out of the array one is randomly selected
    auto index = FMath::RandRange(0, PatrolPoints.Num() - 1);
    return Cast<ATargetPoint>(PatrolPoints[index]);
}

void AMonsterAIController::GoToRandomPoint()
{
    //calls simple move to actor of randomly selected target point
    MoveToActor(GetRandomPatrolPoints());
}

void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    //once move is completed the go to random point is called once again
    GetWorldTimerManager().SetTimer(TimeHandler, this, &AMonsterAIController::GoToRandomPoint, 5.0f, false);

}