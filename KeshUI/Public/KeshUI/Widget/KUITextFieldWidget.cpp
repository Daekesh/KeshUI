// Copyright 2014-2015 Matt Chapman. All Rights Reserved.


#include "KeshUI/KeshUI.h"
#include "KeshUI/Component/KUITextInterfaceComponent.h"
#include "KeshUI/Component/KUITextureInterfaceComponent.h"
#include "KeshUI/KUIInterface.h"
#include "KeshUI/Widget/KUITextFieldWidget.h"


UKUITextFieldWidget::UKUITextFieldWidget( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	KUICreateDefaultSubobjectAssign( cmTextComponent, UKUITextInterfaceComponent, "Text Component" );
	cmTextComponent->FillContainer();
	
	AddChild( cmTextComponent );

	KUICreateDefaultSubobjectAssign( cmCaret, UKUITextureInterfaceComponent, "Caret" );
	cmCaret->SetVisible( false );
	cmCaret->SetAlignedTo( cmTextComponent );
	cmCaret->SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Left );
	cmCaret->SetVerticalAlignment( EKUIInterfaceVAlign::VA_Top );
	
	AddChild( cmCaret );	

	v2CaretOffset = FVector2D::ZeroVector;
	strFullString = "";
	strEditStart = "";
	strUndo = "";
	strLastAdd = "";
	iRenderStringLength = 0;
	iRenderStringStart = 0;
	iCaretPosition = 0;
	iUndoCaretPosition = 0;
	iMaxLength = KUI_TEXT_FIELD_NO_MAX_LENGTH;
	bMouseDown = false;
	dgInlineValueChange.BindUObject( this, &UKUITextFieldWidget::OnInlineValueChange );
	dgValueChanged.BindUObject( this, &UKUITextFieldWidget::OnValueChange );
	iCharacterFilter = EKUITextFilter::F_All;
	eLastAction = EKUITextFieldWidgetLastAction::LA_None;

	dgSimpleClickWidgetClick.BindUObject( this, &UKUITextFieldWidget::OnTextBoxClick );
}


UKUITextInterfaceComponent* UKUITextFieldWidget::GetTextComponent() const
{
	return cmTextComponent;
}


void UKUITextFieldWidget::SetCaretTexture( const TCHAR* tcCaretTexture )
{
	KUIFindObjText( tCaret, UTexture2D, tcCaretTexture );
	SetCaretTexture( tCaret.Object );
}


void UKUITextFieldWidget::SetCaretTexture( UTexture* tCaretTexture )
{
	if ( tCaretTexture )
	{
		cmCaret->SetTexture( tCaretTexture );

		if ( bFocused )
			cmCaret->SetVisible( true );

		else
			cmCaret->SetVisible( false );
	}

	else
	{
		cmCaret->SetTexture( NULL );
		cmCaret->SetVisible( false );
	}
}


void UKUITextFieldWidget::SetCaretOffset( const FVector2D& v2Offset )
{
	if ( v2Offset.X == v2CaretOffset.X && v2Offset.Y == v2CaretOffset.Y )
		return;

	v2CaretOffset = v2Offset;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


const FString& UKUITextFieldWidget::GetString() const
{
	return strFullString;
}


void UKUITextFieldWidget::SetString( const FString& strString )
{
	if ( strFullString.Equals( strString ) )
		return;

	KUISendEvent( FKUIInterfaceWidgetInlineValueChangeEvent, EKUIInterfaceWidgetEventList::E_InlineValueChange, static_cast< const void* >( &strFullString ), static_cast< const void* >( &strString ) );

	iUndoCaretPosition = iCaretPosition;
	strUndo = strFullString;
	strFullString = strString;
	iCaretPosition = min( iCaretPosition, strFullString.Len() );

	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );	
}


uint16 UKUITextFieldWidget::GetMaxLength() const
{
	return iMaxLength;
}


