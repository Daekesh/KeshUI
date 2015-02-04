// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUIIconInterfaceComponent.h"


UKUIIconInterfaceComponent::UKUIIconInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	stIcon = UCanvas::MakeIcon( NULL );
	fScale = 1.f;
}


UTexture* UKUIIconInterfaceComponent::GetTexture() const
{
	return stIcon.Texture;
}


void UKUIIconInterfaceComponent::SetTexture( UTexture* tTexture )
{
	if ( stIcon.Texture == tTexture )
		return;

	stIcon = UCanvas::MakeIcon( tTexture, stIcon.U, stIcon.V, stIcon.UL, stIcon.VL );
	
	InvalidateContainerRenderCache();
}


void UKUIIconInterfaceComponent::SetTexturePointer( UTexture** tTexturePtr )
{
	if ( this->tTexturePtr == tTexturePtr )
		return;

	this->tTexturePtr = tTexturePtr;

	SetTexture( *tTexturePtr );
}


void UKUIIconInterfaceComponent::SetTextureName( const FName& nTextureName )
{
	SetTexturePointer( ( UTexture** ) GetAsset( nTextureName ) );
}


FVector2D UKUIIconInterfaceComponent::GetTextureCoords() const
{
	return FVector2D( stIcon.U, stIcon.V );
}


void UKUIIconInterfaceComponent::SetTextureCoordsStruct( const FVector2D& v2TextureCoords )
{
	SetTextureCoords( v2TextureCoords.X, v2TextureCoords.Y );
}


void UKUIIconInterfaceComponent::SetTextureCoords( float fU, float fV )
{
	if ( stIcon.U == fU && stIcon.V == fV )
		return;

	stIcon.U = fU;
	stIcon.V = fV;

	InvalidateContainerRenderCache();
}


FVector2D UKUIIconInterfaceComponent::GetTextureSize() const
{
	return FVector2D( stIcon.UL, stIcon.VL );
}


void UKUIIconInterfaceComponent::SetTextureSizeStruct( const FVector2D& v2TextureSize )
{
	return SetTextureSize( v2TextureSize.X, v2TextureSize.Y );
}


void UKUIIconInterfaceComponent::SetTextureSize( float fUL, float fVL )
{
	if ( stIcon.UL == fUL && stIcon.VL == fVL )
		return;

	stIcon.UL = fUL;
	stIcon.VL = fVL;

	InvalidateContainerRenderCache();
}


float UKUIIconInterfaceComponent::GetIconScale() const
{
	return fScale;
}


void UKUIIconInterfaceComponent::SetIconScale( float fScale )
{
	if ( this->fScale == fScale )
		return;

	this->fScale = fScale;

	if ( GetContainer() != NULL )
	{
		FKUIInterfaceContainerElementEvent stEventInfo( EKUIInterfaceContainerEventList::E_ChildSizeChange, this );
		GetContainer()->SendEvent( stEventInfo );
	}

	InvalidateContainerRenderCache();
}


FVector2D UKUIIconInterfaceComponent::GetSize() const
{
	if ( stIcon.Texture == NULL )
		return FVector2D::ZeroVector;

	return FVector2D( stIcon.Texture->GetSurfaceWidth() * fScale, stIcon.Texture->GetSurfaceHeight() * fScale );
}


bool UKUIIconInterfaceComponent::CanRender() const
{
	return ( stIcon.Texture != NULL );
}


void UKUIIconInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( tTexturePtr != NULL && *tTexturePtr != stIcon.Texture )
		SetTexture( *tTexturePtr );

	if ( stIcon.Texture == NULL )
		return;

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );

	if ( IsRenderCaching() && oRenderCacheObject != this )
		return;

	const FVector2D v2Size = GetSize();
	const FVector2D v2RenderLocation = ( IsRenderCaching() ? FVector2D::ZeroVector : GetRenderLocation() );

	//oCanvas->SetClip( FMath::RoundToInt( v2Origin.X + v2RenderLocation.X ) + v2Size.X, FMath::RoundToInt( v2Origin.Y + v2RenderLocation.Y ) + v2Size.Y );
	oCanvas->DrawIcon( stIcon, FMath::RoundToInt( v2Origin.X + v2RenderLocation.X ), FMath::RoundToInt( v2Origin.Y + v2RenderLocation.Y ), fScale );
}


void UKUIIconInterfaceComponent::EnableRenderCache()
{
	// Stops the parent method from being called, stopping render caching - It's not needed for this type of component.
}
