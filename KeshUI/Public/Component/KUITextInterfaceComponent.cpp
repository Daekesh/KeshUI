// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUITextInterfaceComponent.h"


UKUITextInterfaceComponent::UKUITextInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	txText = FText::GetEmpty();
	foFont = NULL;
	foFontPtr = NULL;
	bCentreX = false;
	bCentreY = false;
	v2Scale = FVector2D( 1.f, 1.f );
	fDepth = 1.f;
	bOutlined = false;
	lcOutlineColor = FLinearColor::Black;
	bShadow = false;
	bUpdateShadowInfo = false;
	v2ShadowOffset = FVector2D::ZeroVector;
	lcShadowColor = FLinearColor::Black;
	bClipped = false;
	fHorizontalSpacingAdjust = 0.f;
	bDontCorrectStereoscopic = true;
	v2Size = FVector2D::ZeroVector;
}


const FText& UKUITextInterfaceComponent::GetText() const
{ 
	return txText;
}


const FString& UKUITextInterfaceComponent::GetTextString() const
{
	return txText.ToString();
}


void UKUITextInterfaceComponent::SetText( const FText& txText )
{ 
	if ( this->txText.EqualTo( txText ) )
		return;

	this->txText = txText;
	v2Size = GetSizeText( txText );

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	InvalidateRenderCache();
	InvalidateAlignLocation();
}


void UKUITextInterfaceComponent::SetTextString( const FString& sString )
{ 
	SetText( FText::FromString( sString ) );
}


UFont* UKUITextInterfaceComponent::GetFont() const
{ 
	return foFont;
}


void UKUITextInterfaceComponent::SetFont( UFont* foFont )
{
	if ( this->foFont == foFont )
		return;

	this->foFont = foFont;
	v2Size = GetSizeText( txText );

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	Invalidate();
	InvalidateAlignLocation();
}


void UKUITextInterfaceComponent::SetFontPointer( UFont** foFontPtr )
{
	if ( this->foFontPtr == foFontPtr )
		return;

	this->foFontPtr = foFontPtr;

	SetFont( *foFontPtr );
}


void UKUITextInterfaceComponent::SetFontName( const FName& nFontName )
{
	SetFontPointer( GetAsset<UFont>( nFontName ) );
}


bool UKUITextInterfaceComponent::IsCenteredHorizontally() const
{
	return bCentreX;
}


void UKUITextInterfaceComponent::SetCentredHorizontally( bool bCentred )
{
	if ( bCentreX == bCentred )
		return;

	bCentreX = bCentred;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stContainerEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stContainerEventInfo );
	}

	InvalidateRenderCache();
}


bool UKUITextInterfaceComponent::IsCenteredVertically() const
{
	return bCentreY;
}


void UKUITextInterfaceComponent::SetCentredVertically( bool bCentred )
{
	if ( bCentreY == bCentred )
		return;

	bCentreY = bCentred;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stContainerEventInfo( EKUIInterfaceContainerEventList::E_ChildLocationChange, this );
		GetContainer()->SendEvent( stContainerEventInfo );
	}

	InvalidateRenderCache();
}


const FVector2D& UKUITextInterfaceComponent::GetScale() const
{ 
	return v2Scale;
}


void UKUITextInterfaceComponent::SetScaleStruct( const FVector2D& v2Scale )
{ 
	SetScale( v2Scale.X, v2Scale.Y );
}


void UKUITextInterfaceComponent::SetScale( float fX, float fY )
{
	if ( v2Scale.X == fX && v2Scale.Y == fY )
		return;

	v2Scale.X = fX;
	v2Scale.Y = fY;
	v2Size = GetSizeText( txText );

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	InvalidateRenderCache();
}


float UKUITextInterfaceComponent::GetDepth() const
{ 
	return fDepth;
}


void UKUITextInterfaceComponent::SetDepth( float fDepth )
{ 
	if ( this->fDepth == fDepth )
		return;

	this->fDepth = fDepth;
	v2Size = GetSizeText( txText );

	InvalidateRenderCache();
}


bool UKUITextInterfaceComponent::IsOutlined() const
{
	return bOutlined;
}


void UKUITextInterfaceComponent::SetOutlined( bool bOutlined )
{
	if ( this->bOutlined == bOutlined )
		return;

	this->bOutlined = bOutlined;
	v2Size = GetSizeText( txText );

	InvalidateRenderCache();
}


const FLinearColor& UKUITextInterfaceComponent::GetOutlineColor() const
{
	return lcOutlineColor;
}


void UKUITextInterfaceComponent::SetOutlineColor( const FLinearColor& lcColor )
{
	if ( lcOutlineColor.R == lcColor.R && lcOutlineColor.G == lcColor.G && lcOutlineColor.B == lcColor.B && lcOutlineColor.A == lcColor.A )
		return;

	lcOutlineColor = lcColor;

	InvalidateRenderCache();
}


const FVector2D& UKUITextInterfaceComponent::GetShadowOffset() const
{
	return v2ShadowOffset;
}


