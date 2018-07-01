#include "ROSControlsGameInstance.h"
#include "ROSIntegration/Classes/RI/Service.h"
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

    if (bConnectToROS && !PythonCommandService)
    {
        PythonCommandService = NewObject<UService>(UService::StaticClass());
        PythonCommandService->Init(ROSIntegrationCore, FString(TEXT("/unreal/python_command")), FString(TEXT("realsynth/python_command")));

        auto ServiceHandlerCallback = [this](TSharedPtr<FROSBaseServiceRequest> Request, TSharedPtr<FROSBaseServiceResponse> Response) -> void
        {
            auto ConcreteRequest = StaticCastSharedPtr<FROSControlsCommandRequest>(Request);
            if (ConcreteRequest.IsValid())
            {
                FString Command = ConcreteRequest->_Data;
                FString Result;
                bool success = PythonModule->RunString(TCHAR_TO_UTF8(*Command), Result);

                auto ConcreteResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(Response);
                if (success)
                {
                    if (Command.StartsWith("_"))
                    {
                        ConcreteResponse->_Data = Result;
                    }
                }
                else
                {
                    ConcreteResponse->_Error = Result;
                }
                ConcreteResponse->_Result = true;
            }
        };

        PythonCommandService->Advertise(ServiceHandlerCallback, true);
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
