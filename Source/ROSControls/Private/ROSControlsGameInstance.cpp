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
            + FString(TEXT("game_instance = list(filter(lambda x : x.get_name() == \"")) + GetName() + FString(TEXT("\", unreal_engine.all_worlds()[-1].all_objects()))[0]\n"))
            + FString(TEXT("def return_cmd(result):\n"))
            + FString(TEXT("    game_instance.CommandResult = json.dumps(result)\n"))
            ;

    PythonModule->RunString(TCHAR_TO_UTF8(*PyCode));

    if (bConnectToROS && !PythonCommandService)
    {
        PythonCommandService = NewObject<UService>(UService::StaticClass());
        PythonCommandService->Init(ROSIntegrationCore, FString(TEXT("/unreal/python_command")), FString(TEXT("realsynth/python_command")));

        FUnrealEnginePythonModule* PyModule = PythonModule;
        auto ServiceHandlerCallback = [this, PyModule](TSharedPtr<FROSBaseServiceRequest> Request, TSharedPtr<FROSBaseServiceResponse> Response) -> void
        {
            auto ConcreteRequest = StaticCastSharedPtr<FROSControlsCommandRequest>(Request);
            if (ConcreteRequest.IsValid())
            {
                FString Command = ConcreteRequest->_Data;
                FString PyCode = FString(TEXT("return_cmd(")) + Command + FString(TEXT(")"));
                PyModule->RunString(TCHAR_TO_UTF8(*PyCode));

                auto ConcreteResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(Response);
                ConcreteResponse->_Data = this->CommandResult;
            }
        };

        PythonCommandService->Advertise(ServiceHandlerCallback);
    }
}

void RunPythonCommand(const FString& Command)
{

}


void UROSControlsGameInstance::Shutdown()
{
    if (PythonCommandService)
    {
        PythonCommandService->Unadvertise();
    }

    Super::Shutdown();
}

void UROSControlsGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}
