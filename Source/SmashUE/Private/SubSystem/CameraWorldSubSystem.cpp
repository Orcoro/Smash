// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/CameraWorldSubSystem.h"
#include "Camera/CameraComponent.h"
#include "Interface/CameraFollowTarget.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubSystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubSystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain")); 

	if (const AActor* CameraBoundsActor = FindCameraBoundsActor(); CameraBoundsActor != nullptr) {
		InitCameraBounds(CameraBoundsActor);
	}
	InitCameraZoomParameters();
}

void UCameraWorldSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("CameraBoundsMin %s"), *CameraBoundsMin.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("CameraBoundsMax %s"), *CameraBoundsMax.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("CameraBoundYProjectionCenter %f"), CameraBoundYProjectionCenter));
}

void UCameraWorldSubSystem::AddFollowTarget(UObject* Target)
{
	if (Target == nullptr || FollowTargets.Contains(Target)) return;
	FollowTargets.Add(Target);
	ICameraFollowTarget* FollowTargetI = Cast<ICameraFollowTarget>(Target);
	if (FollowTargetI->IsFollowable()) {
		FollowTargetsInterface.Add(FollowTargetI);
	}
}

void UCameraWorldSubSystem::RemoveFollowTarget(UObject* Target)
{
	if (Target == nullptr || !FollowTargets.Contains(Target)) return;
	FollowTargets.Remove(Target);
	ICameraFollowTarget* FollowTargetI = Cast<ICameraFollowTarget>(Target);
	if (FollowTargetI->IsFollowable()) {
		FollowTargetsInterface.Remove(FollowTargetI);
	}
}

void UCameraWorldSubSystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain == nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float DistanceOffset = CameraZoomYMax - CameraZoomYMin;
	float ZoomValue = GreatestDistanceBetweenTargets / DistanceOffset;
	float NewCameraZoom = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, ZoomValue);
	FVector CameraLocation = CameraMain->GetOwner()->GetActorLocation();
	CameraLocation.Y = NewCameraZoom;
	CameraMain->GetOwner()->SetActorLocation(CameraLocation);
}

float UCameraWorldSubSystem::CalculateGreatestDistanceBetweenTargets() const
{
	float GreatestDistance = 0.0f;

	for (int i = 0; i < FollowTargetsInterface.Num(); i++) {
		if (FollowTargetsInterface[i]->IsFollowable()) {
			for (int j = i + 1; j < FollowTargetsInterface.Num(); j++) {
				if (FollowTargetsInterface[j]->IsFollowable()) {
					const float Distance = FVector::Dist(FollowTargetsInterface[i]->GetFollowPosition(), FollowTargetsInterface[j]->GetFollowPosition());
					if (Distance > GreatestDistance) {
						GreatestDistance = Distance;
					}
				}
			}
		}
	}
	
	GreatestDistance -= CameraZoomYMin;
	if (GreatestDistance < 0) GreatestDistance = 0;
	return GreatestDistance;
}

void UCameraWorldSubSystem::InitCameraZoomParameters()
{
	const AActor* ZoomMin = GetActorByTag(TEXT("CameraDistanceMin"));
	const AActor* ZoomMax = GetActorByTag(TEXT("CameraDistanceMax"));

	if (ZoomMin != nullptr && ZoomMax != nullptr) {
		CameraZoomYMin = ZoomMin->GetActorLocation().Y;
		CameraZoomYMax = ZoomMax->GetActorLocation().Y;
	}
}

void UCameraWorldSubSystem::TickUpdateCameraPosition(float DeltaTime)
{	
	if (CameraMain == nullptr) return;
	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	ClampPositionIntoCameraBounds(AveragePosition);
	CameraMain->SetWorldLocation(AveragePosition);
}

FVector UCameraWorldSubSystem::CalculateAveragePositionBetweenTargets()
{
	FVector AveragePosition = FVector::ZeroVector;

	for (const ICameraFollowTarget* FollowTargetI : FollowTargetsInterface) {
		if (FollowTargetI->IsFollowable()) {
			AveragePosition += FollowTargetI->GetFollowPosition();
		}
	}
	AveragePosition = AveragePosition / FollowTargetsInterface.Num();
	AveragePosition.Y = CameraMain->GetOwner()->GetActorLocation().Y;
	return AveragePosition;
}

