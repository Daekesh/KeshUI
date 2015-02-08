// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceComponent.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUIBorderInterfaceComponent.h"


UKUIBorderInterfaceComponent::UKUIBorderInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	tTextureBackground = NULL;
	tTextureBorderLeft = NULL;
	tTextureBorderRight = NULL;
	tTextureBorderTop = NULL;
	tTextureBorderBottom = NULL;
	tTextureBorderTopLeft = NULL;
	tTextureBorderTopRight = NULL;
	tTextureBorderBottomLeft = NULL;
	tTextureBorderBottomRight = NULL;
	tTextureBackgroundPtr = NULL;
	tTextureBorderLeftPtr = NULL;
	tTextureBorderRightPtr = NULL;
	tTextureBorderTopPtr = NULL;
	tTextureBorderBottomPtr = NULL;
	tTextureBorderTopLeftPtr = NULL;
	tTextureBorderTopRightPtr = NULL;
	tTextureBorderBottomLeftPtr = NULL;
	tTextureBorderBottomRightPtr = NULL;
	v2Size = FVector2D::ZeroVector;
	rRotation = FRotator::ZeroRotator;
	v2PivotPoint = FVector2D::ZeroVector;
	v4BorderMetrics.X = KUI_BORDER_NOT_SET;
	v4BorderMetrics.Y = KUI_BORDER_NOT_SET;
	v4BorderMetrics.Z = KUI_BORDER_NOT_SET;
	v4BorderMetrics.W = KUI_BORDER_NOT_SET;
}


UTexture2D* UKUIBorderInterfaceComponent::GetBackgroundTexture() const
{
	return tTextureBackground;
}


