#pragma once

#include "CoreMinimal.h"

class ROSCONTROLS_API FROSControlsCommandRequest : public FROSBaseServiceRequest {

public:
    FROSControlsCommandRequest() = default;
    ~FROSControlsCommandRequest() = default;

    FString _Data;
};
