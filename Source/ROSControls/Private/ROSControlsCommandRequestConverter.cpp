
#include "ROSControlsCommandRequestConverter.h"
#include "ROSControlsCommandRequest.h"
#include "Conversion/Messages/BaseMessageConverter.h"

UROSControlsCommandRequestConverter::UROSControlsCommandRequestConverter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	_ServiceType = "realsynth/python_command";
}

bool UROSControlsCommandRequestConverter::ConvertOutgoingRequest(TSharedPtr<FROSBaseServiceRequest> Request, bson_t** BSONRequest) {

	auto ROSControlsCommandRequest = StaticCastSharedPtr<FROSControlsCommandRequest >(Request);
	*BSONRequest = BCON_NEW(
        "data", TCHAR_TO_UTF8(*ROSControlsCommandRequest->_Data)
    );
	return true;
}

bool UROSControlsCommandRequestConverter::ConvertIncomingRequest(ROSBridgeCallServiceMsg &req, TSharedPtr<FROSBaseServiceRequest> Request) {

    bool KeyFound = false;
    FString Data = UBaseMessageConverter::GetFStringFromBSON(TEXT("args.data"), req.full_msg_bson_, KeyFound);
    if (!KeyFound) return false;

    auto ServiceRequest = StaticCastSharedPtr<FROSControlsCommandRequest>(Request);
    ServiceRequest->_Data = Data;
	return true;
}

TSharedPtr<FROSBaseServiceRequest> UROSControlsCommandRequestConverter::AllocateConcreteRequest() {
	return MakeShareable(new FROSControlsCommandRequest);
}
