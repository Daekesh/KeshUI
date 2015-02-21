// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceComponent.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUIBorderInterfaceComponent.h"


UKUIBorderInterfaceComponent::UKUIBorderInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	arTextures.SetNum( EBCBorderTexture::TI_Max );
	arTexturePtrs.SetNum( EBCBorderTexture::TI_Max );
	arItems.SetNum( EBCBorderTexture::TI_Max );

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
	{
		arTextures[ i ] = NULL;
		arTexturePtrs[ i ] = NULL;
		arItems[ i ].Reset();
	}

	v2Size = FVector2D::ZeroVector;
	rRotation = FRotator::ZeroRotator;
	v2PivotPoint = FVector2D::ZeroVector;
	v4BorderMetrics.X = KUI_BORDER_NOT_SET;
	v4BorderMetrics.Y = KUI_BORDER_NOT_SET;
	v4BorderMetrics.Z = KUI_BORDER_NOT_SET;
	v4BorderMetrics.W = KUI_BORDER_NOT_SET;
}


UTexture2D* UKUIBorderInterfaceComponent::GetTexture( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex ) const
{
	return arTextures[ eIndex ];
}


void UKUIBorderInterfaceComponent::SetTexture( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, UTexture2D* tTexture )
{
	if ( arTextures[ eIndex ] == tTexture )
		return;

	arTextures[ eIndex ] = tTexture;

	if ( tTexture != NULL )
	{
		switch ( eIndex )
		{
			case EBCBorderTexture::TI_TopLeft:
			case EBCBorderTexture::TI_Left:
			case EBCBorderTexture::TI_BottomLeft:
				if ( v4BorderMetrics.X == KUI_BORDER_NOT_SET )
					v4BorderMetrics.X = tTexture->GetSurfaceWidth();

				break;

			case EBCBorderTexture::TI_TopRight:
			case EBCBorderTexture::TI_Right:
			case EBCBorderTexture::TI_BottomRight:
				if ( v4BorderMetrics.Z == KUI_BORDER_NOT_SET )
					v4BorderMetrics.Z = tTexture->GetSurfaceWidth();

				break;
		}

		switch ( eIndex )
		{
			case EBCBorderTexture::TI_TopLeft:
			case EBCBorderTexture::TI_Top:
			case EBCBorderTexture::TI_TopRight:
				if ( v4BorderMetrics.Y == KUI_BORDER_NOT_SET )
					v4BorderMetrics.Y = tTexture->GetSurfaceHeight();

				break;

			case EBCBorderTexture::TI_BottomLeft:
			case EBCBorderTexture::TI_Bottom:
			case EBCBorderTexture::TI_BottomRight:
				if ( v4BorderMetrics.W == KUI_BORDER_NOT_SET )
					v4BorderMetrics.W = tTexture->GetSurfaceHeight();

				break;
		}
	}

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetTexturePointer( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, UTexture2D** tTexturePtr )
{
	if ( arTexturePtrs[ eIndex ] == tTexturePtr )
		return;

	arTexturePtrs[ eIndex ] = tTexturePtr;

	SetTexture( eIndex, tTexturePtr != NULL ? *tTexturePtr : NULL );
}


void UKUIBorderInterfaceComponent::SetTextureName( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, const FName& nTextureName )
{
	SetTexturePointer( eIndex, GetAsset<UTexture2D>( nTextureName ) );
}


FVector2D UKUIBorderInterfaceComponent::GetSize() const
{
	return v2Size;
}


void UKUIBorderInterfaceComponent::SetSizeStruct( const FVector2D& v2Size )
{
	SetSize( v2Size.X, v2Size.Y );
}


void UKUIBorderInterfaceComponent::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	v2Size.X = fWidth;
	v2Size.Y = fHeight;

	Super::SetSize( fWidth, fHeight );
	
	Invalidate();
}


FRotator UKUIBorderInterfaceComponent::GetRotation() const
{
	return rRotation;
}


void UKUIBorderInterfaceComponent::SetRotationStruct( const FRotator& rRotation )
{
	SetRotation( rRotation.Pitch, rRotation.Yaw, rRotation.Yaw );
}


void UKUIBorderInterfaceComponent::SetRotation( float fPitch, float fYaw, float fRoll )
{
	if ( rRotation.Pitch == fPitch && rRotation.Yaw == fYaw && rRotation.Roll == fRoll )
		return;

	rRotation.Pitch = fPitch;
	rRotation.Yaw = fYaw;
	rRotation.Roll = fRoll;

	InvalidateRenderCache();
}


FVector2D UKUIBorderInterfaceComponent::GetPivotPoint() const
{
	return v2PivotPoint;
}


void UKUIBorderInterfaceComponent::SetPivotPointStruct( const FVector2D& v2PivotPoint )
{
	SetPivotPoint( v2PivotPoint.X, v2PivotPoint.Y );
}


