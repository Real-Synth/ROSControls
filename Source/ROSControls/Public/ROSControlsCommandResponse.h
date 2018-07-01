#pragma once

#include "CoreMinimal.h"

class ROSCONTROLS_API FROSControlsCommandResponse : public FROSBaseServiceResponse {

public:
    FROSControlsCommandResponse() = default;
    ~FROSControlsCommandResponse() = default;

    FString _Data;
    FString _Error;
};
