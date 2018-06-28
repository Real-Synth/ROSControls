// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#endif

#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Conversion/Services/BaseResponseConverter.h"
#include "ROSControlsCommandResponseConverter.generated.h"


UCLASS()
class ROSCONTROLS_API UROSControlsCommandResponseConverter : public UBaseResponseConverter
{
	GENERATED_UCLASS_BODY()

public:

	virtual bool ConvertIncomingResponse(const ROSBridgeServiceResponseMsg &res, TSharedRef<TSharedPtr<FROSBaseServiceResponse>> Response) override;
	virtual bool ConvertOutgoingResponse(TSharedPtr<FROSBaseServiceResponse> Response, ROSBridgeServiceResponseMsg &res) override;

	virtual TSharedPtr<FROSBaseServiceResponse> AllocateConcreteResponse() override;
};

