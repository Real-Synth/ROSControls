#pragma once

#include "CoreMinimal.h"
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

private:

    UPROPERTY()
    class UService* PythonCommandService;

    UPROPERTY()
    FString CommandResult;

    FUnrealEnginePythonModule* PythonModule;
};