UCameraComponent* UCameraWorldSubSystem::FindCameraByTag(const FName Tag) const
{
	if (const UWorld* World = GetWorld()) {
		for (TArray<TObjectPtr<AActor>> ActorList = World->GetCurrentLevel()->Actors; const TObjectPtr<AActor> It : ActorList) {
			if (It == nullptr) continue;
			TArray<UCameraComponent*> CameraComponents;
			if (It->Tags.Contains(Tag)) {
				return It->FindComponentByClass<UCameraComponent>();
			}
		}
	}
	return nullptr;
}

AActor* UCameraWorldSubSystem::GetActorByTag(const FName &Tag) const
{
	if (const UWorld* World = GetWorld()) {
		for (TArray<TObjectPtr<AActor>> ActorList = World->GetCurrentLevel()->Actors; const TObjectPtr<AActor> It : ActorList) {
			if (It == nullptr) continue;
			if (It->Tags.Contains(Tag)) {
				return It;
			}
		}
	}
	return nullptr;
}

AActor* UCameraWorldSubSystem::FindCameraBoundsActor() const
{
	const FString Tag = TEXT("CameraBounds");
	if (const UWorld* World = GetWorld()) {
		for (TArray<TObjectPtr<AActor>> ActorList = World->GetCurrentLevel()->Actors; const TObjectPtr<AActor> It : ActorList) {
			if (It == nullptr) continue;
			if (It->Tags.Contains(Tag)) {
				return It;
			}
		}
	}
	return nullptr;
}

void UCameraWorldSubSystem::InitCameraBounds(const AActor* CameraBoundsActor)
{
	FVector BoundCenter;
	FVector BoundExtents;

	CameraBoundsActor->GetActorBounds(false, BoundCenter, BoundExtents);
	CameraBoundsMin.X = BoundCenter.X - BoundExtents.X;
	CameraBoundsMin.Y = BoundCenter.Z - BoundExtents.Z;
	CameraBoundsMax.X = BoundCenter.X + BoundExtents.X;
	CameraBoundsMax.Y = BoundCenter.Z + BoundExtents.Z;
	CameraBoundYProjectionCenter = BoundCenter.Y;
}

void UCameraWorldSubSystem::ClampPositionIntoCameraBounds(FVector& Position) const
{
	FVector2D ViewportBoundsMin;
	FVector2D ViewportBoundsMax;

	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	const FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	const FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("WorldBoundsMin %s"), *WorldBoundsMin.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("WorldBoundsMax %s"), *WorldBoundsMax.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Position %s"), *Position.ToString()));
	
	const FVector WorldBoundsOffset = FVector(WorldBoundsMax - WorldBoundsMin);
	const FVector2D CameraBoundsOffset = FVector2D(CameraBoundsMax - CameraBoundsMin);
	const FVector2D CameraBoundsCenter = FVector2D(CameraBoundsMin + CameraBoundsMax) / 2;
	const FVector2D ViewportOffset = FVector2D(ViewportBoundsMax - ViewportBoundsMin);
	const FVector2D ViewportBoundsCenter = FVector2D(ViewportBoundsMin + ViewportBoundsMax) / 2;
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("World Bounds Offset %s"), *WorldBoundsOffset.ToString()));
	
	if (CameraBoundsMax.X < Position.X + WorldBoundsOffset.X / 2) {
		Position.X = CameraBoundsMax.X - WorldBoundsOffset.X / 2;
	}
	if (CameraBoundsMin.X > Position.X - WorldBoundsOffset.X / 2) {
		Position.X = CameraBoundsMin.X + WorldBoundsOffset.X / 2;
	}
	if (CameraBoundsMax.Y < Position.Z + -WorldBoundsOffset.Z / 2) {
        Position.Z = CameraBoundsMax.Y + WorldBoundsOffset.Z / 2;
    }
	if (CameraBoundsMin.Y > Position.Z + WorldBoundsOffset.Z / 2) {
        Position.Z = CameraBoundsMin.Y + -WorldBoundsOffset.Z / 2;
    }
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Final Position %s"), *Position.ToString()));
}

void UCameraWorldSubSystem::GetViewportBounds(FVector2D& OutViewportMin, FVector2D& OutViewportMax) const
{
	const UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	const FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	const FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	OutViewportMin.X = ViewportRect.Min.X;
	OutViewportMin.Y = ViewportRect.Min.Y;

	OutViewportMax.X = ViewportRect.Max.X;
	OutViewportMax.Y = ViewportRect.Max.Y;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("ViewportMin %s"), *OutViewportMin.ToString()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("ViewportMax %s"), *OutViewportMax.ToString()));
}

FVector UCameraWorldSubSystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition) const
{
	if (CameraMain == nullptr) return FVector::Zero();
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	const float YDistanceToCenter =  CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;
	return WorldPosition;
}
