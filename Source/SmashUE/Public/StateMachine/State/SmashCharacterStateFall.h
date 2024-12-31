// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() const override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HorizontalMoveSpeed = 150.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0", UIMin="0", ClampMax="1", UIMax="1"))
	float AirControl = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float GravityScale = 3.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FastGravityScale = 10.f;
};
