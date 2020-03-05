// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThePlayer.generated.h"

UCLASS()
class GAM1528_PROJECT_API AThePlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AThePlayer();

    virtual void PostInitializeComponents() override;
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        class UCameraComponent* FPSCamera;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
        float MaxStamina;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health)
        int PlayerHealthMax;

    UPROPERTY(EditAnywhere)
        int m_Trackers;

    UPROPERTY(VisibleDefaultsOnly, Category = Box, Replicated)
        class UBoxComponent* LaserSpawn;

    UPROPERTY(VisibleDefaultsOnly, Category = Box, Replicated)
        class UCapsuleComponent* DamageCapsule;

    UPROPERTY(VisibleDefaultsOnly, Category = Box, Replicated)
        class UBoxComponent* PistolSpawnBox;

    UPROPERTY(VisibleDefaultsOnly, Category = Box, Replicated)
        class UBoxComponent* RifleSpawnBox;

    UPROPERTY(VisibleDefaultsOnly, Category = Box, Replicated)
        class UBoxComponent* MovementSoundLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sound, Replicated)
        UAudioComponent* RifleAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sound, Replicated)
        UAudioComponent* PistolAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sound, Replicated)
        UAudioComponent* RifleReloadAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Sound, Replicated)
        UAudioComponent* PistolReloadAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        UAudioComponent* WalkAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        UAudioComponent* SprintAudio;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        UAudioComponent* ActiveMovementAudio;

    UPROPERTY(EditAnywhere)
        TSubclassOf<AActor> MarksToSpawn;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, Replicated)
        USkeletalMeshComponent* FPSArmsMesh;

    // Replicated Variables
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, Replicated)
        USkeletalMeshComponent* FPSPistolMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, Replicated)
        USkeletalMeshComponent* FPSRifleMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        float CurrentStamina;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        float MaxWalkSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        float SprintWalkSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, Replicated)
        float RegularWalkSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health, Replicated)
        int PlayerHealth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health, Replicated)
        int NumOfMedPacks;

    UPROPERTY(EditDefaultsOnly, Category = "Effects", Replicated)
        UParticleSystem * BulletImpact;

    UPROPERTY(EditDefaultsOnly, Category = "Effects", Replicated)
        UParticleSystem * PistolMuzzleFlash;

    UPROPERTY(EditDefaultsOnly, Category = "Effects", Replicated)
        UParticleSystem * RifleMuzzleFlash;

    UPROPERTY(EditDefaultsOnly, Category = "Effects", Replicated)
        UParticleSystemComponent * LaserBeam;

    UPROPERTY(VisibleAnywhere, Replicated)
        class UGunComponent* ActiveGun;

    UPROPERTY(VisibleAnywhere, Replicated)
        class UPistolGunComponent* PistolGun;

    UPROPERTY(VisibleAnywhere, Replicated)
        class URifleGunComponent* RifleGun;

    UPROPERTY(VisibleAnywhere, Replicated)
        bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, Replicated)
        bool bHasBlackBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
        FTimerHandle DamageTimer;
public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void SetAiming(float DeltaTime);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Movement Functions
    UFUNCTION()
        void MoveForward(float Val);
    UFUNCTION()
        void MoveRight(float Val);
    UFUNCTION()
        void SetMovementAudio();
    UFUNCTION()
        void OnStartJump();
    UFUNCTION()
        void OnStopJump();

    // Health Functions
    UFUNCTION(BlueprintCallable)
        int getHealth();

    UFUNCTION(BlueprintCallable)
        int getHealthPercentage();

    // Marking
    UFUNCTION()
        void MarkLocation();

    // Aiming
    UFUNCTION()
        void AimWeapon();
    UFUNCTION()
        void StopAimWeapon();

    // Damage
    UFUNCTION()
        void ApplyPlayerDamage(AActor* OtherActor);

    UFUNCTION(BlueprintCallable)
        bool IsRifleActive();

    // Rotation
    FRotator GetViewRotation() const override;

    UFUNCTION()
        void HandleOverlap(UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult);

    // Networked Functions
    UFUNCTION(Reliable, Server, WithValidation)
        void SetHealth(int health);

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerStartSprint();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerStopSprint();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerSetSprintSpeed(float Val);

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerSubtractStamina(bool jump);

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerReplenishStamina();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetSetPlayerModelOff();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerFire();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetLaser();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerReloadCurrentWeapon();

    UFUNCTION(Reliable, Client, WithValidation)
        void ClientVisibleWeapon();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetRecoil();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerSwitchWeapon();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerApplyMonsterDamage(AMonsters* monster, UPrimitiveComponent* HitComp);

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerApplyPlayerDamage(AActor* OtherActor);

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerUseMedPack();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerUseMedPackForRevival();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetPlayParticleEffect(UParticleSystem* ParticleSystem, FVector SpawnPoint);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetPlayAudio(UAudioComponent* AudioComponent);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetStopAudio(UAudioComponent* AudioComponent);

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void OnDeath();

    UFUNCTION(Reliable, Server, WithValidation)
        void ServerRevival();

    UFUNCTION(Reliable, Client, WithValidation)
        void ClientDisableInput();

    UFUNCTION(Reliable, Client, WithValidation)
        void ClientEnableInput();
        

private:
    FVector StartPos;
    bool AimisActive;
    float Active = 90.0f;
    float NotActive = 45.0f;
    bool IsMovingRight;
    bool IsMovingFoward;
    FTimerHandle MovementTimer;
};