void UKUITextFieldWidget::SetMaxLength( uint16 iMaxLength )
{
	if ( this->iMaxLength == iMaxLength )
		return;

	this->iMaxLength = iMaxLength;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


uint16 UKUITextFieldWidget::GetCaretPosition() const
{
	return iCaretPosition;
}


void UKUITextFieldWidget::SetCaretPosition( uint16 iIndex )
{
	if ( iCaretPosition == iIndex )
		return;

	iCaretPosition = iIndex;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


uint8 UKUITextFieldWidget::GetCharacterFilter() const
{
	return iCharacterFilter;
}


void UKUITextFieldWidget::SetCharacterFilter( uint8 iFilter )
{
	iCharacterFilter = iFilter;
}


void UKUITextFieldWidget::OnFocus( const FKUIInterfaceEvent& stEventInfo )
{
	Super::OnFocus( stEventInfo );

	if ( cmCaret->GetTexture() != NULL )
		cmCaret->SetVisible( true );

	//KUILogUO( "Focus: \"%s\", \"%s\"", *strEditStart, *strFullString );
	strEditStart = strFullString;

	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


void UKUITextFieldWidget::OnBlur( const FKUIInterfaceEvent& stEventInfo )
{
	Super::OnBlur( stEventInfo );

	cmCaret->SetVisible( false );
	//KUILogUO( "Blur: \"%s\", \"%s\"", *strEditStart, *strFullString );

	KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


bool UKUITextFieldWidget::CanReceieveMouseEvents() const
{
	return true;
}


bool UKUITextFieldWidget::CanReceieveKeyEvents() const
{
	return true;
}


bool UKUITextFieldWidget::OnKeyDown( const FKUIInterfaceContainerKeyEvent& stEventInfo )
{
	if ( stEventInfo.bHandled )
		return stEventInfo.bHandled;

	if ( !bFocused )
		return stEventInfo.bHandled;

	if ( stEventInfo.eKey.IsModifierKey() )
		return false;

	if ( GetInterface() == NULL )
	{
		KUIErrorUO( "Null interface" );
		return stEventInfo.bHandled;
	}

	APlayerController* const aPC = GetInterface()->GetPlayerController();

	if ( aPC == NULL )
	{
		KUIErrorUO( "Null player controller" );
		return stEventInfo.bHandled;
	}

	if ( aPC->IsInputKeyDown( EKeys::LeftControl ) )
	{
		if ( stEventInfo.eKey == EKeys::C )
		{
			// Copy
		}

		else if ( stEventInfo.eKey == EKeys::X )
		{
			// Cut
		}

		else if ( stEventInfo.eKey == EKeys::A )
		{
			// Select all
		}

		else if ( stEventInfo.eKey == EKeys::BackSpace )
		{
			if ( iCaretPosition > 0 )
			{
				int32 iWordStart = -1;

				for ( uint16 i = iCaretPosition - 1; i >= 0; --i )
				{
					if ( !IsPunctuation( strFullString[ i ] ) )
						continue;

					iWordStart = i + 1;
					break;
				}

				if ( iWordStart == -1 )
					iWordStart = 0;

				int32 iWordEnd = -1;

				for ( uint16 i = iWordStart + 1; i < strFullString.Len(); ++i )
				{
					if ( !IsPunctuation( strFullString[ i ] ) )
						continue;

					iWordEnd = i - 1;
					break;
				}

				if ( iWordEnd == -1 )
					iWordEnd = strFullString.Len() - 1;

				iCaretPosition = iWordStart;
				RemoveChars( iWordStart, iWordEnd - iWordStart + 1);
			}
		}

		else if ( stEventInfo.eKey == EKeys::V )
		{
			FString strPastedText;
			FPlatformMisc::ClipboardPaste( strPastedText );
			AddString( strPastedText );
		}

		else if ( stEventInfo.eKey == EKeys::Z )
		{
			iCaretPosition = iUndoCaretPosition;
			SetString( strUndo );
		}

		else if ( stEventInfo.eKey == EKeys::Y )
		{
			switch ( eLastAction )
			{
				case EKUITextFieldWidgetLastAction::LA_Add:
					AddString( strLastAdd );
					break;

				case EKUITextFieldWidgetLastAction::LA_Backspace:
					if ( iCaretPosition > 0 )
					{
						--iCaretPosition;
						RemoveChars( iCaretPosition );
					}

					break;

				case EKUITextFieldWidgetLastAction::LA_Delete:
					if ( iCaretPosition < strFullString.Len() )
						RemoveChars( iCaretPosition );

					break;
			}
		}
	}

	else if ( stEventInfo.eKey == EKeys::BackSpace )
	{
		if ( iCaretPosition > 0 )
		{
			--iCaretPosition;
			RemoveChars( iCaretPosition );
		}

		eLastAction = EKUITextFieldWidgetLastAction::LA_Backspace;
	}

	else if ( stEventInfo.eKey == EKeys::Delete )
	{
		if ( iCaretPosition < strFullString.Len() )
			RemoveChars( iCaretPosition );

		eLastAction = EKUITextFieldWidgetLastAction::LA_Delete;
	}

	else if ( stEventInfo.eKey == EKeys::Left )
	{
		if ( iCaretPosition > 0 )
		{
			--iCaretPosition;
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
		}
	}

	else if ( stEventInfo.eKey == EKeys::Right )
	{
		if ( iCaretPosition < strFullString.Len() )
		{
			++iCaretPosition;
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
		}
	}

	else if ( stEventInfo.eKey == EKeys::Home )
	{
		if ( iCaretPosition > 0 )
		{
			iCaretPosition = 0;
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
		}
	}

	else if ( stEventInfo.eKey == EKeys::End )
	{
		if ( iCaretPosition < strFullString.Len() )
		{
			iCaretPosition = strFullString.Len();
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
		}
	}

	else if ( stEventInfo.eKey == EKeys::Enter )
	{
		GetInterface()->SetFocus( NULL );
		eLastAction = EKUITextFieldWidgetLastAction::LA_None;

		//KUILogUO( "Enter: \"%s\", \"%s\"", *strTemp, *strFullString );
		if ( !strEditStart.Equals( strFullString ) )
		{
			strEditStart = "";
			strUndo = strFullString;
			iUndoCaretPosition = iCaretPosition;
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );

			FKUIInterfaceEvent stSubSubEventInfo( EKUIInterfaceWidgetEventList::E_ValueChange );
			SendEvent( stSubSubEventInfo );
		}
	}

	else if ( stEventInfo.eKey == EKeys::Escape )
	{
		GetInterface()->SetFocus( NULL );
		eLastAction = EKUITextFieldWidgetLastAction::LA_None;

		//KUILogUO( "Escape: \"%s\", \"%s\"", *strTemp, *strFullString );
		if ( !strEditStart.Equals( strFullString ) )
		{
			strFullString = strEditStart;
			strUndo = strFullString;
			iUndoCaretPosition = iCaretPosition;
			KUISendSubEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
		}
	}

	return true;
}


bool UKUITextFieldWidget::OnKeyChar( const FKUIInterfaceContainerCharEvent& stEventInfo )
{
	if ( stEventInfo.bHandled )
		return stEventInfo.bHandled;

	if ( !bFocused )
		return stEventInfo.bHandled;

	switch ( stEventInfo.chChar )
	{
		// TODO: Implement tab stops
		case 9: // tab
			return true;

		case 1:		// Swallow Ctrl+A, selection not supported
		case 3:		// Swallow Ctrl+C, no copy/paste
		case 8:     // Swallow Backspace, we handle that through OnKeyDown
		case 13:	// Swallow Enter, we handle that through OnKeyDown
		case 22:	// Swallow Ctrl+V, no copy/paste
		case 24:	// Swallow Ctrl+X, no copy/paste
		case 25:	// Swallow Ctrl+Y, we handle that through OnKeyDown
		case 26:	// Swallow Ctrl+Z, we handle that through OnKeyDown
		case 27:	// Swallow ESC, we handle that through OnKeyDown
		case 127:	// Swallow CTRL+Backspace, no delete word
			return true;
	}

	bool bAdd = true;

	if ( iCharacterFilter != 0 && iCharacterFilter != EKUITextFilter::F_All )
	{
		if ( stEventInfo.chChar >= TCHAR( 'A' ) && stEventInfo.chChar <= TCHAR( 'Z' ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Alpha ) )
				bAdd = false;
		}

		else if ( stEventInfo.chChar >= TCHAR( 'a' ) && stEventInfo.chChar <= TCHAR( 'z' ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Alpha ) )
				bAdd = false;
		}

		else if ( stEventInfo.chChar >= TCHAR( '0' ) && stEventInfo.chChar <= TCHAR( '9' ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Numeric ) )
				bAdd = false;
		}

		else if ( stEventInfo.chChar == TCHAR( '.' ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_FullStop ) )
				bAdd = false;
		}

		else if ( stEventInfo.chChar == TCHAR( ' ' ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Space ) )
				bAdd = false;
		}

		else if ( IsPunctuation( stEventInfo.chChar ) )
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Punctuation ) )
				bAdd = false;
		}

		else
		{
			if ( !( iCharacterFilter & EKUITextFilter::F_Other ) )
				bAdd = false;
		}
	}

	if ( bAdd )
		AddString( stEventInfo.strChar );

	return true;
}


