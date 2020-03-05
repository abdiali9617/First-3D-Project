// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GunComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAM1528_PROJECT_API UGunComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();

    UPROPERTY(BlueprintReadOnly, Replicated)
        int MaximumAmmo;

    UPROPERTY(BlueprintReadOnly, Replicated)
        int CurrentAmmo;

    UPROPERTY(BlueprintReadOnly, Replicated)
        int ActiveMag;

    UPROPERTY(BlueprintReadOnly)
        int DamageDealt;

    UPROPERTY(BlueprintReadOnly, Replicated)
        TArray<int> NumofMagsLeft;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void InitializeComponent() override;

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetReload();

    void FireGun(ACharacter* character, FHitResult &OutHit, FVector &Start, FVector &ForwardVector, FVector &End, FCollisionQueryParams &CollisionParams);

    UFUNCTION()
        int GetCurrentAmmo();

    UFUNCTION()
        int GetDamageDealt();

    UFUNCTION(Reliable, NetMulticast, WithValidation)
        void NetSubtractAmmo();
};