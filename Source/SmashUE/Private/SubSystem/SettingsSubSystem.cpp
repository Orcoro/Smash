// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/SettingsSubSystem.h"

#include "Settings/ArenaSettings.h"
#include "Settings/SmashCharacterSettings.h"

USettingsSubSystem::USettingsSubSystem()
{
	ArenaSettings = GetDefault<UArenaSettings>();
	CharacterSettings = GetDefault<USmashCharacterSettings>();

	SmashCharacters = ArenaSettings->SmashCharacters;
	Montages = ArenaSettings->Montages;
	InputData = CharacterSettings->InputData.LoadSynchronous();
	InputMappingContext = CharacterSettings->InputMappingContext.LoadSynchronous();
}

const UArenaSettings* USettingsSubSystem::GetArenaSettings() const
{
	return ArenaSettings;
}

const USmashCharacterSettings* USettingsSubSystem::GetCharacterSettings() const
{
	return CharacterSettings;
}
