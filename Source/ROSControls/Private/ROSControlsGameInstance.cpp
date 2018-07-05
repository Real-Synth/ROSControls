#include "ROSControlsGameInstance.h"
#include "ROSIntegration/Classes/RI/Service.h"
#include "ROSIntegration/Classes/RI/Topic.h"
#include "ROSControlsCommandRequest.h"
#include "ROSControlsCommandResponse.h"

void UROSControlsGameInstance::Init()
{
    Super::Init();

    PythonModule = FModuleManager::GetModulePtr<FUnrealEnginePythonModule>("UnrealEnginePython");

    // set up the return_cmd method to get the results of external calls as json string
    FString PyCode =
            FString(TEXT("import unreal_engine\n"))
            + FString(TEXT("import json\n"))
            + FString(TEXT("naichar = unreal_engine.all_worlds()[-1]\n"))
            ;

    PythonModule->RunString(TCHAR_TO_UTF8(*PyCode));

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
                FString Result;
                bool success = PythonModule->RunString(TCHAR_TO_UTF8(*RequestCommand), Result);

                auto ConcreteResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(Response);
                if (success)
                {
                    ConcreteResponse->_Data = Result;
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
                FUnrealEnginePythonModule* PM = PythonModule;
                AsyncTask(ENamedThreads::GameThread, [PM, Command]()
                {
                    PM->RunString(TCHAR_TO_UTF8(*Command));
                });
            }
        };

        PythonCommandTopic->Subscribe(CommandCallback);
    }
}

void UROSControlsGameInstance::Shutdown()
{
    Super::Shutdown();
}

void UROSControlsGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}
