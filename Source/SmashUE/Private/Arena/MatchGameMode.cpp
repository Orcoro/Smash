// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/MatchGameMode.h"

#include "SubSystem/SettingsSubSystem.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SmashCharacter.h"
#include "Settings/ArenaSettings.h"
#include "Settings/SmashCharacterSettings.h"

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);
}

void AMatchGameMode::FindPlayerStartInArena(TArray<AArenaPlayerStart*>& ResultActor) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;
		
		ResultActor.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*> &PlayerStartsPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	if (InputMappingContext == nullptr || InputData == nullptr) return;
	
	for (AArenaPlayerStart* SpawnPoint : PlayerStartsPoints) {
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = AMatchGameMode::GetSmashCharacterFromInputType(InputType);
		if (SmashCharacterClass == nullptr){
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("No character for input type %d"), InputType));
			continue;
		}
		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(SmashCharacterClass, SpawnPoint->GetTransform());
		if (NewCharacter == nullptr) continue;
		NewCharacter->SetInputData(InputData);
		NewCharacter->SetInputMappingContext(InputMappingContext);
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		CharactersInsideArena.Add(NewCharacter);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Character %s spawned"), *SmashCharacterClass->GetFName().ToString()));
	}
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig() const
{
	const USettingsSubSystem* SettingsSubSystem = GetGameInstance()->GetSubsystem<USettingsSubSystem>();

	if (SettingsSubSystem == nullptr) return nullptr;
	return SettingsSubSystem->InputData.Get();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig() const
{
	const USettingsSubSystem* SettingsSubSystem = GetGameInstance()->GetSubsystem<USettingsSubSystem>();
	
	if (SettingsSubSystem == nullptr) return nullptr;
	return SettingsSubSystem->InputMappingContext.Get();
}

#pragma optimize("", off)
TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterFromInputType(EAutoReceiveInput::Type const InputType) const
{
	static const TArray<TSubclassOf<ASmashCharacter>> SmashCharacters = GetGameInstance()->GetSubsystem<USettingsSubSystem>()->SmashCharacters;
	
	if (SmashCharacters.Num() > 0 && SmashCharacters[InputType - 1]) {
		return SmashCharacters[InputType - 1];
	}
	return nullptr;
}
#pragma optimize("", on)