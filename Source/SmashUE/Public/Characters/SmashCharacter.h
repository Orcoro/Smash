// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Interface/CameraFollowTarget.h"
#include "SmashCharacter.generated.h"

class USmashCharacterInputData;
class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter, public ICameraFollowTarget
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion Unreal Default

#pragma region Orient
public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion Orient

#pragma region State Machine
public:
	void CreateStateMachine();
	void InitStateMachine();
	void TickStateMachine(const float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
#pragma endregion State Machine

#pragma region Input Data / Mapping Context
public:
	void SetInputMappingContext(UInputMappingContext* NewMappingContext);
	void SetInputData(USmashCharacterInputData* NewInputData);

private:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;
protected:
	void SetupMappingContextIntoController() const;

#pragma endregion Input Data / Mapping Context

#pragma region Input Move X

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, float, VelocityX);
	
public:
	float GetInputMoveX() const;

	UPROPERTY()
	FInputMoveXEvent OnInputMoveXFastEvent;

	UPROPERTY()
	FInputJumpEvent OnInputJumpEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);

	void OnInputJump(const FInputActionValue& InputActionValue);
	
#pragma endregion Input Move X

#pragma region Camera Follow Target
public:
	virtual FVector GetFollowPosition() const override;

	virtual bool IsFollowable() const override;
#pragma endregion Camera Follow Target
	
#pragma region Utils
public:
	void ResetCharacterMovement() const;
	
#pragma endregion Utils
};
