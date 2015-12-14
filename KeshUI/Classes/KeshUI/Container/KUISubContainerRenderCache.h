// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIRenderCache.h"
#include "KeshUI/KUIMacros.h"
#include "KUISubContainerRenderCache.generated.h"


/**
* Wrapper for the render to texture stuff.
*/
UCLASS( ClassGroup = "KeshUI|Container", Blueprintable, BlueprintType, NotPlaceable )
class KESHUI_API UKUISubContainerRenderCache : public UKUIRenderCache
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUISubContainerRenderCache )

	UKUISubContainerRenderCache( const class FObjectInitializer& oObjectInitializer );

public:

	virtual void UpdateRenderCache( UKUIInterfaceElement* oElement ) override;

};
