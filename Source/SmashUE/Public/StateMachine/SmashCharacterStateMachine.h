// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Settings/ArenaSettings.h"
#include "SmashCharacterStateMachine.generated.h"

enum class ESmashCharacterStateID : uint8;
class USmashCharacterState;
class ASmashCharacter;

/**
 * 
 */
UCLASS()
class SMASHUE_API USmashCharacterStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Init(ASmashCharacter *Character);
	void Tick(float DeltaTime) const;
	ASmashCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ESmashCharacterStateID NewStateID);

	USmashCharacterState* GetState(ESmashCharacterStateID StateID) const;
	
	UAnimMontage* GetAnimationByID(ESmashCharacterStateID StateID) const;
	
protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TArray<USmashCharacterState*> AllState;

	UPROPERTY(BlueprintReadOnly)
	ESmashCharacterStateID CurrentStateID;

	UPROPERTY()
	TArray<TSoftObjectPtr<UAnimMontage>> Animation;

	UPROPERTY()
	TObjectPtr<USmashCharacterState> CurrentState;
	
	void FindStates();

	void InitStates();
};


