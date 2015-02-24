// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUITextureInterfaceComponent.h"


UKUITextureInterfaceComponent::UKUITextureInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	tTexture = NULL;
	tTexturePtr = NULL;
	v2Size = FVector2D::ZeroVector;
	v2TextureCoords = FVector2D::ZeroVector;
	v2TextureSize = FVector2D( 1.f, 1.f );
	rRotation = FRotator::ZeroRotator;
	v2PivotPoint = FVector2D::ZeroVector;
	fDepth = 1.f;
}


UTexture* UKUITextureInterfaceComponent::GetTexture() const
{
	return tTexture;
}


void UKUITextureInterfaceComponent::SetTexture( UTexture* tTexture )
{
	if ( this->tTexture == tTexture )
		return;

	bool bSetSize = true;

	if ( this->tTexture != NULL )
		bSetSize = ( this->tTexture->GetSurfaceWidth() == GetSize().X && this->tTexture->GetSurfaceHeight() == GetSize().Y );

	else
		bSetSize = ( GetSize().X == 0.f && GetSize().Y == 0.f );

	this->tTexture = tTexture;

	Invalidate();

	if ( bSetSize )
	{
		if ( this->tTexture != NULL )
			SetSize( this->tTexture->GetSurfaceWidth(), this->tTexture->GetSurfaceHeight() );

		else
			SetSize( 0.f, 0.f );
	}
}


void UKUITextureInterfaceComponent::SetTexturePointer( UTexture** tTexturePtr )
{
	if ( this->tTexturePtr == tTexturePtr )
		return;

	this->tTexturePtr = tTexturePtr;

	SetTexture( *tTexturePtr );
}


void UKUITextureInterfaceComponent::SetTextureName( const FName& nTextureName )
{
	SetTexturePointer( GetAsset<UTexture>( nTextureName ) );
}


float UKUITextureInterfaceComponent::GetDepth() const
{
	return fDepth;
}


void UKUITextureInterfaceComponent::SetDepth( float fDepth )
{
	if ( this->fDepth == fDepth )
		return;

	this->fDepth = fDepth;

	InvalidateContainerRenderCache();
}


const FVector2D& UKUITextureInterfaceComponent::GetSize() const
{
	return v2Size;
}


void UKUITextureInterfaceComponent::SetSizeStruct( const FVector2D& v2Size )
{
	SetSize( v2Size.X, v2Size.Y );
}


void UKUITextureInterfaceComponent::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	v2Size.X = fWidth;
	v2Size.Y = fHeight;

	Super::SetSize( fWidth, fHeight );
}


const FVector2D& UKUITextureInterfaceComponent::GetTextureCoords() const
{
	return v2TextureCoords;
}


void UKUITextureInterfaceComponent::SetTextureCoordsStruct( const FVector2D& v2TextureCoords )
{
	SetTextureCoords( v2TextureCoords.X, v2TextureCoords.Y );
}


void UKUITextureInterfaceComponent::SetTextureCoords( float fU, float fV )
{
	if ( v2TextureCoords.X == fU && v2TextureCoords.Y == fV )
		return;

	v2TextureCoords.X = fU;
	v2TextureCoords.Y = fV;

	InvalidateContainerRenderCache();
}


const FVector2D& UKUITextureInterfaceComponent::GetTextureSize() const
{
	return v2TextureSize;
}


void UKUITextureInterfaceComponent::SetTextureSizeStruct( const FVector2D& v2TextureSize )
{
	SetTextureSize( v2TextureSize.X, v2TextureSize.Y );
}


void UKUITextureInterfaceComponent::SetTextureSize( float fUL, float fVL )
{
	if ( v2TextureSize.X == fUL && v2TextureSize.Y == fVL )
		return;

	v2TextureSize.X = fUL;
	v2TextureSize.Y = fVL;

	InvalidateContainerRenderCache();
}


const FRotator& UKUITextureInterfaceComponent::GetRotation() const
{
	return rRotation;
}


void UKUITextureInterfaceComponent::SetRotationStruct( const FRotator& rRotation )
{
	SetRotation( rRotation.Pitch, rRotation.Yaw, rRotation.Yaw );
}


void UKUITextureInterfaceComponent::SetRotation( float fPitch, float fYaw, float fRoll )
{
	if ( rRotation.Pitch == fPitch && rRotation.Yaw == fYaw && rRotation.Roll == fRoll )
		return;

	rRotation.Pitch = fPitch;
	rRotation.Yaw = fYaw;
	rRotation.Roll = fRoll;
	
	InvalidateContainerRenderCache();
}


const FVector2D& UKUITextureInterfaceComponent::GetPivotPoint() const
{
	return v2PivotPoint;
}


void UKUITextureInterfaceComponent::SetPivotPointStruct( const FVector2D& v2PivotPoint )
{
	SetPivotPoint( v2PivotPoint.X, v2PivotPoint.Y );
}


void UKUITextureInterfaceComponent::SetPivotPoint( float fX, float fY )
{
	if ( v2PivotPoint.X == fX && v2PivotPoint.Y == fY )
		return;

	v2PivotPoint.X = fX;
	v2PivotPoint.Y = fY;

	InvalidateContainerRenderCache();
}


bool UKUITextureInterfaceComponent::HasValidComponents() const
{
	if ( v2Size.X <= 0 || v2Size.Y <= 0 )
		return false;

	if ( tTexture == NULL )
		return false;

	if ( tTexture->Resource == NULL )
		return false;

	return true;
}


void UKUITextureInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasTileItem>( new FCanvasTileItem(
		FVector2D::ZeroVector,
		tTexture->Resource,
		v2Size,
		v2TextureCoords,
		v2TextureCoords + v2TextureSize,
		GetDrawColor().ReinterpretAsLinear()
	) );

	static_cast<FCanvasTileItem*>(&*stItem)->Rotation = rRotation;
	static_cast<FCanvasTileItem*>(&*stItem)->PivotPoint = v2PivotPoint;
	static_cast<FCanvasTileItem*>(&*stItem)->Z = fDepth;

	Super::ConstructNewItem();
}


void UKUITextureInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( tTexturePtr != NULL && *tTexturePtr != tTexture )
		SetTexture( *tTexturePtr );

	if ( stItem.IsValid() )
	{
		static_cast<FCanvasTileItem*>(&*stItem)->Size = v2Size;
		static_cast<FCanvasTileItem*>(&*stItem)->UV0 = v2TextureCoords;
		static_cast<FCanvasTileItem*>(&*stItem)->UV1 = v2TextureCoords + v2TextureSize;
		static_cast<FCanvasTileItem*>(&*stItem)->Rotation = rRotation;
		static_cast<FCanvasTileItem*>(&*stItem)->PivotPoint = v2PivotPoint;
		static_cast<FCanvasTileItem*>(&*stItem)->Z = fDepth;
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}


void UKUITextureInterfaceComponent::EnableRenderCache()
{
	// Stops the parent method from being called, stopping render caching - It's not needed for this type of component.
}
