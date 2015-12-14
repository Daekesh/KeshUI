// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceWidgetChildManager.h"
#include "KeshUI/KUIMacros.h"
#include "KeshUI/KUIInterfaceElement.h"
#include "KUIManagedButtonWidgetChildManager.generated.h"


class UKUIButtonWidget;

/**
 * Used by buttons to filter when this component is visible.
 */
UCLASS(ClassGroup="KeshUI|Widget|Child Manager", BlueprintType, Blueprintable)
class KESHUI_API UKUIManagedButtonWidgetChildManager : public UKUIInterfaceWidgetChildManager
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIManagedButtonWidgetChildManager )

	UKUIManagedButtonWidgetChildManager( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the button widget. */
	UFUNCTION(Category="KeshUI|Widget|Button", BlueprintCallable)
	virtual UKUIButtonWidget* GetButton() const;

	/* Sets the display of this component. */
	UFUNCTION(Category="KeshUI|Widget|Button", BlueprintCallable)
	virtual void SetStates( TEnumAsByte<EKUIBoolMatch::Value> eDisabled, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver, TEnumAsByte<EKUIBoolMatch::Value> eDown );

	/* Sets the offset of the specific state conditions. */
	virtual void SetOffset( FVector2D v2Offset, TEnumAsByte<EKUIBoolMatch::Value> eDisabled = EKUIBoolMatch::V_Any, TEnumAsByte<EKUIBoolMatch::Value> eMouseOver = EKUIBoolMatch::V_Any, TEnumAsByte<EKUIBoolMatch::Value> eDown = EKUIBoolMatch::V_Any );

	/* Updates the visibility of the components. */
	void Update() override;

protected:

	EKUIBoolMatch::Value eDisabled;
	EKUIBoolMatch::Value eMouseOver;
	EKUIBoolMatch::Value eDown;

	TArray<FVector2D> arOffsets;

};
