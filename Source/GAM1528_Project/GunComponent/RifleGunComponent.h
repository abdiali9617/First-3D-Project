// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunComponent.h"
#include "RifleGunComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAM1528_PROJECT_API URifleGunComponent : public UGunComponent
{
	GENERATED_BODY()
public: 
        virtual void InitializeComponent() override;
};
