// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.

// Game header
#include "XGame.h"

// Paged container from KeshUI module
#include <Container/KUIPagedContainer.h>

// Fictitious menu pages
#include "UI/MainMenu/XGameMainMenuRootPage.h"
#include "UI/MainMenu/XGameMainMenuPlayPage.h"
#include "UI/MainMenu/XGameMainMenuOptionsPage.h"
#include "UI/MainMenu/XGameMainMenuServerBrowserPage.h" 

// class header
#include "UI/XGameMainMenuExampleHud.h"


AXGameMainMenuExampleHud::AXGameMainMenuExampleHud( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	// Create a page container to add our menu pages to.
	KUICreateDefaultSubobjectAssign( oPages, UKUIPagedContainer, "Page Container" );
	oPages->FillContainer();

	// Add the page container to the root layer (others are overlay, tooltip and cursor - you can define your own custom layers)
	AddElement( EKUIInterfaceRoot::R_Root, oPages );

	// Create menu pages and add them to the root container
	KUICreateDefaultSubobject( ctMainMenuPage, UXGameMainMenuRootPage, "Main Page" );
	oPages->AddPage( ctMainMenuPage );

	KUICreateDefaultSubobject( ctPlayPage, UXGameMainMenuPlayPage, "Play Page" );
	oPages->AddPage( ctPlayPage );

	KUICreateDefaultSubobject( ctMainMenuOptionPage, UXGameMainMenuOptionsPage, "Options Page" );
	oPages->AddPage( ctMainMenuOptionPage );

	KUICreateDefaultSubobject( ctMainMenuServerBrowserPage, UXGameMainMenuServerBrowserPage, "Server Browser Page" );
	oPages->AddPage( ctMainMenuServerBrowserPage );

	// Make sure the main menu root page is active
	SwitchToPage( EXGameMainMenuPages::P_MainMenu );
}


void AXGameMainMenuExampleHud::SwitchToPage( TEnumAsByte<EXGameMainMenuPages::Page> ePage )
{
	oPages->SetActivePageByIndex( ePage );
}
