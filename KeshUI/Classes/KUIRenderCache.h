// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Component/KUITextureInterfaceComponent.h"
#include "KUIMacros.h"
#include "KUIRenderCache.generated.h"


/**
* Wrapper for the render to texture stuff.
*/
UCLASS( ClassGroup = "KeshUI", Blueprintable, BlueprintType, NotPlaceable )
class KESHUI_API UKUIRenderCache : public UKUITextureInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIRenderCache  )

	UKUIRenderCache ( const class FObjectInitializer& oObjectInitializer );

public:

	virtual void UpdateRenderCache( UKUIInterfaceElement* oElement );

	/* Returns true if the render cache doesn't need to be updated. */
	virtual bool IsRenderCacheValid() const;

	/* Invalidates the render cache, forcing an update. */
	virtual void InvalidateRenderCache();

	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	bool bValidRenderCache;

	/* Destroys the current render cache. */
	virtual void DestroyRenderCache();

	/* Creates a new render cache.  Destroys the old one first. */
	virtual void CreateRenderCache( const FVector2D& v2Size );

	virtual void InvalidateContainerRenderCache();

};
