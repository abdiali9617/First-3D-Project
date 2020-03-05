// Fill out your copyright notice in the Description page of Project Settings.

#include "GunComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/CollisionQueryParams.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGunComponent::InitializeComponent()
{
}


// Called every frame
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UGunComponent::NetReload_Implementation()
{
    // if there are no magazines left return
    if (NumofMagsLeft.Num() == 1)
    {
        return;
    }

    // If the ammo in the magazine is 0 pop it out of the array
    if (NumofMagsLeft[ActiveMag] == 0)
    {
        NumofMagsLeft.RemoveSingle(NumofMagsLeft[ActiveMag]);
        ActiveMag = 0;
    }

    // if the active magazine is the last one in the array set it to the start of the array
    if (ActiveMag == NumofMagsLeft.Num() - 1)
    {
        ActiveMag = 0;
    }

    // set the active mag to next one in the array
    else
    {
        ActiveMag++;
    }
}

bool UGunComponent::NetReload_Validate()
{
    return true;
}

void UGunComponent::FireGun(ACharacter * character, FHitResult &OutHit, FVector &Start, FVector &ForwardVector, FVector &End, FCollisionQueryParams & CollisionParams)
{
    // Generates a ray trace line using the info that's passed into the function
    if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
    {
        if (OutHit.GetActor() != nullptr || OutHit.GetComponent() != nullptr)
        {
            End = OutHit.Location;
        }
    }
}

void UGunComponent::NetSubtractAmmo_Implementation()
{
    // if the ammo left in the current magazine is  not 0 
    if (NumofMagsLeft[ActiveMag] != 0)
    {
        // Subtract ammo by 1
        NumofMagsLeft[ActiveMag]--;
    }
}

bool UGunComponent::NetSubtractAmmo_Validate()
{
    return true;
}

int UGunComponent::GetCurrentAmmo()
{
    // Returns the amount of ammo left in the current active magazine
    return NumofMagsLeft[ActiveMag];
}

int UGunComponent::GetDamageDealt()
{
    // Returns the Damage Dealt by the Gun
    return DamageDealt;
}

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UGunComponent, MaximumAmmo);
    DOREPLIFETIME(UGunComponent, CurrentAmmo);
    DOREPLIFETIME(UGunComponent, ActiveMag);
    DOREPLIFETIME(UGunComponent, NumofMagsLeft);
}