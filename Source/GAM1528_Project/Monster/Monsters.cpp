// Fill out your copyright notice in the Description page of Project Settings.

#include "Monsters.h"
#include "MonsterAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

#include "Engine/World.h"
#include "Player/ThePlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"

#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"

#include "Perception/PawnSensingComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMonsters::AMonsters() : Super()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    //setting up the collision component
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootComponent = GetCapsuleComponent();

    ApplyDamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ApplyDamageBox"));
    ApplyDamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ApplyDamageBox->SetCollisionProfileName("OverlapAll");
    ApplyDamageBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsters::OnOverlapBegin);
    ApplyDamageBox->SetupAttachment(RootComponent);

    BodyBox = CreateDefaultSubobject<UBoxComponent>("BodyBox");
    BodyBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BodyBox->ComponentTags.Add("Body");
    BodyBox->SetupAttachment(RootComponent);

    HeadBox = CreateDefaultSubobject<USphereComponent>("HeadBox");
    HeadBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    HeadBox->ComponentTags.Add("Head");
    HeadBox->SetupAttachment(RootComponent);

    //setting up the sensing component
    DetectionComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing Component"));
    DetectionComponent->OnSeePawn.AddDynamic(this, &AMonsters::HasSeen);
    DetectionComponent->OnHearNoise.AddDynamic(this, &AMonsters::HasHeard);

    //SetState(EMonsterStates::Idle);
    //setting basic monster health and damage
    Health = 150;
    DamageDealt = 10;
}

// Called when the game starts or when spawned
void AMonsters::BeginPlay()
{
    Super::BeginPlay();

    MonsterRotation = GetActorRotation();
   // SetHealth(150);
    SetState(EMonsterStates::Wander);
}

void AMonsters::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void AMonsters::HasSeen(APawn* SeenPlayer)
{
    if (SeenPlayer == nullptr)
    {
        TargetedPlayer = nullptr;
        return;
    }

    //if a player has been seen the player is now the target of the monster
    TargetedPlayer = SeenPlayer;

    //if target player isnt null then set the state to chase
    if (TargetedPlayer != nullptr)
    {
        SetState(EMonsterStates::Chase);
    }
}

void AMonsters::HasHeard(APawn * LoudPlayer, const FVector & SoundLocaction, float volume)
{
    if (TargetedPlayer == nullptr)
    {
        //gets the sound location and turns the actor towards the location
        FVector Dir = SoundLocaction - GetActorLocation();
        Dir.Normalize();
        FRotator FaceSound = FRotationMatrix::MakeFromX(Dir).Rotator();
        FaceSound.Pitch = 0.0f;
        FaceSound.Roll = 0.0f;
        SetActorRotation(FaceSound);
        GetWorld()->GetTimerManager().ClearTimer(HeardNoiseTimeHandler);
        //after clearing the timer it sets a new one that calls set alert to change the state to alert afte 5 seconds
        GetWorld()->GetTimerManager().SetTimer(HeardNoiseTimeHandler, this, &AMonsters::SetAlert, 5.0f);
    }
}

void AMonsters::Chase()
{
    //Move to Player
    //override the controller
    AAIController* controller = Cast<AAIController>(GetController());

    //set the location to move to as the targeted player location
    if (CurrentMonsterLocation != TargetedPlayer)
    {
        CurrentMonsterLocation = TargetedPlayer;
    }
    UAIBlueprintHelperLibrary::SimpleMoveToActor(controller, CurrentMonsterLocation);

    //if the targeted player is lost then move to alerted state
    if (TargetedPlayer == nullptr)
    {
        SetState(EMonsterStates::Alert);
    }
}

void AMonsters::Search(float Deltatime)
{
    //rotate AI in a circle then return to wandering if no player seen

    //rotate monster to look around him 
    FRotator RotateMonster = FRotator(0.0f, 10.0f, 0.0f);
    AddActorWorldRotation(RotateMonster);

    //after a couple seconds reset the state
    GetWorld()->GetTimerManager().ClearTimer(LostTargetTimeHandler);
    GetWorld()->GetTimerManager().SetTimer(LostTargetTimeHandler, this, &AMonsters::StateReset, 2.0f);
}

void AMonsters::Attacking()
{
    //if player is within range(make a skeletal mesh that overlaps)
    //attack player dealing damage
}

void AMonsters::Wander()
{
    //https://www.vikram.codes/blog/ai/01-basic-navigation
    //https://api.unrealengine.com/INT/Engine/AI/BehaviorTrees/QuickStart/3/index.html
    //https://wiki.unrealengine.com/AI_Navigation_in_C%2B%2B,_Customize_Path_Following_Every_Tick

    //continues roaming
    //call the Ai controller to continue
    &AMonsterAIController::GoToRandomPoint;

}

int AMonsters::GetDamageDealt()
{
    return DamageDealt;
}

int AMonsters::GetHealth()
{
    return Health;
}

void AMonsters::SetHealth(int health)
{
    Health = health;
}

EMonsterStates AMonsters::GetState()
{
    return CurrentState;
}

void AMonsters::AttackPlayer()
{
    // Check If the Monster is Overlapping the Player
    TArray<AActor*> OverlappingPlayers;
    ApplyDamageBox->GetOverlappingActors(OverlappingPlayers, AThePlayer::StaticClass());

    if (OverlappingPlayers.Num() > 0)
    {
        for (int i = 0; i < OverlappingPlayers.Num(); i++)
        {
            AThePlayer* player = Cast<AThePlayer>(OverlappingPlayers[i]);
            if (player->getHealth() > 0)
            {
                // ApplyDamage to the player if the condition is meet
                player->ServerApplyPlayerDamage(this);
            }
        }
    }

}

void AMonsters::SetState(EMonsterStates set)
{
    //sets the current statet to the one passed in
    //or return if it is equal to the one already passed in
    if (CurrentState == set)
        return;

    CurrentState = set;

    SetActorRotation(MonsterRotation);
}

void AMonsters::StateReset()
{
    //resets the state to wander
    SetState(EMonsterStates::Wander);

    //removes the targeted player
    TargetedPlayer = nullptr;

    SetActorRotation(MonsterRotation);

}

// Called every frame
void AMonsters::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AttackPlayer();
    //SetState of AI
    //call function for each state the monster is currently in
    if (CurrentState == EMonsterStates::Wander)
    {
        Wander();
    }
    else if (CurrentState == EMonsterStates::Chase)
    {
        Chase();
    }
    else if (CurrentState == EMonsterStates::Alert)
    {
        Search(DeltaTime);
    }

    //Face Player
    //set the monsters rotation to face the player
    if (TargetedPlayer != nullptr)
    {
        FVector MonsterLoc = GetActorLocation();
        FVector TargetLoc = TargetedPlayer->GetActorLocation();
        FVector Direction = (TargetLoc - MonsterLoc);
        Direction.Normalize();

        FMath::Lerp(GetActorRotation(), Direction.Rotation(), 0.05f);
    }
}