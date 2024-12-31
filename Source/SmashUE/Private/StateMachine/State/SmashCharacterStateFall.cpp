// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/State/SmashCharacterStateFall.h"
#include "Characters/SmashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/SmashCharacterSettings.h"
#include "StateMachine/SmashCharacterStateMachine.h"
#include "SubSystem/SettingsSubSystem.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID() const
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	Character->GetCharacterMovement()->AirControl = AirControl;
	Character->GetCharacterMovement()->GravityScale = GravityScale;
	Character->GetCharacterMovement()->MaxWalkSpeed = HorizontalMoveSpeed;
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Character->ResetCharacterMovement();
	Super::StateExit(NextStateID);
}

void USmashCharacterStateFall::StateTick(const float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if (Character->GetCharacterMovement()->IsMovingOnGround()) {
		if (FMath::Abs(Character->GetVelocity().X) <= GetWorld()->GetGameInstance()->GetSubsystem<USettingsSubSystem>()->GetCharacterSettings()->InputMoveXThreshold) {
			StateMachine->ChangeState(ESmashCharacterStateID::Idle);
		}
		if (FMath::Abs(Character->GetVelocity().X) > GetWorld()->GetGameInstance()->GetSubsystem<USettingsSubSystem>()->GetCharacterSettings()->InputMoveXThreshold) {
			StateMachine->ChangeState(ESmashCharacterStateID::Walk);
		}
	}
	Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX() * HorizontalMoveSpeed * DeltaTime);
}