void UKUIBorderInterfaceComponent::SetPivotPoint( float fX, float fY )
{
	if ( v2PivotPoint.X == fX && v2PivotPoint.Y == fY )
		return;

	v2PivotPoint.X = fX;
	v2PivotPoint.Y = fY;

	InvalidateRenderCache();
}


FVector4 UKUIBorderInterfaceComponent::GetBorderMetrics() const
{
	return v4BorderMetrics;
}


void UKUIBorderInterfaceComponent::SetBorderMetrics( float fLeft, float fTop, float fRight, float fBottom )
{
	if ( fLeft != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.X = fLeft;

	if ( fRight != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Z = fRight;

	if ( fTop != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Y = fTop;

	if ( fBottom != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.W = fBottom;

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetBorderMetricsStruct( const FVector4& v4Metrics )
{
	SetBorderMetrics( v4Metrics.X, v4Metrics.Y, v4Metrics.Z, v4Metrics.W );
}


bool UKUIBorderInterfaceComponent::HasValidComponents() const
{
	if ( v2Size.X <= 0 || v2Size.Y <= 0 )
		return false;

	if ( arTextures[ EBCBorderTexture::TI_Centre ] == NULL )
		return false;

	return true;
}


void UKUIBorderInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
		arItems[ i ].Reset();

	FVector4 v4BorderMetricsActual;
	v4BorderMetricsActual.X = ( v4BorderMetrics.X != KUI_BORDER_NOT_SET ? v4BorderMetrics.X : 0.f );
	v4BorderMetricsActual.Y = ( v4BorderMetrics.Y != KUI_BORDER_NOT_SET ? v4BorderMetrics.Y : 0.f );
	v4BorderMetricsActual.Z = ( v4BorderMetrics.Z != KUI_BORDER_NOT_SET ? v4BorderMetrics.Z : 0.f );
	v4BorderMetricsActual.W = ( v4BorderMetrics.W != KUI_BORDER_NOT_SET ? v4BorderMetrics.W : 0.f );

	if ( ( v4BorderMetricsActual.X + v4BorderMetricsActual.Y ) > v2Size.X )
	{
		float fOldWidth = v4BorderMetricsActual.X + v4BorderMetricsActual.Y;
		v4BorderMetricsActual.X *= v2Size.X / fOldWidth;
		v4BorderMetricsActual.Y *= v2Size.X / fOldWidth;
	}

	if ( ( v4BorderMetricsActual.Z + v4BorderMetricsActual.W ) > v2Size.Y )
	{
		float fOldHeight = v4BorderMetricsActual.Z + v4BorderMetricsActual.W;
		v4BorderMetricsActual.W *= v2Size.Y / fOldHeight;
		v4BorderMetricsActual.Z *= v2Size.Y / fOldHeight;
	}

	const FVector2D v2Size = GetSize();
	FVector2D v2CentreSize;
	v2CentreSize.X = v2Size.X - v4BorderMetricsActual.X - v4BorderMetricsActual.Y;
	v2CentreSize.Y = v2Size.Y - v4BorderMetricsActual.Z - v4BorderMetricsActual.W;

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
	{
		if ( arTextures[ i ] == NULL )
			continue;

		FVector2D v2ItemPosition = FVector2D::ZeroVector;
		FVector2D v2ItemSize = FVector2D::ZeroVector;

		switch ( i )
		{
			case EBCBorderTexture::TI_TopLeft:
				v2ItemPosition.X = 0.f;
				v2ItemPosition.Y = 0.f;
				v2ItemSize.X = v4BorderMetricsActual.X;
				v2ItemSize.Y = v4BorderMetricsActual.Y;
				break;

			case EBCBorderTexture::TI_Left:
				v2ItemPosition.X = 0.f;
				v2ItemPosition.Y = v4BorderMetricsActual.Y;
				v2ItemSize.X = v4BorderMetricsActual.X;
				v2ItemSize.Y = v2CentreSize.Y;
				break;

			case EBCBorderTexture::TI_BottomLeft:
				v2ItemPosition.X = 0.f;
				v2ItemPosition.Y = v4BorderMetricsActual.Y + v2CentreSize.Y;
				v2ItemSize.X = v4BorderMetricsActual.X;
				v2ItemSize.Y = v4BorderMetricsActual.W;
				break;

			case EBCBorderTexture::TI_Top:
				v2ItemPosition.X = v4BorderMetricsActual.X;
				v2ItemPosition.Y = 0.f;
				v2ItemSize.X = v2CentreSize.X;
				v2ItemSize.Y = v4BorderMetricsActual.Y;
				break;

			case EBCBorderTexture::TI_Centre:
				v2ItemPosition.X = v4BorderMetricsActual.X;
				v2ItemPosition.Y = v4BorderMetricsActual.Y;
				v2ItemSize = v2CentreSize;
				break;

			case EBCBorderTexture::TI_Bottom:
				v2ItemPosition.X = v4BorderMetricsActual.X;
				v2ItemPosition.Y = v4BorderMetricsActual.Y + v2CentreSize.Y;
				v2ItemSize.X = v2CentreSize.X;
				v2ItemSize.Y = v4BorderMetricsActual.W;
				break;

			case EBCBorderTexture::TI_TopRight:
				v2ItemPosition.X = v4BorderMetricsActual.X + v2CentreSize.X;
				v2ItemPosition.Y = 0.f;
				v2ItemSize.X = v4BorderMetricsActual.Z;
				v2ItemSize.Y = v4BorderMetricsActual.Y;
				break;

			case EBCBorderTexture::TI_Right:
				v2ItemPosition.X = v4BorderMetricsActual.X + v2CentreSize.X;
				v2ItemPosition.Y = v4BorderMetricsActual.Y;
				v2ItemSize.X = v4BorderMetricsActual.Z;
				v2ItemSize.Y = v2CentreSize.Y;
				break;

			case EBCBorderTexture::TI_BottomRight:
				v2ItemPosition.X = v4BorderMetricsActual.X + v2CentreSize.X;
				v2ItemPosition.Y = v4BorderMetricsActual.Y + v2CentreSize.Y;
				v2ItemSize.X = v4BorderMetricsActual.Z;
				v2ItemSize.Y = v4BorderMetricsActual.W;
				break;
		}

		arItems[ i ] = TSharedPtr<FCanvasTileItem>( new FCanvasTileItem(
			v2ItemPosition,
			arTextures[ i ]->Resource,
			v2ItemSize,
			FVector2D::ZeroVector,
			v2ItemSize / FVector2D( arTextures[ i ]->GetSurfaceWidth(), arTextures[ i ]->GetSurfaceHeight() ),
			GetDrawColor().ReinterpretAsLinear()
		) );
	}

	Validate();
}


void UKUIBorderInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	Super::Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );

	if ( IsRenderCaching() && oRenderCacheObject != this )
		return;

	if ( !HasValidComponents() )
		return;

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
		if ( arTexturePtrs[ i ] != NULL && *arTexturePtrs[ i ] != arTextures[ i ] )
			SetTexture( static_cast<EBCBorderTexture::TextureIndex>( i ), *arTexturePtrs[ i ] );

	if ( !arItems[ EBCBorderTexture::TI_Centre ].IsValid() )
		ConstructNewItem();

	FVector2D v2RenderLocation = ( ( IsRenderCaching() || !IsPositionable() ) ? FVector2D::ZeroVector : v2Origin + GetRenderLocation() );

	v2RenderLocation.X = bRoundPosition ? FMath::RoundToInt( v2RenderLocation.X ) : v2RenderLocation.X;
	v2RenderLocation.Y = bRoundPosition ? FMath::RoundToInt( v2RenderLocation.Y ) : v2RenderLocation.Y;

	//oCanvas->SetClip( v2RenderLocation.X + v2Size.X, v2RenderLocation.Y + v2Size.Y );

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
	{
		if ( arItems[ i ].IsValid() )
		{
			arItems[ i ]->Position += v2RenderLocation;

			if ( oRenderCacheObject != NULL && eBlendMode == SE_BLEND_Translucent )
				arItems[ i ]->BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaComposite;

			else
				arItems[ i ]->BlendMode = eBlendMode;
		}
	}

	if ( arItems[ EBCBorderTexture::TI_Centre ].IsValid() )      oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_Centre ] );
	if ( arItems[ EBCBorderTexture::TI_Left ].IsValid() )        oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_Left ] );
	if ( arItems[ EBCBorderTexture::TI_Right ].IsValid() )       oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_Right ] );
	if ( arItems[ EBCBorderTexture::TI_Top ].IsValid() )         oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_Top ] );
	if ( arItems[ EBCBorderTexture::TI_Bottom ].IsValid() )      oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_Bottom ] );
	if ( arItems[ EBCBorderTexture::TI_TopLeft ].IsValid() )     oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_TopLeft ] );
	if ( arItems[ EBCBorderTexture::TI_TopRight ].IsValid() )    oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_TopRight ] );
	if ( arItems[ EBCBorderTexture::TI_BottomLeft ].IsValid() )  oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_BottomLeft ] );
	if ( arItems[ EBCBorderTexture::TI_BottomRight ].IsValid() ) oCanvas->DrawItem( *arItems[ EBCBorderTexture::TI_BottomRight ] );

	for ( uint8 i = EBCBorderTexture::TI_Centre; i < EBCBorderTexture::TI_Max; ++i )
		if ( arItems[ i ].IsValid() )
			arItems[ i ]->Position -= v2RenderLocation;
}
