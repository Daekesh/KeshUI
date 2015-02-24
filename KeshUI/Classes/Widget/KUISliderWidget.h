// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceWidget.h"
#include "KUIMacros.h"
#include "KUISliderWidget.generated.h"

class UKUISliderWidget;

KUI_DECLARE_DELEGATE_OneParam( FKUISliderValueChange, UKUISliderWidget* );


/* Slider for sliding. */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUISliderWidget : public UKUIInterfaceWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUISliderWidget )

	UKUISliderWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the element for the handle. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual UKUIInterfaceElement* GetHandle() const;

	/* Sets the element for the handle. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetHandle( UKUIInterfaceElement* oHandle );

	/* Gets the handle offset. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual FVector2D GetHandleOffset() const;

	/* Sets the handle offset. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetHandleOffset( const FVector2D& v2HandleOffset );

	/* Gets the deadzone at the end of the track above/below which the max/min value is used. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual float GetDeadZone() const;

	/* Sets the deadzone at the end of the track. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetDeadZone( float fDeadZone );

	/* Gets the distance from the end of the track that the handle will stop. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual float GetEndSize() const;

	/* Sets the distance from the end of the track that the handle will stop. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetEndSize( float fEndSize );

	/* Gets whether this is a horizontal or vertical slider. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual bool IsHorizontal() const;

	/* Sets whether this is a horizontal or vertical slider. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetHorizontal( bool bHorizontal );

	/* Gets the step count for this slider, 0 for continuous. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual uint8 GetStepCount() const;

	/* Sets the step count for this slider, 0 for continuous. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetStepCount( uint8 iStepCount );

	/* Gets the value of this slider.  0 -> 1 for continuous, 0 to stepcount for discrete. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual float GetValue() const;

	/* Sets the value of this slider. 0 -> 1 for continuous, 0 to stepcount for discrete. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual void SetValue( float fValue );

	/* Overrides to true. */
	virtual bool CanReceieveMouseEvents() const;

	/* Overrides to true so arrows keys can move the handle. */
	virtual bool CanReceieveKeyEvents() const;

	/* Sets the slider value change delegate. By default it is the internal OnSliderValueChange function. */
	virtual void SetValueChangeDelegate( UObject* oObject, FKUISliderValueChangePrototype fnSliderValueChangeCallback );

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	TWeakObjectPtr<UKUIInterfaceElement> oHandle;
	float fDeadZone;
	float fEndSize;
	FVector2D v2HandleOffset;
	bool bHorizontal;
	uint8 iStepCount;
	float fValue;
	bool bDragging;
	float fMouseDownOffset;
	FKUISliderValueChangeDelegate dgSliderChange;

	/* Triggers when the mouse is moved. */
	virtual void OnMouseMove( const FKUIInterfaceContainerMouseLocationEvent& stEventInfo ) override;

	/* Triggers when a mouse button is pressed. */
	virtual bool OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo ) override;

	/* Triggers when a mouse button is released. */
	virtual bool OnMouseButtonUp( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo ) override;

	/* Returns the total distance that the handle can travel. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual float GetTravelDistance() const;

	/* Returns the size of each step.  0.f if there are no steps. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintCallable )
	virtual float GetStepSize() const;

	/* Updates the components. */
	virtual void OnWidgetStateChange() override;

	/* Called when the value changes. */
	virtual void OnSliderValueChange( UKUISliderWidget* cmSlider );

	/* Called when the value changes. */
	UFUNCTION( Category = "KeshUI | Widget | Slider", BlueprintImplementableEvent )
	virtual void OnSliderValueChangeBP( UKUISliderWidget* cmSlider );

	/* Sets the value based on the current mouse position. */
	virtual void OnDrag( float fLocation );

};
