// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaSettings.generated.h"


class ASmashCharacter;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName = "Arena Settings"))
class SMASHUE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TArray<TSubclassOf<ASmashCharacter>> SmashCharacters;
	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TArray<TSoftObjectPtr<UAnimMontage>> Montages;
};
