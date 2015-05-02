// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceWidget.h"
#include "KeshUI/KUIMacros.h"
#include "KUISimpleClickWidget.generated.h"

class AKUIInterface;
class UKUISimpleClickWidget;

KUI_DECLARE_DELEGATE_TwoParams( FKUISimpleClickWidgetClick, UKUISimpleClickWidget*, const FVector2D& );


USTRUCT( BlueprintType )
struct FKUIInterfaceWidgetClickEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceWidgetClickEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceWidgetClickEvent( uint8 iEventID, const FVector2D& v2ClickOffset )
		: FKUIInterfaceEvent( iEventID )
	{
		this->v2ClickOffset = v2ClickOffset;
	}

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Click Offset" ) )
	FVector2D v2ClickOffset;
};


/**
* The base click widget class.  For handling clicks.
*/
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUISimpleClickWidget : public UKUIInterfaceWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUISimpleClickWidget )

	UKUISimpleClickWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns true if the left mouse button was pressed down over this component. */
	UFUNCTION( Category = "KeshUI | Widget", BlueprintCallable )
	virtual bool IsDown() const;

	/* Sets the slider value change delegate. By default it is the internal OnSliderValueChange function. */
	virtual void SetClickDelegate( UObject* oObject, FKUISimpleClickWidgetClickPrototype fnSimpleClickWidgetClickDelegate );

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

	/* Overrides to true. */
	virtual bool CanReceieveMouseEvents() const;

protected:

	bool bMouseOver;
	bool bDown;
	FVector2D v2ClickOffset;
	FKUISimpleClickWidgetClickDelegate dgSimpleClickWidgetClick;

	/* Triggers when the mouse is moved. */
	virtual void OnMouseMove( const FKUIInterfaceContainerMouseLocationEvent& stEventInfo );

	/* Triggers when a mouse button is pressed. */
	virtual bool OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo );

	/* Triggers when a mouse button is released. */
	virtual bool OnMouseButtonUp( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo );

	/* Triggers when this widget is clicked. Must enable mouse events for this to trigger. */
	virtual void OnClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset );

	/* Triggers when this widget is clicked. Must enable mouse events for this to trigger. */
	UFUNCTION( Category = "KeshUI | Widget", BlueprintImplementableEvent )
	virtual void OnClickBP( const FKUIInterfaceWidgetClickEvent& stEventInfo );

};