void UKUIBorderInterfaceComponent::SetBackgroundTexture( UTexture2D* tTexture )
{
	if ( tTextureBackground == tTexture )
		return;

	tTextureBackground = tTexture;

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetBackgroundTexturePointer( UTexture2D** tTextureBackgroundPtr )
{
	if ( this->tTextureBackgroundPtr == tTextureBackgroundPtr )
		return;

	this->tTextureBackgroundPtr = tTextureBackgroundPtr;

	SetBackgroundTexture( *tTextureBackgroundPtr );
}


void UKUIBorderInterfaceComponent::SetBackgroundTextureName( const FName& nTextureName )
{
	SetBackgroundTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetLeftBorderTexture() const
{
	return tTextureBorderLeft;
}


void UKUIBorderInterfaceComponent::SetLeftBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderLeft == tTexture )
		return;

	tTextureBorderLeft = tTexture;

	if ( tTexture != NULL && v4BorderMetrics.X == KUI_BORDER_NOT_SET )
		v4BorderMetrics.X = tTexture->GetSurfaceWidth();

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetLeftBorderTexturePointer( UTexture2D** tTextureBorderLeftPtr )
{
	if ( this->tTextureBorderLeftPtr == tTextureBorderLeftPtr )
		return;

	this->tTextureBorderLeftPtr = tTextureBorderLeftPtr;

	SetLeftBorderTexture( *tTextureBorderLeftPtr );
}


void UKUIBorderInterfaceComponent::SetLeftBorderTextureName( const FName& nTextureName )
{
	SetLeftBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetRightBorderTexture() const
{
	return tTextureBorderRight;
}


void UKUIBorderInterfaceComponent::SetRightBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderRight == tTexture )
		return;

	tTextureBorderRight = tTexture;

	if ( tTexture != NULL && v4BorderMetrics.Y == KUI_BORDER_NOT_SET )
		v4BorderMetrics.Y = tTexture->GetSurfaceWidth();

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetRightBorderTexturePointer( UTexture2D** tTextureBorderRightPtr )
{
	if ( this->tTextureBorderRightPtr == tTextureBorderRightPtr )
		return;

	this->tTextureBorderRightPtr = tTextureBorderRightPtr;

	SetRightBorderTexture( *tTextureBorderRightPtr );
}


void UKUIBorderInterfaceComponent::SetRightBorderTextureName( const FName& nTextureName )
{
	SetRightBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetTopBorderTexture() const
{
	return tTextureBorderTop;
}


void UKUIBorderInterfaceComponent::SetTopBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderTop == tTexture )
		return;

	tTextureBorderTop = tTexture;

	if ( tTexture != NULL && v4BorderMetrics.Z == KUI_BORDER_NOT_SET )
		v4BorderMetrics.Z = tTexture->GetSurfaceHeight();

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetTopBorderTexturePointer( UTexture2D** tTextureBorderTopPtr )
{
	if ( this->tTextureBorderTopPtr == tTextureBorderTopPtr )
		return;

	this->tTextureBorderTopPtr = tTextureBorderTopPtr;

	SetTopBorderTexture( *tTextureBorderTopPtr );
}


void UKUIBorderInterfaceComponent::SetTopBorderTextureName( const FName& nTextureName )
{
	SetTopBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetBottomBorderTexture() const
{
	return tTextureBorderBottom;
}


void UKUIBorderInterfaceComponent::SetBottomBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderBottom == tTexture )
		return;

	tTextureBorderBottom = tTexture;

	if ( tTexture != NULL && v4BorderMetrics.W == KUI_BORDER_NOT_SET )
		v4BorderMetrics.W = tTexture->GetSurfaceHeight();

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetBottomBorderTexturePointer( UTexture2D** tTextureBorderBottomPtr )
{
	if ( this->tTextureBorderBottomPtr == tTextureBorderBottomPtr )
		return;

	this->tTextureBorderBottomPtr = tTextureBorderBottomPtr;

	SetBottomBorderTexture( *tTextureBorderBottomPtr );
}


void UKUIBorderInterfaceComponent::SetBottomBorderTextureName( const FName& nTextureName )
{
	SetBottomBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetTopLeftBorderTexture() const
{
	return tTextureBorderTopLeft;
}


void UKUIBorderInterfaceComponent::SetTopLeftBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderTopLeft == tTexture )
		return;

	tTextureBorderTopLeft = tTexture;

	if ( tTexture != NULL )
	{
		if ( v4BorderMetrics.X == KUI_BORDER_NOT_SET )
			v4BorderMetrics.X = tTexture->GetSurfaceWidth();

		if ( v4BorderMetrics.Z == KUI_BORDER_NOT_SET )
			v4BorderMetrics.Z = tTexture->GetSurfaceHeight();
	}

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetTopLeftBorderTexturePointer( UTexture2D** tTextureBorderTopLeftPtr )
{
	if ( this->tTextureBorderTopLeftPtr == tTextureBorderTopLeftPtr )
		return;

	this->tTextureBorderTopLeftPtr = tTextureBorderTopLeftPtr;

	SetTopLeftBorderTexture( *tTextureBorderTopLeftPtr );
}


void UKUIBorderInterfaceComponent::SetTopLeftBorderTextureName( const FName& nTextureName )
{
	SetTopLeftBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetTopRightBorderTexture() const
{
	return tTextureBorderTopRight;
}


void UKUIBorderInterfaceComponent::SetTopRightBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderTopRight == tTexture )
		return;

	tTextureBorderTopRight = tTexture;

	if ( tTexture != NULL )
	{
		if ( v4BorderMetrics.Y == KUI_BORDER_NOT_SET )
			v4BorderMetrics.Y = tTexture->GetSurfaceWidth();

		if ( v4BorderMetrics.Z == KUI_BORDER_NOT_SET )
			v4BorderMetrics.Z = tTexture->GetSurfaceHeight();
	}

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetTopRightBorderTexturePointer( UTexture2D** tTextureBorderTopRightPtr )
{
	if ( this->tTextureBorderTopRightPtr == tTextureBorderTopRightPtr )
		return;

	this->tTextureBorderTopRightPtr = tTextureBorderTopRightPtr;

	SetTopRightBorderTexture( *tTextureBorderTopRightPtr );
}


void UKUIBorderInterfaceComponent::SetTopRightBorderTextureName( const FName& nTextureName )
{
	SetTopRightBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetBottomLeftBorderTexture() const
{
	return tTextureBorderBottomLeft;
}


void UKUIBorderInterfaceComponent::SetBottomLeftBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderBottomLeft == tTexture )
		return;

	tTextureBorderBottomLeft = tTexture;

	if ( tTexture != NULL )
	{
		if ( v4BorderMetrics.X == KUI_BORDER_NOT_SET )
			v4BorderMetrics.X = tTexture->GetSurfaceWidth();

		if ( v4BorderMetrics.W == KUI_BORDER_NOT_SET )
			v4BorderMetrics.W = tTexture->GetSurfaceHeight();
	}

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetBottomLeftBorderTexturePointer( UTexture2D** tTextureBorderBottomLeftPtr )
{
	if ( this->tTextureBorderBottomLeftPtr == tTextureBorderBottomLeftPtr )
		return;

	this->tTextureBorderBottomLeftPtr = tTextureBorderBottomLeftPtr;

	SetBottomLeftBorderTexture( *tTextureBorderBottomLeftPtr );
}


void UKUIBorderInterfaceComponent::SetBottomLeftBorderTextureName( const FName& nTextureName )
{
	SetBottomLeftBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
}


UTexture2D* UKUIBorderInterfaceComponent::GetBottomRightBorderTexture() const
{
	return tTextureBorderBottomRight;
}


void UKUIBorderInterfaceComponent::SetBottomRightBorderTexture( UTexture2D* tTexture )
{
	if ( tTextureBorderBottomRight == tTexture )
		return;

	tTextureBorderBottomRight = tTexture;

	if ( tTexture != NULL )
	{
		if ( v4BorderMetrics.Y == KUI_BORDER_NOT_SET )
			v4BorderMetrics.Y = tTexture->GetSurfaceWidth();

		if ( v4BorderMetrics.W == KUI_BORDER_NOT_SET )
			v4BorderMetrics.W = tTexture->GetSurfaceHeight();
	}

	Invalidate();
}


void UKUIBorderInterfaceComponent::SetBottomRightBorderTexturePointer( UTexture2D** tTextureBorderBottomRightPtr )
{
	if ( this->tTextureBorderBottomRightPtr == tTextureBorderBottomRightPtr )
		return;

	this->tTextureBorderBottomRightPtr = tTextureBorderBottomRightPtr;

	SetBottomRightBorderTexture( *tTextureBorderBottomRightPtr );
}


void UKUIBorderInterfaceComponent::SetBottomRightBorderTextureName( const FName& nTextureName )
{
	SetBottomRightBorderTexturePointer( GetAsset<UTexture2D>( nTextureName ) );
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


void UKUIBorderInterfaceComponent::SetBorderMetrics( float fLeft, float fRight, float fTop, float fBottom )
{
	if ( fLeft != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.X = fLeft;

	if ( fRight != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Y = fRight;

	if ( fTop != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Z = fTop;

	if ( fBottom != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.W = fBottom;
}


void UKUIBorderInterfaceComponent::SetBorderMetricsStruct( const FVector4& v4Metrics )
{
	if ( v4Metrics.X != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.X = v4Metrics.X;

	if ( v4Metrics.Y != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Y = v4Metrics.Y;

	if ( v4Metrics.Z != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.Z = v4Metrics.Z;

	if ( v4Metrics.W != KUI_BORDER_NO_CHANGE )
		v4BorderMetrics.W = v4Metrics.W;
}


bool UKUIBorderInterfaceComponent::HasValidComponents() const
{
	if ( v2Size.X <= 0 || v2Size.Y <= 0 )
		return false;

	if ( tTextureBackground == NULL && tTextureBorderLeft == NULL && tTextureBorderRight == NULL &&
		 tTextureBorderTop == NULL && tTextureBorderBottom == NULL && tTextureBorderTopLeft == NULL && 
		 tTextureBorderTopRight == NULL && tTextureBorderBottomLeft == NULL && tTextureBorderBottomRight == NULL )
		return false;

	return true;
}


void UKUIBorderInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();
}


void UKUIBorderInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	Super::Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );

	if ( IsRenderCaching() && oRenderCacheObject != this )
		return;

	if ( tTextureBackgroundPtr != NULL && *tTextureBackgroundPtr != tTextureBackground )
		SetBackgroundTexture( *tTextureBackgroundPtr );

	if ( tTextureBorderLeftPtr != NULL && *tTextureBorderLeftPtr != tTextureBorderLeft )
		SetLeftBorderTexture( *tTextureBorderLeftPtr );

	if ( tTextureBorderRightPtr != NULL && *tTextureBorderRightPtr != tTextureBorderRight )
		SetRightBorderTexture( *tTextureBorderRightPtr );

	if ( tTextureBorderTopPtr != NULL && *tTextureBorderTopPtr != tTextureBorderTop )
		SetTopBorderTexture( *tTextureBorderTopPtr );

	if ( tTextureBorderBottomPtr != NULL && *tTextureBorderBottomPtr != tTextureBorderBottom )
		SetBottomBorderTexture( *tTextureBorderBottomPtr );

	if ( tTextureBorderTopLeftPtr != NULL && *tTextureBorderTopLeftPtr != tTextureBorderTopLeft )
		SetTopLeftBorderTexture( *tTextureBorderTopLeftPtr );

	if ( tTextureBorderTopRightPtr != NULL && *tTextureBorderTopRightPtr != tTextureBorderTopRight )
		SetTopRightBorderTexture( *tTextureBorderTopRightPtr );

	if ( tTextureBorderBottomLeftPtr != NULL && *tTextureBorderBottomLeftPtr != tTextureBorderBottomLeft )
		SetBottomLeftBorderTexture( *tTextureBorderBottomLeftPtr );

	if ( tTextureBorderBottomRightPtr != NULL && *tTextureBorderBottomRightPtr != tTextureBorderBottomRight )
		SetBottomRightBorderTexture( *tTextureBorderBottomRightPtr );

	if ( !HasValidComponents() )
		return;

	FVector2D v2RenderLocation = ( ( IsRenderCaching() || !IsPositionable() ) ? FVector2D::ZeroVector : v2Origin + GetRenderLocation() );

	v2RenderLocation.X = bRoundPosition ? FMath::RoundToInt( v2RenderLocation.X ) : v2RenderLocation.X;
	v2RenderLocation.Y = bRoundPosition ? FMath::RoundToInt( v2RenderLocation.Y ) : v2RenderLocation.Y;

	EBlendMode eBlendMode = EBlendMode::BLEND_Translucent;

	switch ( this->eBlendMode )
	{
		case SE_BLEND_Opaque:
			eBlendMode = EBlendMode::BLEND_Opaque;

		case SE_BLEND_Masked:
		case SE_BLEND_MaskedDistanceField:
		case SE_BLEND_MaskedDistanceFieldShadowed:
			eBlendMode = EBlendMode::BLEND_Masked;

		case SE_BLEND_Additive:
			eBlendMode = EBlendMode::BLEND_Additive;

		case SE_BLEND_Modulate:
			eBlendMode = EBlendMode::BLEND_Modulate;

		case SE_BLEND_Translucent:
		case SE_BLEND_TranslucentDistanceField:
		case SE_BLEND_TranslucentDistanceFieldShadowed:
		case SE_BLEND_AlphaComposite:
		case SE_BLEND_AlphaBlend:
		default:
			eBlendMode = EBlendMode::BLEND_Translucent;
	}

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

	//oCanvas->SetClip( v2RenderLocation.X + v2Size.X, v2RenderLocation.Y + v2Size.Y );

	if ( tTextureBackground && v2CentreSize.X >= 1.f && v2CentreSize.Y >= 1.f )
		oCanvas->DrawTile(
			tTextureBackground,
			v2RenderLocation.X + v4BorderMetricsActual.X,
			v2RenderLocation.Y + v4BorderMetricsActual.Z,
			v2CentreSize.X,
			v2CentreSize.Y,
			0.f, 0.f,
			v2CentreSize.X,
			v2CentreSize.Y,
			eBlendMode
		);
	
	if ( tTextureBorderLeft && v4BorderMetricsActual.X >= 1.f && v2CentreSize.Y >= 1.f )
		oCanvas->DrawTile(
			tTextureBorderLeft,
			v2RenderLocation.X,
			v2RenderLocation.Y,
			v4BorderMetricsActual.X,
			v2CentreSize.Y,
			0.f, 0.f,
			tTextureBorderLeft->GetSurfaceWidth(),
			tTextureBorderLeft->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderRight && v4BorderMetricsActual.Y >= 1.f && v2CentreSize.Y >= 1.f )
		oCanvas->DrawTile(
			tTextureBorderRight,
			v2RenderLocation.X,
			v2RenderLocation.Y,
			v4BorderMetricsActual.Y,
			v2CentreSize.Y,
			0.f, 0.f,
			tTextureBorderRight->GetSurfaceWidth(),
			tTextureBorderRight->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderTop && v4BorderMetricsActual.Z >= 1.f && v2CentreSize.X >= 1.f )
		oCanvas->DrawTile(
			tTextureBorderTop,
			v2RenderLocation.X,
			v2RenderLocation.Y,
			v2CentreSize.X,
			v4BorderMetricsActual.Z,
			0.f, 0.f,
			tTextureBorderTop->GetSurfaceWidth(),
			tTextureBorderTop->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderBottom && v4BorderMetricsActual.W >= 1.f && v2CentreSize.X >= 1.f )
		oCanvas->DrawTile(
			tTextureBorderBottom,
			v2RenderLocation.X,
			v2RenderLocation.Y,
			v2CentreSize.X,
			v4BorderMetricsActual.W,
			0.f, 0.f,
			tTextureBorderBottom->GetSurfaceWidth(),
			tTextureBorderBottom->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderTopLeft && v4BorderMetricsActual.X >= 1.f && v4BorderMetricsActual.Z >= 1.f )
		oCanvas->DrawTile( 
			tTextureBorderTopLeft,
			v2RenderLocation.X, 
			v2RenderLocation.Y,
			v4BorderMetricsActual.X,
			v4BorderMetricsActual.Z,
			0.f, 0.f,
			tTextureBorderTopLeft->GetSurfaceWidth(), 
			tTextureBorderTopLeft->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderTopRight && v4BorderMetricsActual.Y >= 1.f && v4BorderMetricsActual.Z >= 1.f )
		oCanvas->DrawTile( 
			tTextureBorderTopRight,
			v2RenderLocation.X + v4BorderMetricsActual.Y + v2CentreSize.X,
			v2RenderLocation.Y,
			v4BorderMetricsActual.Y,
			v4BorderMetricsActual.Z,
			0.f, 0.f,
			tTextureBorderTopRight->GetSurfaceWidth(), 
			tTextureBorderTopRight->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderBottomLeft && v4BorderMetricsActual.X >= 1.f && v4BorderMetricsActual.W >= 1.f )
		oCanvas->DrawTile( 
			tTextureBorderBottomLeft,
			v2RenderLocation.X, 
			v2RenderLocation.Y + v4BorderMetricsActual.Z + v2CentreSize.Y,
			v4BorderMetricsActual.X,
			v4BorderMetricsActual.W,
			0.f, 0.f,
			tTextureBorderBottomLeft->GetSurfaceWidth(), 
			tTextureBorderBottomLeft->GetSurfaceHeight(),
			eBlendMode
		);

	if ( tTextureBorderBottomRight && v4BorderMetricsActual.Y >= 1.f && v4BorderMetricsActual.W >= 1.f )
		oCanvas->DrawTile(
			tTextureBorderBottomRight,
			v2RenderLocation.X + v4BorderMetricsActual.Y + v2CentreSize.X,
			v2RenderLocation.Y + v4BorderMetricsActual.Z + v2CentreSize.Y,
			v4BorderMetricsActual.Y,
			v4BorderMetricsActual.W,
			0.f, 0.f,
			tTextureBorderBottomRight->GetSurfaceWidth(), 
			tTextureBorderBottomRight->GetSurfaceHeight(),
			eBlendMode
		);
}
