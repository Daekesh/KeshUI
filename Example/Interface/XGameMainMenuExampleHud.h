// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.

#pragma once

/*
 * This example hud (not related to other examples) shows how you can use a paged container
 * to add different menu pages.
 */


// Assume we're in another module, so include via <>
#include <AKUIInterface.h>

// Some useful macros
#include <KUIMacros.h>

// Generated header
#include "XGameMainMenuExampleHud.generated.h"


// Define a nice human readable enum we can use in blueprints/cpp to select a page
UENUM( BlueprintType )
namespace EXGameMainMenuExamplePages
{
	enum Page
	{
		P_MainMenu      UMETA( DisplayName = "Main Menu" ),
		P_Play          UMETA( DisplayName = "Play" ),
		P_Options       UMETA( DisplayName = "Options" ),
		P_ServerBrowser UMETA( DisplayName = "Server Browser" )
	};
}


// Forward declare this class so we don't need to include the header
class UKUIPagedContainer;


/**
* The main menu for game X.
*/
UCLASS( ClassGroup = "X | UI | Main Menu", Blueprintable, BlueprintType )
class AXGameMainMenuExampleHud : public AKUIInterface
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( AXGameMainMenuExampleHud ) // doesn't really do anything at the moment except switch to public:

	// Define the constructor to add menu elements / pages
	AXGameMainMenuExampleHud( const class FObjectInitializer& oObjectInitializer );

public:

	// Define a function to switch pages - changes the page of our internal page container
	UFUNCTION( Category = "X | UI | Main Menu", BlueprintCallable )
	virtual void SwitchToPage( TEnumAsByte<EXGameMainMenuExamplePages::Page> ePage );

protected:

	// The page container we used to hold our menu pages
	UPROPERTY()
	UKUIPagedContainer* oPages;

};
