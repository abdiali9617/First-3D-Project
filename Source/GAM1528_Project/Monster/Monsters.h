// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monsters.generated.h"

//creating an enum of states the monster can be in
UENUM(BlueprintType)
enum class EMonsterStates : uint8
{
    Idle,
    Wander,
    Alert,
    Chase,
    Attack
};

UCLASS()
class GAM1528_PROJECT_API AMonsters : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AMonsters();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //creating the sensing component
    UPROPERTY(VisibleAnywhere, Category = "Components")
        class UPawnSensingComponent* DetectionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
        int DamageDealt;

    UPROPERTY(VisibleDefaultsOnly, Category = Box)
        class UBoxComponent* ApplyDamageBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
        class UBoxComponent* BodyBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Damage)
        class USphereComponent* HeadBox;

    UPROPERTY(EditAnywhere)
        int Health;

    //setting up functions for different states
    UFUNCTION()
        virtual void HasSeen(APawn* SeenPlayer);

    UFUNCTION()
        virtual void HasHeard(APawn* LoudPlayer, const FVector& SoundLocaction, float volume);

    UFUNCTION()
        virtual void Chase();

    UFUNCTION()
        virtual void StateReset();

    UFUNCTION()
        virtual void Search(float Deltatime);

    UFUNCTION()
        virtual void Attacking();

    UFUNCTION()
        virtual void Wander();

    virtual void SetAlert() { SetState(EMonsterStates::Alert); }

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


    UFUNCTION()
        void AttackPlayer();
    //UFUNCTION(BlueprintImplementableEvent, Category = "AI")
    virtual void SetState(EMonsterStates set);

    EMonsterStates CurrentState;

    FRotator MonsterRotation;

    AActor* CurrentMonsterLocation;

    APawn* TargetedPlayer;

    FTimerHandle LostTargetTimeHandler;
    FTimerHandle HeardNoiseTimeHandler;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    //setting up functions for dealing and recieving damage
    UFUNCTION()
        int GetDamageDealt();

    UFUNCTION()
        int GetHealth();

        void SetHealth(int health);

    UFUNCTION()
        EMonsterStates GetState();
};
