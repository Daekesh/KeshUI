// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Widget/KUISelectWidget.h"
#include "KUIMacros.h"
#include "KUIScrollSelectWidget.generated.h"

class UKUIScrollSelectWidget;
class UKUISimpleClickWidget;
class UKUIButtonWidget;
class UKUITextInterfaceComponent;

KUI_DECLARE_DELEGATE_OneParam( FKUIScrollSelectValueChange, UKUIScrollSelectWidget* );


/* Scrollable select widget base class. */
UCLASS( ClassGroup = "KeshUI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIScrollSelectWidget : public UKUISelectWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIScrollSelectWidget )

	UKUIScrollSelectWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Sets the left button. */
	UFUNCTION( Category = "KeshUI | Widget | Select", BlueprintCallable )
	virtual void SetLeftButton( UKUIButtonWidget* cmLeftButton );

	/* Sets the right button. */
	UFUNCTION( Category = "KeshUI | Widget | Select", BlueprintCallable )
	virtual void SetRightButton( UKUIButtonWidget* cmRightButton );

	/* Disables the buttons. */
	virtual void SetDisabled( const TEnumAsByte<EKUIBool::SetValue> eValue ) override;

	/* Sets the current value of the text field. */
	virtual bool SetValueByIndex( uint8 iIndex ) override;

	/* Update button states. */
	virtual uint8 AddValue( const FText& txValue ) override;

	/* Update button states. */
	virtual bool RemoveValueByIndex( uint8 iIndex ) override;

	/* Returns the text component so it can be styled.*/
	UFUNCTION( Category = "KeshUI | Widget | Select", BlueprintCallable )
	virtual UKUITextInterfaceComponent* GetTextComponent();

protected:

	TWeakObjectPtr<UKUIButtonWidget> cmLeftButton;
	TWeakObjectPtr<UKUIButtonWidget> cmRightButton;
	TWeakObjectPtr<UKUITextInterfaceComponent> cmText;
	FVector2D v2LargestTextSize;

	/* Called when the left button is clicked. */
	virtual void OnLeftButtonClick( UKUISimpleClickWidget* cmButton, const FVector2D& v2ClickOffset );

	/* Called when the right button is clicked. */
	virtual void OnRightButtonClick( UKUISimpleClickWidget* cmButton, const FVector2D& v2ClickOffset );

	/* Makes sure size is correct, etc. */
	virtual void DoLayout() override;

	/* Calculates the largest text size. */
	virtual void UpdateLargestTextSize();

	/* Updates the buttons' disabled states. */
	virtual void UpdateDisabledStates();

};
