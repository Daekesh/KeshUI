// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/KUIInterfaceContainer.h"
#include "KeshUI/Component/KUIBoxInterfaceComponent.h"


UKUIBoxInterfaceComponent::UKUIBoxInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	bItemInvalidated = true;
	
	v2Size = FVector2D::ZeroVector;
	fThickness = 1.f;
}


const FVector2D& UKUIBoxInterfaceComponent::GetSize() const
{
	return v2Size;
}


void UKUIBoxInterfaceComponent::SetSizeStruct( const FVector2D& v2Size )
{
	SetSize( v2Size.X, v2Size.Y );
}


void UKUIBoxInterfaceComponent::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	v2Size.X = fWidth;
	v2Size.Y = fHeight;

	Super::SetSize( fWidth, fHeight );
}


float UKUIBoxInterfaceComponent::GetThickness() const
{
	return fThickness;
}


void UKUIBoxInterfaceComponent::SetThickness( float fThickness )
{
	if ( this->fThickness == fThickness )
		return;

	this->fThickness = fThickness;

	InvalidateRenderCache();
}


bool UKUIBoxInterfaceComponent::HasValidComponents() const
{
	if ( v2Size.X <= 0 || v2Size.Y <= 0 )
		return false;

	if ( fThickness <= 0.f )
		return false;

	return true;
}


void UKUIBoxInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasBoxItem>( new FCanvasBoxItem(
		FVector2D::ZeroVector,
		v2Size
	) );

	static_cast<FCanvasBoxItem*>(&*stItem)->LineThickness = fThickness;
	static_cast< FCanvasBoxItem* >( &*stItem )->SetColor( GetDrawColor().ReinterpretAsLinear() );

	Super::ConstructNewItem();
}


void UKUIBoxInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( stItem.IsValid() )
	{
		static_cast<FCanvasBoxItem*>(&*stItem)->Size = v2Size;
		static_cast<FCanvasBoxItem*>(&*stItem)->LineThickness = fThickness;
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}
