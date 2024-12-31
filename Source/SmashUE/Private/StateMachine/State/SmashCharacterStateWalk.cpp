// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/State/SmashCharacterStateWalk.h"
#include "Characters/SmashCharacter.h"
#include "Settings/SmashCharacterSettings.h"
#include "StateMachine/SmashCharacterStateMachine.h"
#include "SubSystem/SettingsSubSystem.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID() const
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->OnInputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->OnInputJumpEvent.AddDynamic(this, &USmashCharacterStateWalk::OnInputJump);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->OnInputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputMoveXFast);
	Character->OnInputJumpEvent.RemoveDynamic(this, &USmashCharacterStateWalk::OnInputJump);
}

void USmashCharacterStateWalk::StateTick(const float DeltaTime)
{
	Super::StateTick(DeltaTime);
	const float Input = Character->GetInputMoveX();
	
	if (FMath::Abs(Input) < GetWorld()->GetGameInstance()->GetSubsystem<USettingsSubSystem>()->GetCharacterSettings()->InputMoveXThreshold) {
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	} else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX() * WalkSpeed * DeltaTime);
	}
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateWalk::OnInputJump(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}