bool UKUITextFieldWidget::OnKeyUp( const FKUIInterfaceContainerKeyEvent& stEventInfo )
{
	return !stEventInfo.eKey.IsModifierKey();
}


bool UKUITextFieldWidget::OnKeyRepeat( const FKUIInterfaceContainerKeyEvent& stEventInfo )
{
	return OnKeyDown( stEventInfo );
}


void UKUITextFieldWidget::OnInlineValueChange( UKUIInterfaceWidget* cmWidget, const void* oOldValue, const void* oNewValue )
{
	KUILogDebugUO( "Text edited!" );
}


void UKUITextFieldWidget::SetInlineValueChangeDelegate( UObject* oObject, FKUIInterfaceWidgetInlineValueChangePrototype fnTextEditedCallback )
{
	if ( oObject == NULL || fnTextEditedCallback == NULL )
		dgInlineValueChange.Unbind();

	else
		dgInlineValueChange.BindUObject( oObject, fnTextEditedCallback );
}


void UKUITextFieldWidget::OnValueChange( UKUIInterfaceWidget* cmWidget )
{
	KUILogDebugUO( "Text saved!" );
}


void UKUITextFieldWidget::SetValueChangeDelegate( UObject* oObject, FKUIInterfaceWidgetValueChangePrototype fnTextUpdatedCallback )
{
	if ( oObject == NULL || fnTextUpdatedCallback == NULL )
		dgValueChanged.Unbind();

	else
		dgValueChanged.BindUObject( oObject, fnTextUpdatedCallback );
}


