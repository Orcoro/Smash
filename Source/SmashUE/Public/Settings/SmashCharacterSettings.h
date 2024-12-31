﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Engine/DeveloperSettings.h"
#include "Inputs/SmashCharacterInputData.h"
#include "SmashCharacterSettings.generated.h"
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName = "Smash Character Settings"))
class SMASHUE_API USmashCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

#pragma region Input Data / Mapping Context
public:
	UPROPERTY(Config, EditAnywhere, Category = "Input")
	TSoftObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(Config, EditAnywhere, Category = "Input")
	float InputMoveXThreshold = 0.1f;

#pragma endregion Input Data / Mapping Context
};
