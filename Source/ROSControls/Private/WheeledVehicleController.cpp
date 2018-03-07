// Copyright 2018 RealSynth GmbH

#include "WheeledVehicleController.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"

#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "ROSIntegration/Public/std_msgs/Float32.h"

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

    const FString& VehicleName = Vehicle->GetName();

    UROSIntegrationGameInstance* rosinst = Cast<UROSIntegrationGameInstance>(Vehicle->GetGameInstance());
    if (rosinst)
    {
        ThrottleTopic = NewObject<UTopic>(UTopic::StaticClass());
        ThrottleTopic->Init(rosinst->_Ric, FString::Printf(TEXT("/unreal/%s/throttle"), *VehicleName), TEXT("std_msgs/Float32"));

        std::function<void(TSharedPtr<FROSBaseMsg>)> ThrottleCallback = [this](TSharedPtr<FROSBaseMsg> msg) -> void
        {
            auto Concrete = StaticCastSharedPtr<ROSMessages::std_msgs::Float32>(msg);
            if (Concrete.IsValid())
            {
                this->Throttle = Concrete->_Data;
            }
        };

        if (!ThrottleTopic->Subscribe(ThrottleCallback))
        {
            ThrottleTopic = nullptr;
            UE_LOG(LogROSWheeledVehicleController, Error, TEXT("Unable to connect to rosbridge %s:%u."), *rosinst->ROSBridgeServerHost, rosinst->ROSBridgeServerPort);
        }
    }
    else
    {
        UE_LOG(LogROSWheeledVehicleController, Warning, TEXT("ROSIntegrationGameInstance does not exist."));
    }
}

void AWheeledVehicleController::Tick(const float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (VehicleMovement && ThrottleTopic)
  {
      VehicleMovement->SetThrottleInput(Throttle);
  }
}