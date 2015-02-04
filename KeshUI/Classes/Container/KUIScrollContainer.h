// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceContainer.h"
#include "KUIMacros.h"
#include "Container/KUISubContainer.h"
#include "KUIScrollContainer.generated.h"

class UKUISliderWidget;

#define KUI_SCROLL_CONTAINER_NO_SCROLL -1.f

/**
* General container that renders a portion of its total area.
**/
UCLASS( ClassGroup = "KeshUI | Container", BlueprintType, Blueprintable )
class KESHUI_API UKUIScrollContainer : public UKUIInterfaceContainer
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIScrollContainer )

public:

	/* Overrides to add to the scroll area. */
	virtual void AddChild( UKUIInterfaceElement* oChild ) override;

	/* Overrides to remove from the scroll area. */
	virtual bool RemoveChild( UKUIInterfaceElement* oChild ) override;

	/* Updates the size of the sub container. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Returns the total size of the sub container. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual FVector2D GetScrollableSize() const;

	/* Sets the total size of the sub container. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetScrollableSize( float fWidth, float fHeight );

	/* Sets the total size of the sub container. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetScrollableSizeStruct( const FVector2D& v2ScrollSize );

	/* Returns the size of the scrollbar corner. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual FVector2D GetCornerSize() const;

	/* Sets the size of the scrollbar corner. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetCornerSize( float fWidth, float fHeight );

	/* Sets the size of the scrollbar corner. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetCornerSizeStruct( const FVector2D& v2ScrollSize );

	/* Gets the horizontal scrollbar component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual UKUISliderWidget* GetHorizontalScrollbar() const;

	/* Sets the horizontal scrollbar component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetHorizontalScrollbar( UKUISliderWidget* cmScrollbar );

	/* Gets the vertical scrollbar component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual UKUISliderWidget* GetVerticalScrollbar() const;

	/* Sets the vertical scrollbar component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetVerticalScrollbar( UKUISliderWidget* cmScrollbar );

	/* Gets the corner component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual UKUIInterfaceElement* GetCornerComponent() const;

	/* Sets the corner component. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetCornerComponent( UKUIInterfaceElement* oCorner );

	/* Updates the scrollable area. */
	virtual void OnScrollbarScrolled( UKUISliderWidget* cmSlider );

	/* Sets the corner size by fraction of over-space. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual void SetVisibleAreaByFraction( float fVertical, float fHorizontal = 0.f );

	/* Returns the scroll area, if required. */
	UFUNCTION( Category = "KeshUI | Container | Scroll", BlueprintCallable )
	virtual UKUISubContainer* GetScrollArea() const;

	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	TWeakObjectPtr<UKUISubContainer> ctScrollArea;
	FVector2D v2ScrollBarSize;
	TWeakObjectPtr<UKUISliderWidget> cmHorizontalScrollBar;
	TWeakObjectPtr<UKUISliderWidget> cmVerticalScrollBar;
	TWeakObjectPtr<UKUIInterfaceElement> oCornerComponent;
	FVector2D v2LastScrollAreaSize;

	virtual void UpdateScrollbarMetrics();

};
