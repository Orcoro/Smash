// Fill out your copyright notice in the Description page of Project Settings.

#include "StateMachine/State/SmashCharacterStateIdle.h"
#include "SubSystem/SettingsSubSystem.h"
#include "Characters/SmashCharacter.h"
#include "Settings/SmashCharacterSettings.h"
#include "StateMachine/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID() const
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->OnInputMoveXFastEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->OnInputJumpEvent.AddDynamic(this, &USmashCharacterStateIdle::OnInputJump);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->OnInputMoveXFastEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputMoveXFast);
	Character->OnInputJumpEvent.RemoveDynamic(this, &USmashCharacterStateIdle::OnInputJump);
}

void USmashCharacterStateIdle::StateTick(const float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if (FMath::Abs(Character->GetInputMoveX()) > GetWorld()->GetGameInstance()->GetSubsystem<USettingsSubSystem>()->GetCharacterSettings()->InputMoveXThreshold) {
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float X)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

void USmashCharacterStateIdle::OnInputJump(float X)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}
