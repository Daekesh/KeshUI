// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceContainer.h"
#include "KUIMacros.h"
#include "KUISubContainer.generated.h"


/**
* General container that renders a portion of its total area.
**/
UCLASS( ClassGroup = "KeshUI | Container", BlueprintType, Blueprintable )
class KESHUI_API UKUISubContainer : public UKUIInterfaceContainer
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUISubContainer )

public:

	/* Gets the distance from the top-left of the container that the visible portion begins. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual FVector2D GetCornerOffset() const;

	/* Sets the distance from the top-left of the container that the visible portion begins. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual void SetCornerOffsetStruct( const FVector2D& v2CornerOffset );

	/* Sets the distance from the top-left of the container that the visible portion begins. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual void SetCornerOffset( float fX, float fY );

	/* Gets the total size of the container. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual FVector2D GetTotalSize() const;

	/* Sets the total size of the container. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual void SetTotalSizeStruct( const FVector2D& v2TotalSize );

	/* Sets the total size of the container. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual void SetTotalSize( float fWidth, float fHeight );

	/* Overrides to update render cache. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the element that this is a container for.  Updates total size to contain it. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual UKUIInterfaceElement* GetContainerFor() const;

	/* Sets the element that this is a container for.  Updates total size to encompass it. */
	UFUNCTION( Category = "KeshUI | Container | Sub", BlueprintCallable )
	virtual void SetContainerFor( UKUIInterfaceElement* oContainerFor );

	/* Called to render the element on the screen. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

	/* Uses custom cache class. */
	virtual void EnableRenderCache() override;
	virtual FVector2D GetNestedLocation( UKUIInterfaceContainer* ctRoot ) const override;
	virtual void BroadcastEvent( FKUIInterfaceEvent& stEventInfo, bool bTopDown = false ) override;
	virtual bool IsMouseOver() const override;

protected:

	FVector2D v2CornerOffset;
	FVector2D v2TotalSize;
	UKUIInterfaceElement* oContainerFor;

	/* Updates the render cache status if we have valid values. */
	virtual void UpdateRenderCacheSize();

};
