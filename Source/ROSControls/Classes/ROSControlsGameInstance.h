#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#endif

#include "Engine/GameInstance.h"

#include "ROSIntegrationGameInstance.h"
#include "UnrealEnginePython/Public/UnrealEnginePython.h"

#include "ROSControlsGameInstance.generated.h"

UCLASS()
class ROSCONTROLS_API UROSControlsGameInstance : public UROSIntegrationGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;
	void BeginDestroy() override;

protected:
    virtual void OnStart() override;
    virtual void LoadComplete(const float LoadTime, const FString& MapName) override;

private:

    UPROPERTY()
    class UService* PythonRequestService;
    UPROPERTY()
    class UTopic* PythonCommandTopic;

    FUnrealEnginePythonModule* PythonModule;
};
