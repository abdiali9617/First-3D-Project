// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GAM1528_PROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        class USkeletalMeshComponent* MeshComp;

    UPROPERTY(Category = Projectile, VisibleAnywhere,
        meta = (AllowPrivateAccess))
        class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        int TopSpeed = 200000;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
        int StartSpeed = 100000;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void SetFireDirection(FVector dir);
	
};
