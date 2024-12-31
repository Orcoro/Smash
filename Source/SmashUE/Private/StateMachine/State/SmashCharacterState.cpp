// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/State/SmashCharacterState.h"

#include "Characters/SmashCharacter.h"
#include "StateMachine/SmashCharacterStateMachine.h"

// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ESmashCharacterStateID USmashCharacterState::GetStateID() const
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = StateMachine->GetCharacter();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Init State %d"), GetStateID()));
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Enter State %d"), GetStateID()));
	PlayAnim();
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NextStateID)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Exit State %d"), GetStateID()));
	// StopAnim();
}

void USmashCharacterState::StateTick(float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Tick State %d"), GetStateID()));
}

void USmashCharacterState::PlayAnim() const
{
	UAnimMontage* Montage = StateMachine->GetAnimationByID(GetStateID());
	if (Montage && StateMachine->GetCharacter()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Play Anim %s"), *Montage->GetName()));
		StateMachine->GetCharacter()->PlayAnimMontage(Montage);
	}
}

void USmashCharacterState::StopAnim() const
{
	// UAnimMontage* Montage = StateMachine->GetAnimationByID(GetStateID());
	if (StateMachine->GetCharacter()) {
		StateMachine->GetCharacter()->StopAnimMontage();
	}
}
