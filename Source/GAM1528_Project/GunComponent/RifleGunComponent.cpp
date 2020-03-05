// Fill out your copyright notice in the Description page of Project Settings.

#include "RifleGunComponent.h"

void URifleGunComponent::InitializeComponent()
{
    ActiveMag = 0;
    MaximumAmmo = 30;
    CurrentAmmo = MaximumAmmo;

    NumofMagsLeft.Add(CurrentAmmo);
    NumofMagsLeft.Add(CurrentAmmo);
    NumofMagsLeft.Add(CurrentAmmo);

    DamageDealt = 100;
}
