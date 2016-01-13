// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceContainer.h"
#include "KeshUI/KUIMacros.h"
#include "KUIInterfaceWidget.generated.h"

class UKUIInterfaceWidget;

KUI_DECLARE_DELEGATE_OneParam( FKUIInterfaceWidgetValueChange, UKUIInterfaceWidget* );
KUI_DECLARE_DELEGATE_ThreeParams( FKUIInterfaceWidgetInlineValueChange, UKUIInterfaceWidget*, const void*, const void* );


UENUM( BlueprintType )
namespace EKUIInterfaceWidgetEventList
{
	enum Event
	{
		E_StateChange = 64       UMETA( DisplayName = "Widget State Change" ),
		E_ValueChange = 65       UMETA( DisplayName = "Widget Value Change" ),
		E_InlineValueChange = 66 UMETA( DisplayName = "Widget Inline Value Change" ),
		E_Click = 67             UMETA( DisplayName = "Widget Interact" )
	};
}

USTRUCT( BlueprintType )
struct FKUIInterfaceWidgetInlineValueChangeEvent : public FKUIInterfaceEvent
{
	GENERATED_USTRUCT_BODY();

	FKUIInterfaceWidgetInlineValueChangeEvent() : FKUIInterfaceEvent() { }

	FKUIInterfaceWidgetInlineValueChangeEvent( uint8 iEventID, const void* oOldValue, const void* oNewValue )
		: FKUIInterfaceEvent( iEventID )
	{
		this->oOldValue = oOldValue;
		this->oNewValue = oNewValue;
	}

	//UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Old Value Pointer (sort of)" ) )
	const void* oOldValue;

	//UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "New Value Pointer (sort of)" ) )
	const void* oNewValue;
};

class AKUIInterface;

/**
 * The base widget class.  Does not automatically dispatch mouse and key events and cannot tick by default.
 */
UCLASS(ClassGroup="KeshUI", BlueprintType, Blueprintable)
class KESHUI_API UKUIInterfaceWidget : public UKUIInterfaceContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIInterfaceWidget )

	UKUIInterfaceWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns true if this button is disabled. */
	virtual bool IsDisabled() const override;

	/* Sets the disabled state of this button.  Returns true if the value was changed. */
	UFUNCTION(Category="KeshUI|Widget", BlueprintCallable)
	virtual void SetDisabled( const TEnumAsByte<EKUIBool::SetValue> eValue );

	/* Calls the state change. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	bool bDisabled;

	/* Triggers when something changes. */
	UFUNCTION( Category = "KeshUI|Widget", BlueprintNativeEvent )
	void OnWidgetStateChange();
	virtual void OnWidgetStateChange_Implementation();

};
