// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterface.h"
#include "Container/KUIRootContainer.h"


UKUIRootContainer::UKUIRootContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{
	aInterface = NULL;
}


AKUIInterface* UKUIRootContainer::GetInterface() const
{
	return aInterface.Get();
}


void UKUIRootContainer::SetInterface( AKUIInterface* aInterface )
{
	this->aInterface = aInterface;
}


bool UKUIRootContainer::IsVisibleRecursive() const
{
	return IsVisible();
}


void UKUIRootContainer::OnScreenResolutionChange( const FKUIInterfaceContainerScreenResolutionEvent& stEventInfo )
{
	SetSizeStruct( stEventInfo.v2NewResolution );

	Super::OnScreenResolutionChange( stEventInfo );
}
