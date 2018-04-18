// Copyright 2018 RealSynth GmbH

#include "WheeledVehicleController.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"

#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "ROSIntegration/Public/std_msgs/Float32.h"

DEFINE_LOG_CATEGORY_STATIC(LogROSWheeledVehicleController, Log, All);

void SubscribeTopicFloat32(UROSIntegrationGameInstance* ROSInstance, UTopic** NewTopic, const FString& VehicleName, const FString& TopicName, float* Float32Variable, int32 QueueSize)
{
    if (*NewTopic)
    {
        (*NewTopic)->Unsubscribe();
        (*NewTopic) = nullptr;
    }

    UTopic* Topic = NewObject<UTopic>(UTopic::StaticClass());
    Topic->Init(ROSInstance->ROSIntegrationCore, FString::Printf(TEXT("/unreal/%s/%s"), *VehicleName, *TopicName), TEXT("std_msgs/Float32"), QueueSize);

    std::function<void(TSharedPtr<FROSBaseMsg>)> Callback = [Float32Variable](TSharedPtr<FROSBaseMsg> msg) -> void
    {
        auto Concrete = StaticCastSharedPtr<ROSMessages::std_msgs::Float32>(msg);
        if (Concrete.IsValid())
        {
            *Float32Variable = Concrete->_Data;
        }
    };

    Topic->Subscribe(Callback);
    *NewTopic = Topic;
}

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

    UROSIntegrationGameInstance* ROSInstance = Cast<UROSIntegrationGameInstance>(Vehicle->GetGameInstance());
    if (ROSInstance && ROSInstance->bConnectToROS)
    {
        const int32 QueueSize = 1;
        SubscribeTopicFloat32(ROSInstance, &ThrottleTopic, VehicleName, TEXT("throttle"), &Throttle, QueueSize);
        SubscribeTopicFloat32(ROSInstance, &SteeringTopic, VehicleName, TEXT("steering"), &Steering, QueueSize);
        SubscribeTopicFloat32(ROSInstance, &HandbrakeTopic, VehicleName, TEXT("handbrake"), &Handbrake, QueueSize);
    }
}

void AWheeledVehicleController::Tick(const float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (VehicleMovement && ThrottleTopic)
  {
      VehicleMovement->SetThrottleInput(Throttle);
  }
  if (VehicleMovement && SteeringTopic)
  {
      VehicleMovement->SetSteeringInput(Steering);
  }
  if (VehicleMovement && HandbrakeTopic)
  {
      VehicleMovement->SetHandbrakeInput(Handbrake > 0.5f);
  }
}
