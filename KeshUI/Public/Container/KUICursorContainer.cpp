// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "KeshUI.h"
#include "Container/KUICursorContainer.h"


UKUICursorContainer::UKUICursorContainer( const class FObjectInitializer& oObjectInitializer )
	: Super(oObjectInitializer)
{

}


bool UKUICursorContainer::CanReceieveKeyEvents() const
{
	return false;
}
