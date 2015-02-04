// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KUIRenderCache.h"
#include "KUIMacros.h"
#include "KUISubContainerRenderCache.generated.h"


/**
* Wrapper for the render to texture stuff.
*/
UCLASS( ClassGroup = "KeshUI | Container", Blueprintable, BlueprintType, NotPlaceable )
class KESHUI_API UKUISubContainerRenderCache : public UKUIRenderCache
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUISubContainerRenderCache )

public:

	virtual void UpdateRenderCache( UKUIInterfaceElement* oElement ) override;

};
