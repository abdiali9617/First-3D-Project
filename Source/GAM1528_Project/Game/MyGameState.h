// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class GAM1528_PROJECT_API AMyGameState : public AGameState
{
	GENERATED_BODY()
protected:
    int m_PlayerHealth = 100;

    int m_CurrentAmmo = 30;

    int m_MaxAmmo = 30;


public:
        UFUNCTION(BlueprintCallable, Category = "Health")
        int GetPlayerHealth();

       UFUNCTION(BlueprintCallable, Category = "Ammo")
        int GetAmmo();

       UFUNCTION(BlueprintCallable, Category = "Ammo")
           int GetMaxAmmo();
	
       void SetPlayerHealth(int health);

       void SetAmmo(int ammo);
};
