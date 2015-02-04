// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "KUIInterfaceComponent.h"


UKUIInterfaceComponent::UKUIInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	SetSmoothEnabled( true );
	SetDrawColorStruct( FColor::White );
	ctContainer = NULL;
}


bool UKUIInterfaceComponent::IsSmoothEnabled() const
{
	return bSmooth;
}


void UKUIInterfaceComponent::SetSmoothEnabled( bool bSmooth )
{
	if ( this->bSmooth == bSmooth )
		return;

	this->bSmooth = bSmooth;

	InvalidateRenderCache();
}


FColor UKUIInterfaceComponent::GetDrawColor() const
{
	return coColor;
}


void UKUIInterfaceComponent::SetDrawColorStruct( const FColor& stColor )
{
	SetDrawColor( stColor.R, stColor.G, stColor.B, stColor.A );
}


void UKUIInterfaceComponent::SetDrawColor( float fRed, float fGreen, float fBlue, float fAlpha )
{
	if ( coColor.R == fRed && coColor.G == fGreen && coColor.B == fBlue && coColor.A == fAlpha )
		return;
	
	coColor.R = fRed;
	coColor.G = fGreen;
	coColor.B = fBlue;
	coColor.A = fAlpha;

	InvalidateRenderCache();
}


bool UKUIInterfaceComponent::IsPositionable() const
{
	return true;
}


bool UKUIInterfaceComponent::CanRender() const
{
	return true;
}


void UKUIInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );

	if ( IsRenderCaching() && oRenderCacheObject != this )
		return;

	oCanvas->bNoSmooth = !IsSmoothEnabled();
	oCanvas->DrawColor = GetDrawColor();
}
