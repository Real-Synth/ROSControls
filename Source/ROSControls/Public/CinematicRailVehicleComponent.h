// Copyright 2018 RealSynth GmbH

#pragma once

#include "CoreMinimal.h"
#include "CameraRig_Rail.h"
#include "CinematicRailVehicleComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = "Simulation", meta = (BlueprintSpawnableComponent))
class ROSCONTROLS_API UCinematicRailVehicleComponent : public UActorComponent
{
	GENERATED_BODY()

public:

  virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

  /** vehicle simulation component */
  UPROPERTY(Category = Vehicle, EditAnywhere, meta = (AllowPrivateAccess = "true"))
  class ACameraRig_Rail* CameraRig_Rail;

private:

    UPROPERTY()
    class UTopic* CurrentPositionOnRailTopic;

    float CurrentPositionOnRail;
};
