// Copyright 2018 RealSynth GmbH

#include "CinematicRailVehicleComponent.h"

#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSIntegration/Classes/ROSIntegrationGameInstance.h"
#include "ROSIntegration/Public/std_msgs/Float32.h"

DEFINE_LOG_CATEGORY_STATIC(LogROSCinematicRailVehicleComponent, Log, All);

static void CinematicSubscribeTopicFloat32(UROSIntegrationGameInstance* ROSInstance, UTopic** NewTopic, const FString& VehicleName, const FString& TopicName, float* Float32Variable, int32 QueueSize)
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

/*void AWheeledVehicleController::Possess(APawn *aPawn)
{
    Super::Possess(aPawn);

    AWheeledVehicle* Vehicle = Cast<AWheeledVehicle>(aPawn);
  
    if (!Vehicle)
    {
        UE_LOG(LogROSCinematicRailVehicleController, Error, TEXT("Vehicle Controller is not used with AWheeledVehicle."));
        return;
    }

    VehicleMovement = Vehicle->GetVehicleMovementComponent();

    const FString& VehicleName = Vehicle->GetName();

    UROSIntegrationGameInstance* ROSInstance = Cast<UROSIntegrationGameInstance>(Vehicle->GetGameInstance());
    if (ROSInstance && ROSInstance->bConnectToROS)
    {
        const int32 QueueSize = 1;
        SubscribeTopicFloat32(ROSInstance, &CurrentPositionOnRailTopic, VehicleName, TEXT("throttle"), &CurrentPositionOnRail, QueueSize);
    }
}*/

void UCinematicRailVehicleComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (CameraRig_Rail && CurrentPositionOnRailTopic)
  {
      CameraRig_Rail->CurrentPositionOnRail = CurrentPositionOnRail;
  }
}
