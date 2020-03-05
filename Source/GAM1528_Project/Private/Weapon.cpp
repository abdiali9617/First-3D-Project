// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include <Components/ArrowComponent.h>

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;

    SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
    SpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::SpawnBullet()
{
    if (Projectile != nullptr)
    {
        UWorld* const World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.SpawnCollisionHandlingOverride =
                ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            FTransform SpawnTransform =
                SpawnPoint->GetComponentTransform();
            AProjectile* SpawnedActor =
                World->SpawnActor<AProjectile>(Projectile,
                    SpawnTransform,
                    SpawnParams);
            if (SpawnedActor)
            {
                FVector direction = FRotationMatrix(SpawnTransform.Rotator()).GetScaledAxis(EAxis::X);

                SpawnedActor->SetFireDirection(direction);
            }
        }
    }
}

void AWeapon::Fire()
{
    SpawnBullet();
}

