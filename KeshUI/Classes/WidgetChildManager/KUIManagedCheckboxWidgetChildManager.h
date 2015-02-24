// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIInterfaceWidgetChildManager.h"
#include "KUIMacros.h"
#include "Widget/KUICheckboxWidget.h"
#include "KUIManagedCheckboxWidgetChildManager.generated.h"


/**
 * Used by buttons to filter when this component is visible.
 */
UCLASS(ClassGroup="KeshUI | Widget | Child Manager", BlueprintType, Blueprintable)
class KESHUI_API UKUIManagedCheckboxWidgetChildManager : public UKUIInterfaceWidgetChildManager
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIManagedCheckboxWidgetChildManager )

	UKUIManagedCheckboxWidgetChildManager( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the button widget. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual UKUICheckboxWidget* GetCheckbox() const;

	/* Sets the display of this component. */
	UFUNCTION(Category="KeshUI | Widget | Checkbox", BlueprintCallable)
	virtual void SetStates( TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, 
	                        TEnumAsByte<EKUICheckboxState::State> eState );

	/* Sets the offset of the specific state conditions. Use EKUICheckboxState::S_Max to match all states*/
	virtual void SetOffset( FVector2D v2Offset, TEnumAsByte<EKUIBoolMatch::Value> eDisabled = EKUIBoolMatch::V_Any, 
		                    TEnumAsByte<EKUIBoolMatch::Value> eMouseOver = EKUIBoolMatch::V_Any, 
		                    TEnumAsByte<EKUICheckboxState::State> eState = EKUICheckboxState::S_Max );

	/* Updates the visibility of the components. */
	void Update() override;

protected:

	EKUIBoolMatch::Value eDisabled;
	EKUIBoolMatch::Value eMouseOver;
	EKUICheckboxState::State eState;

	TArray<FVector2D> arOffsets;

};
