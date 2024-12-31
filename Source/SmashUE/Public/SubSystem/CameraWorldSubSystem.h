// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubSystem.generated.h"

class UCameraFollowTarget;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class SMASHUE_API UCameraWorldSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region Main Camera

protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	void TickUpdateCameraZoom(float DeltaTime);
	
	void TickUpdateCameraPosition(float DeltaTime);
	
#pragma endregion Main Camera
	
public:
	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override { return TStatId(); }

	void AddFollowTarget(UObject* Target);

	void RemoveFollowTarget(UObject* Target);

	AActor* GetActorByTag(const FName &Tag) const;

protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	UPROPERTY()
	TArray<ICameraFollowTarget*> FollowTargetsInterface;

	FVector CalculateAveragePositionBetweenTargets();

	float CalculateGreatestDistanceBetweenTargets() const;

	UCameraComponent* FindCameraByTag(const FName Tag) const;

#pragma region Bound
protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundYProjectionCenter;

	AActor* FindCameraBoundsActor() const;

	void InitCameraBounds(const AActor* CameraBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position) const;

	void GetViewportBounds(FVector2D& OutViewportMin, FVector2D& OutViewportMax) const;

	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition) const;
#pragma endregion Bound

#pragma region Zoom

protected:
	UPROPERTY()
	float CameraZoomYMin = 0.0f;

	UPROPERTY()
	float CameraZoomYMax = 0.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetMin = 300.0f;

	UPROPERTY()
	float CameraZoomDistanceBetweenTargetMax = 1500.0f;

	UFUNCTION()
	void InitCameraZoomParameters();
	
#pragma endregion Zoom
};
