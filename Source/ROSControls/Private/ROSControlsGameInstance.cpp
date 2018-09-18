#include "ROSControlsGameInstance.h"
#include "ROSIntegration/Classes/RI/Service.h"
#include "ROSIntegration/Classes/RI/Topic.h"
#include "std_msgs/String.h"
#include "ROSControlsCommandRequest.h"
#include "ROSControlsCommandResponse.h"

void UROSControlsGameInstance::Init()
{
    Super::Init();
}

void UROSControlsGameInstance::OnStart()
{
    if (!PythonModule) // in PIE we need to call Load manually
    {
        LoadComplete(0.0f, TEXT(""));
    }

    if (bConnectToROS && !PythonRequestService)
    {
        PythonRequestService = NewObject<UService>(UService::StaticClass());
        PythonRequestService->Init(ROSIntegrationCore, FString(TEXT("/naichar/python_request")), FString(TEXT("realsynth/python_request")));

        auto ServiceHandlerCallback = [this](TSharedPtr<FROSBaseServiceRequest> Request, TSharedPtr<FROSBaseServiceResponse> Response) -> void
        {
            auto ConcreteRequest = StaticCastSharedPtr<FROSControlsCommandRequest>(Request);
            if (ConcreteRequest.IsValid())
            {
                FString RequestCommand = TEXT("_=") + ConcreteRequest->_Data;
                bool isOnlyCommand = ConcreteRequest->_Data.StartsWith(":");
                if (isOnlyCommand)
                {
                    RequestCommand = ConcreteRequest->_Data.RightChop(1);
                }
                FString Result;
                bool success = PythonModule->RunString(TCHAR_TO_UTF8(*RequestCommand), Result);

                auto ConcreteResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(Response);
                if (success)
                {
                    ConcreteResponse->_Data = isOnlyCommand ? TEXT("OK") : Result;
                }
                else
                {
                    ConcreteResponse->_Error = Result;
                }
                ConcreteResponse->_Result = true;
            }
        };

        PythonRequestService->Advertise(ServiceHandlerCallback, true);

        PythonCommandTopic = NewObject<UTopic>(UTopic::StaticClass());
        PythonCommandTopic->Init(ROSIntegrationCore, FString(TEXT("/naichar/python_command")), FString(TEXT("std_msgs/String")), 10);

        PythonCommandTopic->Advertise();
        std::function<void(TSharedPtr<FROSBaseMsg>)> CommandCallback = [this](TSharedPtr<FROSBaseMsg> msg) -> void
        {
            auto ConcreteStringMessage = StaticCastSharedPtr<ROSMessages::std_msgs::String>(msg);
            if (ConcreteStringMessage.IsValid())
            {
                const FString Command = ConcreteStringMessage->_Data;
				TWeakPtr<FUnrealEnginePythonModule, ESPMode::ThreadSafe> PM(PythonModule);
                AsyncTask(ENamedThreads::GameThread, [PM, Command]()
                {
					auto localPM = PM.Pin();
					if(localPM.IsValid())
					{
						localPM->RunString(TCHAR_TO_UTF8(*Command));
					}
                });
            }
        };

        PythonCommandTopic->Subscribe(CommandCallback);
    }
}

void UROSControlsGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
    Super::LoadComplete(LoadTime, MapName);

    PythonModule = MakeShareable(FModuleManager::GetModulePtr<FUnrealEnginePythonModule>("UnrealEnginePython"));

    // set up the return_cmd method to get the results of external calls as json string
    FString PyCode =
        FString(TEXT("import unreal_engine\n"))
        + FString(TEXT("import json\n"))
        + FString(TEXT("from unreal_engine import FVector, FRotator, FColor, FHitResult\n"))
        + FString(TEXT("naichar = unreal_engine.all_worlds()[-1]\n"))
        + FString(TEXT("from unreal_engine.classes import GameplayStatics\n"))
        ;

    PythonModule->RunString(TCHAR_TO_UTF8(*PyCode));
}

void UROSControlsGameInstance::Shutdown()
{
	PythonModule.Reset();

    Super::Shutdown();
}

void UROSControlsGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}
