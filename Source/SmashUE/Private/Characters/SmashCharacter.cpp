// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/SmashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inputs/SmashCharacterInputData.h"
#include "StateMachine/SmashCharacterStateMachine.h"
#include "SubSystem/CameraWorldSubSystem.h"

// Sets default values
ASmashCharacter::ASmashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();

	GetWorld()->GetSubsystem<UCameraWorldSubSystem>()->AddFollowTarget(this);
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();

	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine)
		StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(const float DeltaTime) const
{
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetInputMappingContext(UInputMappingContext* NewMappingContext)
{
	InputMappingContext = NewMappingContext;
}

void ASmashCharacter::SetInputData(USmashCharacterInputData* NewInputData)
{
	InputData = NewInputData;
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	OnInputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	OnInputJumpEvent.Broadcast(GetVelocity().X);
}

FVector ASmashCharacter::GetFollowPosition() const
{
	return GetActorLocation();
}

bool ASmashCharacter::IsFollowable() const
{
	return true;
}

void ASmashCharacter::ResetCharacterMovement() const
{
	GetCharacterMovement()->JumpZVelocity = 420.f;
	GetCharacterMovement()->AirControl = 0.05f;
	GetCharacterMovement()->GravityScale = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveX) {
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Started, this, &ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Triggered, this, &ASmashCharacter::OnInputMoveX);
		EnhancedInputComponent->BindAction(InputData->InputActionMoveX, ETriggerEvent::Completed, this, &ASmashCharacter::OnInputMoveX);
	}
	if (InputData->InputActionMoveXFast) {
		EnhancedInputComponent->BindAction(InputData->InputActionMoveXFast, ETriggerEvent::Triggered, this, &ASmashCharacter::OnInputMoveXFast);
	}
	if (InputData->InputActionJump) {
		EnhancedInputComponent->BindAction(InputData->InputActionJump, ETriggerEvent::Triggered, this, &ASmashCharacter::OnInputJump);
	}
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}
