
#include "ROSControlsCommandResponseConverter.h"
#include "ROSControlsCommandResponse.h"
#include "Conversion/Messages/BaseMessageConverter.h"


UROSControlsCommandResponseConverter::UROSControlsCommandResponseConverter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	_ServiceType = "realsynth/python_command";
}

bool UROSControlsCommandResponseConverter::ConvertIncomingResponse(const ROSBridgeServiceResponseMsg &res, TSharedRef<TSharedPtr<FROSBaseServiceResponse>> Response) {

    auto ServiceResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(*Response);
    ServiceResponse->_Result = res.result_;

    bool KeyFound = false;
    FString Data = UBaseMessageConverter::GetFStringFromBSON(TEXT("values.data"), res.full_msg_bson_, KeyFound);
    if (!KeyFound) return false;

    ServiceResponse->_Data = Data;
	return true;
}

bool UROSControlsCommandResponseConverter::ConvertOutgoingResponse(TSharedPtr<FROSBaseServiceResponse> Response, ROSBridgeServiceResponseMsg &res) {
	auto ServiceResponse = StaticCastSharedPtr<FROSControlsCommandResponse>(Response);

    res.result_ = ServiceResponse->_Result;
    BSON_APPEND_UTF8(res.values_bson_, "data", TCHAR_TO_UTF8(*ServiceResponse->_Data));
	return true;
}

TSharedPtr<FROSBaseServiceResponse> UROSControlsCommandResponseConverter::AllocateConcreteResponse() {
	return MakeShareable(new FROSControlsCommandResponse);
}
