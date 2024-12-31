// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/State/SmashCharacterStateRun.h"
#include "Characters/SmashCharacter.h"
#include "Settings/SmashCharacterSettings.h"
#include "StateMachine/SmashCharacterStateMachine.h"
#include "SubSystem/SettingsSubSystem.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID() const
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->OnInputJumpEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputJump);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->OnInputJumpEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputJump);
}

void USmashCharacterStateRun::StateTick(const float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < GetWorld()->GetGameInstance()->GetSubsystem<USettingsSubSystem>()->GetCharacterSettings()->InputMoveXThreshold) {
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	} else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX() * RunSpeed * DeltaTime);
	}
}

void USmashCharacterStateRun::OnInputJump(float X)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}
