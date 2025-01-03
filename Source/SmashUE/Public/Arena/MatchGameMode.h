﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class USmashCharacterInputData;
class UInputMappingContext;
class ASmashCharacter;
class AArenaPlayerStart;
/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
private:
	void FindPlayerStartInArena(TArray<AArenaPlayerStart*> &ResultActor) const;

	void SpawnCharacters(const TArray<AArenaPlayerStart*> &PlayerStartsPoints);

	USmashCharacterInputData* LoadInputDataFromConfig() const;

	UInputMappingContext* LoadInputMappingContextFromConfig() const;
	
	TSubclassOf<ASmashCharacter> GetSmashCharacterFromInputType(EAutoReceiveInput::Type const InputType) const;
	
protected:
	UPROPERTY()
	TArray<ASmashCharacter*> CharactersInsideArena;

	
	
};
