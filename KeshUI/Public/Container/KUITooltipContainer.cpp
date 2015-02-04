// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "Container/KUITooltipContainer.h"


UKUITooltipContainer::UKUITooltipContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{

}


bool UKUITooltipContainer::CanReceieveKeyEvents() const
{
	return false;
}
