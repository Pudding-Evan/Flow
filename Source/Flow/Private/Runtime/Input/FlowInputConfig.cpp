#include "Runtime/Input/FlowInputConfig.h"
#include "Runtime/FlowLogChannels.h"


const UInputAction* UFlowInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag)
{
	for(const FFlowInputAction& Action : NativeInputActions)
	{
		if(Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogFlow, Log, TEXT("Find Native Action for InputTag [%s]"), *InputTag.ToString());
	return nullptr;
}

