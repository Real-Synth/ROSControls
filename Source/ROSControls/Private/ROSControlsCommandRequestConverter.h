// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#endif
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Conversion/Services/BaseRequestConverter.h"
#include "ROSControlsCommandRequestConverter.generated.h"


UCLASS()
class ROSCONTROLS_API UROSControlsCommandRequestConverter : public UBaseRequestConverter
{
	GENERATED_UCLASS_BODY()

public:

	virtual bool ConvertIncomingRequest(ROSBridgeCallServiceMsg &req, TSharedPtr<FROSBaseServiceRequest> Request) override;
	virtual bool ConvertOutgoingRequest(TSharedPtr<FROSBaseServiceRequest> Request, bson_t** BSONRequest) override;

	virtual TSharedPtr<FROSBaseServiceRequest> AllocateConcreteRequest() override;
};

