// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widget/KUISliderWidget.h"
#include "KUIMacros.h"
#include "KUIScrollBarWidget.generated.h"

class UKUIScrollContainer;

/* Slider component using the small Slider images. */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIScrollBarWidget : public UKUISliderWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIScrollBarWidget )

public:

	/* Gets the Left/Top track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual UKUIInterfaceElement* GetLeftTopTrackComponent() const;

	/* Sets the Left/Top track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual void SetLeftTopTrackComponent( UKUIInterfaceElement* oComponent );

	/* Gets the Right/Bottom track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual UKUIInterfaceElement* GetRightBottomTrackComponent() const;

	/* Sets the Right/Bottom track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual void SetRightBottomTrackComponent( UKUIInterfaceElement* oComponent );

	/* Gets the Central track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual UKUIInterfaceElement* GetCentralTrackComponent() const;

	/* Sets the Central track component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual void SetCentralTrackComponent( UKUIInterfaceElement* oComponent );

	/* Convenience functionality.  Gets whether the central component will be stretched or tile if it is a text component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual bool ShouldTileInsteadOfStretchTrack() const;

	/* Convenience functionality.  Sets whether the central component will be stretched or tile if it is a text component. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual void SetTileInsteadOfStretchTrack( bool bValue );

	/* Gets the scroll container to move when this is changed. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual UKUIScrollContainer* GetScrollContainer() const;

	/* Sets the scroll container to move when this is changed. */
	UFUNCTION( Category = "KeshUI | Widget | Scroll Bar", BlueprintCallable )
	virtual void SetScrollContainer( UKUIScrollContainer* ctScrollContainer );

	virtual void SetSize( float fWidth, float fHeight ) override;

	virtual void SetHorizontal( bool bHorizontal ) override;

protected:

	TWeakObjectPtr<UKUIInterfaceElement> oEndLeftTop;
	TWeakObjectPtr<UKUIInterfaceElement> oEndRightBottom;
	TWeakObjectPtr<UKUIInterfaceElement> oCentral;
	bool bTileInsteadOfStretchTrack;
	TWeakObjectPtr<UKUIScrollContainer> ctScrollContainer;

	/* Updates the components elements. */
	virtual void UpdateScrollbarComponents();

	virtual void OnWidgetStateChange() override;

};
