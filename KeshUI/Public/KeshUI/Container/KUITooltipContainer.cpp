// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/Container/KUITooltipContainer.h"


UKUITooltipContainer::UKUITooltipContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{

}


bool UKUITooltipContainer::CanReceieveKeyEvents() const
{
	return false;
}
