// Copyright 2018 RealSynth GmbH

#include "WheeledVehicleController.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogROSWheeledVehicleController, Log, All);

void AWheeledVehicleController::Possess(APawn *aPawn)
{
    Super::Possess(aPawn);

    AWheeledVehicle* Vehicle = Cast<AWheeledVehicle>(aPawn);
  
    if (!Vehicle)
    {
        UE_LOG(LogROSWheeledVehicleController, Error, TEXT("Vehicle Controller is not used with AWheeledVehicle."));
        return;
    }

    VehicleMovement = Vehicle->GetVehicleMovementComponent();
}

void AWheeledVehicleController::Tick(const float DeltaTime)
{
  Super::Tick(DeltaTime);

  if(VehicleMovement) 
  {
      VehicleMovement->SetThrottleInput(RawThrottleInput);
      VehicleMovement->SetSteeringInput(RawSteeringInput);
      VehicleMovement->SetHandbrakeInput(RawHandbrakeInput > 0.5f);
  }
}
