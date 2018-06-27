#include "ROSControlsGameInstance.h"
#include "ROSIntegration/Classes/RI/Service.h"

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
        PythonCommandService->Init(ROSIntegrationCore, FString(TEXT("/unreal/python_command")), FString(TEXT("ros_ctrl/command")));

        auto ServiceHandlerCallback = [](TSharedPtr<FROSBaseServiceRequest> Request, TSharedPtr<FROSBaseServiceResponse> Response) -> void
        {
            /*auto ConcreteRequest = StaticCastSharedPtr<ROSMessages::std_msgs::Float32>(Request);
            if (ConcreteRequest.IsValid())
            {
                *Float32Variable = Concrete->_Data;

                FString PyCode = FString(TEXT("return_cmd(")) + Command + FString(TEXT(")"));
                //PythonModule->RunString(TCHAR_TO_UTF8(*PyCode));

                // TODO: put CommandResult back into service response
            }*/
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
