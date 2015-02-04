// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

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
