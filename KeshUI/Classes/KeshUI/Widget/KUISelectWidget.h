// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceWidget.h"
#include "KeshUI/KUIMacros.h"
#include "KUISelectWidget.generated.h"

class UKUISelectWidget;

KUI_DECLARE_DELEGATE_OneParam( FKUISelectValueChange, UKUISelectWidget* );

#define KUI_SELECT_WIDGET_MAX_VALUE 254
#define KUI_SELECT_WIDGET_NO_VALUE 255


/* Select for sliding. */
UCLASS( ClassGroup = "KeshUI|Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUISelectWidget : public UKUIInterfaceWidget
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUISelectWidget )

	UKUISelectWidget( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns true if this select has a value. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool HasValue() const;

	/* Returns the currently selected value. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual FText GetValue() const;

	/* Returns the selected index. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual uint8 GetSelectedIndex() const;

	/* Sets the currently selected value.  Returns true if valid index. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool SetValueByIndex( uint8 iIndex );

	/* Sets the currently selected value. Sets to unselected if not found.  Returns true if value was found. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool SetValueByValue( const FText& txValue );

	/* Adds a value, returning its index. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual uint8 AddValue( const FText& txValue );

	/* Removes a value by index. Returns true if removed. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool RemoveValueByIndex( uint8 iIndex );

	/* Removes a value by value. Returns true if it was removed. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool RemoveValueByValue( const FText& txValue );

	/* Returns the index of a given value. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual uint8 GetIndexOf( const FText& txValue ) const;

	/* Goes to the next value in the list. Returns true if successful. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool NextValue();

	/* Goes to the previous value in the list. Returns true if successful. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool PreviousValue();

	/* Finds the index of the given value. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual uint8 GetIndexByValue( const FText& txValue ) const;

	/* Returns the number of values. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual uint8 GetValueCount() const;

	/* Clears all values. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual void ClearValues();

	/* Returns true if this is a cyclic selection. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual bool IsCyclic() const;

	/* Sets whether this is cyclic or not. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintCallable )
	virtual void SetCyclic( bool bCyclic );

	/* Sets the value change delegate. By default it is the internal OnSliderValueChange function. */
	virtual void SetValueChangeDelegate( UObject* oObject, FKUISelectValueChangePrototype fnValueChangeCallback );

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	uint8 iSelectedValue;
	TArray<FText> arValues;
	FKUISelectValueChangeDelegate dgSelectValueChangeDelegate;
	bool bCyclic;

	/* Default on-state-change delegate. */
	virtual void OnSliderValueChange( UKUISelectWidget* cmSelect );

	/* Called when the select's value is changed. */
	UFUNCTION( Category = "KeshUI|Widget|Select", BlueprintImplementableEvent )
	virtual void OnSelectValueStateChangeBP( UKUISelectWidget* cmSelect );

};
