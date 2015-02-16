// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Widget/KUISelectWidget.h"
#include "KUIMacros.h"
#include "KUIKeyPressInterceptWidget.generated.h"

class UKUIKeyPressInterceptWidget;

KUI_DECLARE_DELEGATE_TwoParams( FKUIInterceptedKeyPress, UKUIKeyPressInterceptWidget*, const FKeyBind& );
KUI_DECLARE_DELEGATE_OneParam( FKUIKeyInterceptCancelled, UKUIKeyPressInterceptWidget* );


/* Intercepts a single key press with modifiers (ctrl/alt/shift/cmd). */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIKeyPressInterceptWidget : public UKUISelectWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIKeyPressInterceptWidget )

public:

	/* Gets whether this key intercept will allow the scape key to stop the key intercept and trigger the canceled delegate. */
	UFUNCTION( Category = "KeshUI | Wigdet | Key Intercept", BlueprintCallable )
	virtual bool IsCancellable() const;

	/* Sets whether this key intercept will allow the scape key to stop the key intercept and trigger the canceled delegate. */
	UFUNCTION( Category = "KeshUI | Wigdet | Key Intercept", BlueprintCallable )
	virtual void SetCancellable( bool bCancellable );

	/* Returns true if this widget will intercept the next key press. */
	UFUNCTION( Category = "KeshUI | Wigdet | Key Intercept", BlueprintCallable )
	virtual bool WillInterceptNextKeyPress() const;

	/* Initiate this widget to accept the next key press. */
	UFUNCTION( Category = "KeshUI | Wigdet | Key Intercept", BlueprintCallable )
	virtual void InterceptNextKeyPress();

	/* Sets the callback executed when a key press is intercepted. */
	virtual void SetInterceptedKeyPressDelegate( UObject* oObject, FKUIInterceptedKeyPressPrototype fnCallback );

	/* Sets the callback executed when a key press is intercepted. */
	virtual void SetKeyPressCancelledDelegate( UObject* oObject, FKUIKeyInterceptCancelledPrototype fnCallback );

	virtual bool CanReceieveMouseEvents() const override;
	virtual bool CanReceieveKeyEvents() const override;
	virtual bool IsInputEventConsumer() const override;

protected:

	bool bAllowCancel;
	FKUIInterceptedKeyPressDelegate dgInterceptedKeyPress;
	FKUIKeyInterceptCancelledDelegate dgInterceptCancelled;

	virtual bool OnKeyDown( const FKUIInterfaceContainerKeyEvent& stEventInfo ) override;

	virtual bool OnMouseButtonDown( const FKUIInterfaceContainerMouseButtonEvent& stEventInfo ) override;

	virtual void KeyIntercept( const FKey& stKey );

};
