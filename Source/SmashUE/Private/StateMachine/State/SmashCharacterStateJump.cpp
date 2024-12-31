// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/State/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Settings/SmashCharacterSettings.h"
#include "StateMachine/SmashCharacterStateMachine.h"
#include "SubSystem/SettingsSubSystem.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID() const
{
	return ESmashCharacterStateID::Jump;
}

#pragma optimize("", off)
void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	const float H = JumpHeight;
	const float TH = JumpDuration / 2.f;
	const float VX = Character->GetVelocity().X;

	const float XH = VX * TH;
	const float V0 = VX == 0.f ? (2 * H) / TH : (2 * H * VX) / XH;
	float G = 0.f;
	if (VX == 0.f) {
		G = (-2 * H);
		float sub = (TH * TH);
		G = G / sub;
	}
	else {
		G =  (-2 * H * VX * VX);
		float sub = (XH * XH);
		G = G / sub;
	}
	//const float G = VX == 0.f ? (-2 * H) / (TH * TH) : (-2 * H * VX) / (XH * XH);

	constexpr float Gravity = -980.f;
	
   	Character->GetCharacterMovement()->JumpZVelocity = V0;
	Character->GetCharacterMovement()->AirControl = AirControl;
	Character->GetCharacterMovement()->GravityScale = G / Gravity;
	Character->GetCharacterMovement()->MaxWalkSpeed = AirControlSpeed;
	Character->Jump();
}
#pragma optimize("", on)

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopJumping();
}

void USmashCharacterStateJump::StateTick(const float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (Character->GetVelocity().Z <= 0.f) {
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX() * AirControlSpeed * DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Jump State %f"), Character->GetVelocity().Z));
}