void UKUITextFieldWidget::AddString( const FString& strString )
{
	if ( iMaxLength != KUI_TEXT_FIELD_NO_MAX_LENGTH )
	{
		if ( strFullString.Len() == iMaxLength )
			return;

		if ( ( strFullString.Len() + strString.Len() ) > iMaxLength )
		{
			AddString( strString.Left( iMaxLength - strFullString.Len() ) );
			return;
		}
	}

	SetString( strFullString.Left( iCaretPosition ) + strString + strFullString.Right( strFullString.Len() - iCaretPosition ) );
	strLastAdd = strString;
	eLastAction = EKUITextFieldWidgetLastAction::LA_Add;

	SetCaretPosition( iCaretPosition + strString.Len() );
}


void UKUITextFieldWidget::RemoveChars( uint16 iIndex, uint16 iCount )
{
	if ( iIndex < 0 || iIndex >= strFullString.Len() )
	{
		KUIErrorUO( "Invalid index: %d", iIndex );
		return;
	}

	//KUILogUO( "Remove Char: %d: %s -> %s", iIndex, *strFullString.Left( iIndex ), *strFullString.Right( strFullString.Len() - iIndex - 1 ) );
	SetString( strFullString.Left( iIndex ) + strFullString.Right( strFullString.Len() - iIndex - iCount ) );
}


