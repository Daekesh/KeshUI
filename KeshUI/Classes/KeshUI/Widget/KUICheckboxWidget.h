// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Widget/KUISimpleClickWidget.h"
#include "KeshUI/KUIMacros.h"
#include "KUICheckboxWidget.generated.h"

class UKUICheckboxWidget;

KUI_DECLARE_DELEGATE_OneParam( FKUICheckboxStateChange, UKUICheckboxWidget* );


UENUM(BlueprintType)
namespace EKUICheckboxState
{
	enum State
	{
		S_Unchecked UMETA(DisplayName="Unchecked"),
		S_Checked   UMETA(DisplayName="Checked"),
		S_NoValue   UMETA(DisplayName="No Value"),
		S_Max       UMETA(Hidden)
	};
}

/* Base class KESHUI_API for checkboxes. */
UCLASS(Abstract, Category="KeshUI | Widget", BlueprintType, Blueprintable)
class KESHUI_API UKUICheckboxWidget : public UKUISimpleClickWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUICheckboxWidget )

	UKUICheckboxWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns true if the no value state is enabled. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual bool IsNoValueStateEnabled() const;

	/* Sets whether the no value state is enabled. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual void SetNoValueStateEnabled( bool bEnabled );

	/* Gets the current state of the checkbox. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual TEnumAsByte<EKUICheckboxState::State> GetValue() const;

	/* Sets the value of the checkbox. */
	UFUNCTION( Category = "KeshUI | Widget | Checkbox", BlueprintCallable )
	virtual void SetValue( const TEnumAsByte<EKUICheckboxState::State> eValue );

	/* Returns true if the checkbox is currently checked. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual bool IsChecked() const;

	/* Overrides to true. */
	virtual bool CanReceieveMouseEvents() const;

	/* Overrides to true so space can click the checkbox. */
	virtual bool CanReceieveKeyEvents() const;

	/* Sets the checkbox click delegate. By default it is the internal OnCheckboxStateChange function. */
	virtual void SetStateChangeDelegate( UObject* oObject, FKUICheckboxStateChangePrototype fnStateChangeCallback );

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	bool bEnableNoValueState;
	EKUICheckboxState::State eState;
	FKUICheckboxStateChangeDelegate dgCheckboxStateChange;

	virtual void OnCheckboxClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset );

	/* Default on-state-change delegate. */
	virtual void OnCheckboxStateChange( UKUICheckboxWidget* cmCheckbox );

	/* Called when the checkbox's state is changed. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintImplementableEvent)
	virtual void OnCheckboxStateChangeBP( UKUICheckboxWidget* cmCheckbox );

};