void UKUITextInterfaceComponent::SetShadowOffsetStruct( const FVector2D& v2Offset )
{
	SetShadowOffset( v2Offset.X, v2Offset.Y );
}


void UKUITextInterfaceComponent::SetShadowOffset( float fX, float fY )
{
	if ( v2ShadowOffset.X == fX || v2ShadowOffset.Y == fY )
		return;

	v2ShadowOffset.X = fX;
	v2ShadowOffset.Y = fY;
	v2Size = GetSizeText( txText );

	bUpdateShadowInfo = true;
	InvalidateRenderCache();
}


bool UKUITextInterfaceComponent::HasShadow() const
{
	return bShadow;
}


void UKUITextInterfaceComponent::SetShadow( bool bShadow )
{
	if ( this->bShadow == bShadow )
		return;

	this->bShadow = bShadow;
	v2Size = GetSizeText( txText );

	bUpdateShadowInfo = true;
	InvalidateRenderCache();
}


const FLinearColor& UKUITextInterfaceComponent::GetShadowColor() const
{
	return lcShadowColor;
}


void UKUITextInterfaceComponent::SetShadowColor( const FLinearColor& lcColor )
{
	if ( lcShadowColor.R == lcColor.R && lcShadowColor.G == lcColor.G && lcShadowColor.B == lcColor.B && lcShadowColor.A == lcColor.A )
		return;

	lcShadowColor = lcColor;

	InvalidateRenderCache();
}


bool UKUITextInterfaceComponent::IsClipped() const
{
	return bClipped;
}


void UKUITextInterfaceComponent::SetClipped( bool bClipped )
{
	if ( this->bClipped == bClipped )
		return;

	this->bClipped = bClipped;

	InvalidateRenderCache();
}


float UKUITextInterfaceComponent::GetHorizontalSpacingAdjustment() const
{
	return fHorizontalSpacingAdjust;
}


void UKUITextInterfaceComponent::SetHorizontalSpacingAdjustment( float fSpacing )
{
	if ( fHorizontalSpacingAdjust == fSpacing )
		return;

	fHorizontalSpacingAdjust = fSpacing;
	v2Size = GetSizeText( txText );

	InvalidateRenderCache();
}


bool UKUITextInterfaceComponent::IsCorrectingStereoscopic() const
{
	return !bDontCorrectStereoscopic;
}


void UKUITextInterfaceComponent::SetCorrectStereoscopic( bool bCorrect )
{
	if ( bDontCorrectStereoscopic != bCorrect )
		return;

	bDontCorrectStereoscopic = !bCorrect;

	InvalidateRenderCache();
}


FFontRenderInfo& UKUITextInterfaceComponent::GetFontRenderInfo()
{
	if ( stItem.IsValid() )
		return static_cast<FCanvasTextItem*>(&*stItem)->FontRenderInfo;

	static FFontRenderInfo stInfo;

	return stInfo;
}


const FVector2D& UKUITextInterfaceComponent::GetSize() const
{
	return v2Size;
}


const FVector2D UKUITextInterfaceComponent::GetSizeText( const FText& txText ) const
{
	return GetSizeString( txText.ToString() );
}


const FVector2D UKUITextInterfaceComponent::GetSizeString( const FString& strString ) const
{
	if ( foFont == NULL )
		return FVector2D::ZeroVector;

	float fX, fY;
	FVector2D v2Size = FVector2D::ZeroVector;

	for ( uint16 i = 0; i < strString.Len(); ++i )
	{
		foFont->GetCharSize( strString[ i ], fX, fY );
		v2Size.X += fX;
		v2Size.Y = max( v2Size.Y, fY );
	}

	v2Size.X += ( ( strString.Len() > 0 ? strString.Len() - 1 : 0 ) * fHorizontalSpacingAdjust ) + max( ( bShadow ? v2ShadowOffset.X : 0.f ), ( bOutlined ? 2.f : 0.f ) );
	v2Size.Y += max( ( bShadow ? v2ShadowOffset.Y : 0.f ), ( bOutlined ? 2.f : 0.f ) );

	return v2Size * v2Scale;
}


bool UKUITextInterfaceComponent::HasValidComponents() const
{
	if ( txText.IsEmpty() )
		return false;

	if ( foFont == NULL )
		return false;

	return true;
}


void UKUITextInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasTextItem>( new FCanvasTextItem(
		FVector2D::ZeroVector,
		txText,
		foFont,
		GetDrawColor().ReinterpretAsLinear()
	) );

	if ( this->bShadow )
		static_cast<FCanvasTextItem*>(&*stItem)->EnableShadow( lcShadowColor, v2ShadowOffset );

	else
		static_cast<FCanvasTextItem*>(&*stItem)->DisableShadow();

	bUpdateShadowInfo = false;

	static_cast<FCanvasTextItem*>(&*stItem)->bCentreX = bCentreX;
	static_cast<FCanvasTextItem*>(&*stItem)->bCentreY = bCentreY;
	static_cast<FCanvasTextItem*>(&*stItem)->bOutlined = bOutlined;
	static_cast<FCanvasTextItem*>(&*stItem)->OutlineColor = lcOutlineColor;
	static_cast<FCanvasTextItem*>(&*stItem)->Scale = v2Scale;
	static_cast<FCanvasTextItem*>(&*stItem)->Depth = fDepth;
	static_cast<FCanvasTextItem*>(&*stItem)->HorizSpacingAdjust = fHorizontalSpacingAdjust;
	static_cast<FCanvasTextItem*>(&*stItem)->bDontCorrectStereoscopic = bDontCorrectStereoscopic;
	static_cast<FCanvasTextItem*>(&*stItem)->FontRenderInfo.bClipText = bClipped;
	static_cast<FCanvasTextItem*>(&*stItem)->Text = txText;

	v2Size = GetSizeText( txText );

	Super::ConstructNewItem();
}


void UKUITextInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( foFontPtr != NULL && *foFontPtr != foFont )
		SetFont( *foFontPtr );

	if ( stItem.IsValid() )
	{
		if ( bUpdateShadowInfo )
		{
			if ( this->bShadow )
				static_cast<FCanvasTextItem*>(&*stItem)->EnableShadow( lcShadowColor, v2ShadowOffset );

			else
				static_cast<FCanvasTextItem*>(&*stItem)->DisableShadow();

			bUpdateShadowInfo = false;
		}

		static_cast<FCanvasTextItem*>(&*stItem)->bCentreX = bCentreX;
		static_cast<FCanvasTextItem*>(&*stItem)->bCentreY = bCentreY;
		static_cast<FCanvasTextItem*>(&*stItem)->bOutlined = bOutlined;
		static_cast<FCanvasTextItem*>(&*stItem)->OutlineColor = lcOutlineColor;
		static_cast<FCanvasTextItem*>(&*stItem)->Scale = v2Scale;
		static_cast<FCanvasTextItem*>(&*stItem)->Depth = fDepth;
		static_cast<FCanvasTextItem*>(&*stItem)->HorizSpacingAdjust = fHorizontalSpacingAdjust;
		static_cast<FCanvasTextItem*>(&*stItem)->bDontCorrectStereoscopic = bDontCorrectStereoscopic;
		static_cast<FCanvasTextItem*>(&*stItem)->FontRenderInfo.bClipText = bClipped;
		static_cast<FCanvasTextItem*>(&*stItem)->Text = txText;
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}


TArray<FString> UKUITextInterfaceComponent::SplitString( const FString& strString, UFont* foFont, float fMaxWidth, const FString& strIndentString )
{
	uint16 iCharCount = 0;
	TArray<FString> arBrokenUpMessage;
	const float fIndentWidth = ( strIndentString.Len() > 0 ? foFont->GetStringSize( *strIndentString ) : 0.f );
	FString strIter = strString;

	while ( strIter.Len() > 0 )
	{
		++iCharCount;

		float fWidth = foFont->GetStringSize( *strIter.Mid( 0, iCharCount ) );

		if ( arBrokenUpMessage.Num() > 0 )
			fWidth += fIndentWidth;

		if ( fWidth < fMaxWidth && iCharCount < strIter.Len() )
			continue;

		if ( fWidth > fMaxWidth )
		{
			--iCharCount;

			if ( iCharCount == 0 )
			{
				KUIErrorCW( GWorld, "Text area too small for single char" );
				break;
			}
		}

		if ( iCharCount == strIter.Len() )
		{
			arBrokenUpMessage.Add( strIter );
			break;
		}

		bool bFoundBreak = false;

		// Don't backtrack really short lines...
		if ( iCharCount > 2 )
		{
			uint16 iMaxBacktrack = clamp( iCharCount / 2, 1, 20 );

			for ( uint16 i = 0; i < iMaxBacktrack; ++i )
			{
				uint16 iCurrentCount = iCharCount - i;

				switch ( strIter[ iCurrentCount - 1 ] )
				{
					case ' ': // Found a space... don't include it in the rendered string						
						arBrokenUpMessage.Add( strIter.Left( iCurrentCount - 1 ) );
						iCharCount = iCurrentCount;
						bFoundBreak = true;
						break;

					case '.': // do they suck at spaces?
					case ',': // "
					case ':': // "
					case ';': // "
					case '-': // Found a hyphen, include it						
						arBrokenUpMessage.Add( strIter.Left( iCurrentCount ) );
						iCharCount = iCurrentCount;
						bFoundBreak = true;
						break;
				}

				if ( bFoundBreak )
					break;
			}
		}

		if ( !bFoundBreak )
		{
			if ( iCharCount > 1 )
			{
				--iCharCount;
				arBrokenUpMessage.Add( strIter.Left( iCharCount ) + "-" );
			}

			else
				arBrokenUpMessage.Add( strIter.Left( iCharCount ) );
		}

		if ( strIter.Len() == iCharCount )
			break;

		strIter = strIter.Mid( iCharCount ); // Not the most efficient method, but screw it.
		iCharCount = 0;
	}

	return arBrokenUpMessage;
}
