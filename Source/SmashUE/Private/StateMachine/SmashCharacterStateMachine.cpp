// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/SmashCharacterStateMachine.h"

#include "Characters/SmashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StateMachine/State/SmashCharacterState.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	// Deplace it in SettingsSubSystem
	Animation = GetDefault<UArenaSettings>()->Montages;
	FindStates();
	InitStates();
	ChangeState(ESmashCharacterStateID::Idle);
}

void USmashCharacterStateMachine::Tick(const float DeltaTime) const
{
	if (CurrentState == nullptr) return;
	CurrentState->StateTick(DeltaTime);
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter() const
{
	return Character.Get();
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NewStateID)
{
	USmashCharacterState* NewState = GetState(NewStateID);
	if (NewState == nullptr) return;
	if (CurrentState != nullptr) {
		CurrentState->StateExit(NewStateID);
	}

	ESmashCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NewStateID;
	CurrentState = NewState;

	if (CurrentState != nullptr) {
		CurrentState->StateEnter(PreviousStateID);
	}
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID) const
{
	for (USmashCharacterState* State : AllState) {
		if (State->GetStateID() == StateID) {
			return State;
		}
	}
	return nullptr;
}

UAnimMontage* USmashCharacterStateMachine::GetAnimationByID(ESmashCharacterStateID StateID) const
{
	TSoftObjectPtr<UAnimMontage> AnimMontage = Animation[static_cast<int>(StateID) - 1];
	return AnimMontage.LoadSynchronous();
}

void USmashCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(USmashCharacterState::StaticClass());

	for (UActorComponent* StateComponent : FoundComponents) {
		USmashCharacterState* State = Cast<USmashCharacterState>(StateComponent);
		if (State == nullptr) continue;
		if (State->GetStateID() == ESmashCharacterStateID::None) continue;
		AllState.Add(State);
	}
}

void USmashCharacterStateMachine::InitStates()
{
	for (USmashCharacterState* State : AllState) {
		State->StateInit(this);
	}
}
