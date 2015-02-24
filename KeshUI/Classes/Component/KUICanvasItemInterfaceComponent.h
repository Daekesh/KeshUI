// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceComponent.h"
#include "KUIMacros.h"
#include "KUICanvasItemInterfaceComponent.generated.h"


/**
 * KeshUI UI Framework (KUI) Canvas Item base render component.
 */
UCLASS(Abstract, ClassGroup="KeshUI | Component", BlueprintType)
class KESHUI_API UKUICanvasItemInterfaceComponent : public UKUIInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUICanvasItemInterfaceComponent )

	UKUICanvasItemInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	~UKUICanvasItemInterfaceComponent();

	/* Gets this elements render mode. */
	virtual ESimpleElementBlendMode GetBlendMode() const;

	/* Gets this elements render mode. Blueprint mode. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual uint8 GetBlendModeBP() const;

	/* Sets this elements render mode. Blueprint mode. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual void SetBlendMode( uint8 eBlendMode );

	/* Returns whether this element has rounded position (to prevent blurring.) */
	UFUNCTION( Category = "KeshUI | Component | Canvas Item", BlueprintCallable )
	virtual bool IsRoundingPosition() const;

	/* Sets whether this element has rounded position (to prevent blurring.) */
	UFUNCTION( Category = "KeshUI | Component | Canvas Item", BlueprintCallable )
	virtual void SetRoundingPosition( bool bEnabled );

	/* Sets the linear color. */
	virtual void SetDrawColor( float fRed, float fGreen, float fBlue, float fAlpha = 255.f ) override;

	/* Returns true if this component can render. */
	virtual bool CanRender() const override;

	/* Returns true if there's enough information to render. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual bool HasValidComponents() const;

	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	bool bItemInvalidated;
	ESimpleElementBlendMode eBlendMode;
	bool bRoundPosition;
	TSharedPtr<FCanvasItem> stItem;

	/* Invalidates the FCanvasItem so it is re-created. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual void Invalidate();

	/* Called when a canvas item is invalidated. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintImplementableEvent)
	virtual void OnInvalidated();

	/* Validates the FCanvasItem after creation. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual void Validate();

	/* Called when a canvas item is validated. */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintImplementableEvent)
	virtual void OnValidated();

	/* Tries to construct a new FCanvasBorderItem! */
	UFUNCTION(Category="KeshUI | Component | Canvas Item", BlueprintCallable)
	virtual void ConstructNewItem();

};
