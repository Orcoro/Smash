// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "SettingsSubSystem.generated.h"

class ASmashCharacter;
class USmashCharacterSettings;
class UArenaSettings;
class UInputMappingContext;
class USmashCharacterInputData;
/**
 * 
 */
UCLASS()
class SMASHUE_API USettingsSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public :
	USettingsSubSystem();

	UPROPERTY()
	TArray<TSubclassOf<ASmashCharacter>> SmashCharacters;
	UPROPERTY()
	TArray<TSoftObjectPtr<UAnimMontage>> Montages;
	
	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	const UArenaSettings* GetArenaSettings() const;
	
	const USmashCharacterSettings* GetCharacterSettings() const;

private:
	UPROPERTY()
	const UArenaSettings* ArenaSettings;
	UPROPERTY()
	const USmashCharacterSettings* CharacterSettings;
};
