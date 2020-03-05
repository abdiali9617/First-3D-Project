// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameState.h"


int AMyGameState::GetPlayerHealth()
{
    return m_PlayerHealth;
}

int AMyGameState::GetAmmo()
{
    return m_CurrentAmmo;
}

int AMyGameState::GetMaxAmmo()
{
    return m_MaxAmmo;
}

void AMyGameState::SetPlayerHealth(int health)
{
}

void AMyGameState::SetAmmo(int ammo)
{
}
