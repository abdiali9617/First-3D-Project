// Fill out your copyright notice in the Description page of Project Settings.

#include "PistolGunComponent.h"
#include "GunComponent.h"

void UPistolGunComponent::InitializeComponent()
{
    ActiveMag = 0;
    MaximumAmmo = 20;
    CurrentAmmo = MaximumAmmo;

    NumofMagsLeft.Add(CurrentAmmo);
    NumofMagsLeft.Add(CurrentAmmo);

    DamageDealt = 75;
}