void UKUITextFieldWidget::OnWidgetStateChange_Implementation()
{
	Super::OnWidgetStateChange_Implementation();

	//KUILogUO( "OnWidgetStateChange_Implementation() %s", *strFullString );

	if ( iCaretPosition < 0 )
		iCaretPosition = 0;

	else if ( iCaretPosition > strFullString.Len() )
		iCaretPosition = strFullString.Len();

	//KUILogUO( "Caret Position: %d, %d, %d", iCaretPosition, iRenderStringStart, iRenderStringLength );

	// Move the rendered text so the caret is on the far left.
	if ( iCaretPosition < iRenderStringStart )
	{
		//KUILogUO( "iCaretPosition < iRenderStringStart" );

		iRenderStringStart = iCaretPosition;
		iRenderStringLength = 0;
		const FVector2D v2Size = GetSize();

		while ( true )
		{
			++iRenderStringLength;
			//KUILogUO( "iRenderStringLength: %d", iRenderStringLength );

			if ( ( iRenderStringStart + iRenderStringLength ) > strFullString.Len() )
				break;

			//KUILogUO( "Text Size: %f", cmTextComponent->GetSize().X );

			if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X > v2Size.X )
				break;
		}

		--iRenderStringLength;
		cmTextComponent->SetTextString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) );

		//KUILogUO( "Done: %d, %d, %s", iRenderStringStart, iRenderStringLength, *cmTextComponent->GetText().ToString() );
	}

	// Move the rendered text so the caret is on the far right.
	else if ( iCaretPosition > ( iRenderStringStart + iRenderStringLength ) )
	{
		//KUILogUO( "iCaretPosition > ( iRenderStringStart + iRenderStringLength )" );
		iRenderStringStart = iCaretPosition;
		iRenderStringLength = 0;
		const FVector2D v2Size = GetSize();

		while ( true )
		{
			if ( iRenderStringStart > 0 )
			{
				--iRenderStringStart;
				++iRenderStringLength;

				//KUILogUO( "iRenderStringStart: %d, iRenderStringLength: %d", iRenderStringStart, iRenderStringLength );
				//KUILogUO( "Text Size: %f", cmTextComponent->GetSize().X );

				if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X > v2Size.X )
				{
					++iRenderStringStart;
					--iRenderStringLength;
					break;
				}
			}

			else
			{
				++iRenderStringLength;
				//KUILogUO( "iRenderStringLength: %d", iRenderStringLength );

				if ( ( iRenderStringStart + iRenderStringLength ) > strFullString.Len() )
				{
					--iRenderStringLength;
					break;
				}

				//KUILogUO( "Text Size: %f", cmTextComponent->GetSize().X );

				if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X > v2Size.X )
				{
					--iRenderStringLength;
					break;
				}
			}
		}

		cmTextComponent->SetTextString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) );
		//KUILogUO( "Done: %d, %d, %s", iRenderStringStart, iRenderStringLength, *cmTextComponent->GetText().ToString() );
	}

	// Re-set the rendered text so that it does not overlap the bounds.
	else
	{
		//KUILogUO( "else" ) );
		iRenderStringLength = 0;
		const FVector2D v2Size = GetSize();

		while ( true )
		{
			if ( ( iRenderStringStart + iRenderStringLength ) < strFullString.Len() )
			{
				++iRenderStringLength;

				//KUILogUO( "iRenderStringLength: %d", iRenderStringLength );
				//float fTextWidth = cmTextComponent->GetSize( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X;
				//KUILogUO( "Text Size: %f %f", fTextWidth, v2Size.X );

				if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X > v2Size.X )
				{
					--iRenderStringLength;
					break;
				}
			}

			else
			{
				if ( iRenderStringStart == 0 )
					break;

				--iRenderStringStart;
				++iRenderStringLength;

				//KUILogUO( "iRenderStringStart: %d, iRenderStringLength: %d", iRenderStringStart, iRenderStringLength );
				//KUILogUO( LogTemp, Log, TEXT( "Text Size: %f", cmTextComponent->GetSize().X );

				if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) ).X > v2Size.X )
				{
					++iRenderStringStart;
					--iRenderStringLength;
					break;
				}
			}
		}

		cmTextComponent->SetTextString( strFullString.Mid( iRenderStringStart, iRenderStringLength ) );
		//KUILogUO( "Done: %d, %d, %s", iRenderStringStart, iRenderStringLength, *cmTextComponent->GetText().ToString() );
	}

	// Find where to render our caret.
	const uint16 iCaretRenderOffset = iCaretPosition - iRenderStringStart;

	//KUILogUO( "iCaretPosition: %d, iCaretRenderOffset: %d", iCaretPosition, iCaretRenderOffset );

	if ( iCaretRenderOffset == 0 )
		cmCaret->SetLocation( v2CaretOffset.X + cmTextComponent->GetMargin().X, v2CaretOffset.Y + cmTextComponent->GetMargin().Y );

	else
		cmCaret->SetLocation( v2CaretOffset.X + cmTextComponent->GetMargin().X + cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iCaretRenderOffset ) ).X, v2CaretOffset.Y + cmTextComponent->GetMargin().Y );

	//KUILogUO( "-----------" );
	//KUILogUO( "--" );
}


