 // Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Widget/KUISimpleClickWidget.h"
#include "KUIMacros.h"
#include "KUITextFieldWidget.generated.h"

class UKUITextInterfaceComponent;
class UKUITextureInterfaceComponent;
class UKUITextFieldWidget;

#define KUI_TEXT_FIELD_NO_MAX_LENGTH 0

UENUM( BlueprintType )
namespace EKUITextFilter
{
	enum Filter
	{
		F_Alpha          =  1 UMETA( DisplayName = "A-Z, a-z" ),
		F_Numeric        =  2 UMETA( DisplayName = "0-9" ),
		F_AlphaNumeric   =  3 UMETA( DisplayName = "Alpha/Numeric Characters" ),
		F_FullStop       =  4 UMETA( DisplayName = "." ),
		F_Float          =  6 UMETA( DisplayName = "0-9 and ." ),
		F_Space          =  8 UMETA( DisplayName = "Space" ),
		F_Punctuation    = 16 UMETA( DisplayName = "Other Punctuation (!''`()-:;@',.<>?/)" ),
		F_AllPunctuation = 28 UMETA( DisplayName = "All Punctuation" ),
		F_Other          = 32 UMETA( DisplayName = "Other (symbols, non-English, etc.)"),
		F_All            = 63 UMETA( DisplayName = "All" ),
		F_Max            = 64 UMETA( Hidden )
	};
}

/* Text field, for user text input. */
UCLASS(ClassGroup="KeshUI | Widget", BlueprintType, Blueprintable)
class KESHUI_API UKUITextFieldWidget : public UKUISimpleClickWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUITextFieldWidget )

public:

	/* Returns the text component so it can be styled.*/
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual UKUITextInterfaceComponent* GetTextComponent() const;

	/* Sets the caret texture. */
	virtual void SetCaretTexture( const TCHAR* tcCaretTexture );

	/* Sets the caret texture. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual void SetCaretTexture( UTexture* tCaretTexture );	

	/* Sets the caret texture offset from the current character. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual void SetCaretOffset( const FVector2D& v2Offset );

	/* Returns the string in the component. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual FString GetText() const;

	/* Sets the text in the component. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual void SetString( const FString& strString );

	/* Returns the max length of the string. KUI_TEXT_FIELD_NO_MAX_LENGTH for no max. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual uint16 GetMaxLength() const;

	/* Sets the max length of the string. KUI_TEXT_FIELD_NO_MAX_LENGTH for no max. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual void SetMaxLength( uint16 iMaxLength );

	/* Gets the current caret position as an offset from the start of the string. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual uint16 GetCaretPosition() const;

	/* Sets the current caret position as an offset from the star of the string. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintCallable)
	virtual void SetCaretPosition( uint16 iIndex );

	/* Gets the filter used to accept characters while typing.  Not applied to manually set strings. */
	UFUNCTION( Category = "KeshUI | Widget | Text Field", BlueprintCallable )
	virtual uint8 GetCharacterFilter() const;

	/* Sets the filter used to accept characters while typing.  Not applied to manually set strings. */
	UFUNCTION( Category = "KeshUI | Widget | Text Field", BlueprintCallable )
	virtual void SetCharacterFilter( uint8 iFilter );

	/* Overrides to true. */
	virtual bool CanReceieveMouseEvents() const;

	/* Overrides to true so space can click the button. */
	virtual bool CanReceieveKeyEvents() const;

	/* Sets the text change delegate. By default it is the internal OnInlineValueChange function. */
	virtual void SetInlineValueChangeDelegate( UObject* oObject, FKUIInterfaceWidgetInlineValueChangePrototype fnTextEditedCallback );

	/* Sets the text change delegate. By default it is the internal OnInlineValueChange function. */
	virtual void SetValueChangeDelegate( UObject* oObject, FKUIInterfaceWidgetValueChangePrototype fnTextUpdatedCallback );

	/* Overrides to return true. */
	virtual bool IsKeyEventConsumer() const override;

	virtual bool RespondsToEvent( uint8 iEventID ) const override;

	virtual void SendEvent( FKUIInterfaceEvent& stEventInfo ) override;

protected:

	enum EKUITextFieldWidgetLastAction
	{
		LA_None,
		LA_Add,
		LA_Delete,
		LA_Backspace
	};

	UKUITextInterfaceComponent* cmTextComponent;
	UKUITextureInterfaceComponent* cmCaret;
	FVector2D v2CaretOffset;
	FString strFullString;
	FString strEditStart;
	uint16 iRenderStringStart;
	uint16 iRenderStringLength;
	uint16 iCaretPosition;
	uint16 iMaxLength;
	bool bMouseDown;
	FKUIInterfaceWidgetInlineValueChangeDelegate dgInlineValueChange;
	FKUIInterfaceWidgetValueChangeDelegate dgValueChanged;
	uint8 iCharacterFilter;
	uint16 iUndoCaretPosition;
	FString strUndo;
	FString strLastAdd;
	EKUITextFieldWidgetLastAction eLastAction;

	/* Overrides to handle caret, text input, etc. */
	virtual void OnFocus( const FKUIInterfaceEvent& stEventInfo ) override;

	/* Overrides to handle caret, text input, etc. */
	virtual void OnBlur( const FKUIInterfaceEvent& stEventInfo ) override;

	/* Triggers when a key is pressed. */
	virtual bool OnKeyDown( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a key is released. */
	virtual bool OnKeyUp( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a key press is repeated. */
	virtual bool OnKeyRepeat( const FKUIInterfaceContainerKeyEvent& stEventInfo );

	/* Triggers when a character is input. */
	virtual bool OnKeyChar( const FKUIInterfaceContainerCharEvent& stEventInfo );

	/* Updates stuff. */
	virtual void OnWidgetStateChange() override;

	/* Adds a string at the caret position. */
	virtual void AddString( const FString& strString );

	/* Removes a char at the given position. */
	virtual void RemoveChars( uint16 iIndex, uint16 iCount = 1 );

	/* Takes focus and sets caret position. */
	virtual void OnTextBoxClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset );

	/* Default method for the text edited delegate. */
	virtual void OnInlineValueChange( UKUIInterfaceWidget* cmWidget, void* oOldValue, void* oNewValue );

	/* Default method for the text changed delegate. */
	virtual void OnValueChange( UKUIInterfaceWidget* cmWidget );

	/* Called when the text in the component is saved. */
	UFUNCTION(Category="KeshUI | Widget | Text Field", BlueprintImplementableEvent)
	virtual void OnInlineValueChangeBP( const FString& strOld, const FString& strNew );

	/* Called when the text in the component is changed, without saving its value. */
	UFUNCTION( Category = "KeshUI | Widget | Text Field", BlueprintImplementableEvent )
	virtual void OnValueChangeBP();

	virtual bool IsPunctuation( TCHAR chChar );

};
