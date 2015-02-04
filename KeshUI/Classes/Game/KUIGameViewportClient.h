// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIMacros.h"
#include "KUIGameViewportClient.generated.h"

/**
* Overloads the InputChar method to grab actual characters for text input.
*/
UCLASS( ClassGroup = "KeshUI | Game", Blueprintable, BlueprintType )
class KESHUI_API UKUIGameViewportClient : public UGameViewportClient
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIGameViewportClient )

public:

	virtual bool InputChar( FViewport * Viewport, int32 ControllerId, TCHAR Character ) override;

};