void UKUITextFieldWidget::OnTextBoxClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset )
{
	if ( GetInterface() == NULL )
	{
		KUIErrorUO( "Null Interface" );
		return;
	}

	if ( IsDisabledRecursive() )
		return;

	//KUILogUO( "Focusing" ) );
	GetInterface()->SetFocus( this );

	float fOffset = 0.f;
	uint16 iChars = 0;

	while ( true )
	{
		++iChars;

		if ( iChars > strFullString.Len() )
			break;

		if ( cmTextComponent->GetSizeString( strFullString.Mid( iRenderStringStart, iChars ) ).X > v2ClickOffset.X )
			break;
	}

	iCaretPosition = iRenderStringStart + iChars - 1;

	KUISendEvent( FKUIInterfaceEvent, EKUIInterfaceWidgetEventList::E_StateChange );
}


bool UKUITextFieldWidget::IsInputEventConsumer() const
{
	return true;
}


bool UKUITextFieldWidget::RespondsToEvent( uint8 iEventID ) const
{
	if ( Super::RespondsToEvent( iEventID ) )
		return true;

	return ( iEventID == EKUIInterfaceWidgetEventList::E_InlineValueChange || iEventID == EKUIInterfaceWidgetEventList::E_ValueChange );
}


void UKUITextFieldWidget::SendEvent( FKUIInterfaceEvent& stEventInfo )
{
	if ( IsTemplate() )
		return;

	Super::SendEvent( stEventInfo );

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_InlineValueChange )
	{
		// Can't use UStruct pointers... so this.
#pragma warning( disable : 4946 )

		FKUIInterfaceWidgetInlineValueChangeEvent& stInlineValueChangeInfo = *reinterpret_cast<FKUIInterfaceWidgetInlineValueChangeEvent*>( &stEventInfo );

		dgInlineValueChange.ExecuteIfBound( this, reinterpret_cast<const void*>( stInlineValueChangeInfo.oOldValue ), reinterpret_cast<const void*>( stInlineValueChangeInfo.oNewValue ) );
		OnInlineValueChangeBP( *reinterpret_cast<const FString*>( stInlineValueChangeInfo.oOldValue ), *reinterpret_cast<const FString*>( stInlineValueChangeInfo.oNewValue ) );

#pragma warning( default : 4946 )

	}

	if ( stEventInfo.iEventID == EKUIInterfaceWidgetEventList::E_ValueChange )
	{
		dgValueChanged.ExecuteIfBound( this );
		OnValueChangeBP();
	}
}


bool UKUITextFieldWidget::IsPunctuation( TCHAR chChar )
{
	switch ( chChar )
	{
		case TCHAR( '(' ):
		case TCHAR( '!' ):
		case TCHAR( '"' ):
		case TCHAR( '`' ):
		case TCHAR( '-' ):
		case TCHAR( ':' ):
		case TCHAR( ';' ):
		case TCHAR( '@' ):
		case TCHAR( '\'' ):
		case TCHAR( ',' ):
		case TCHAR( '.' ):
		case TCHAR( '<' ):
		case TCHAR( '>' ):
		case TCHAR( '?' ):
		case TCHAR( '/' ):
		case TCHAR( ')' ):
			return true;
	}

	return false;
}
