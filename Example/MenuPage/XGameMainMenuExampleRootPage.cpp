// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.

#include "XGame.h"

// Text interface componet used for the button elements
#include <Component/KUITextInterfaceComponent.h>

// The main menu hud we need to switch pages
#include "UI/XGameMainMenuHud.h"

// A fictitious button widget defining a "blue" component style
#include "UI/Widget/XGameBlueButtonWidget.h"

// The main menu page
#include "UI/MainMenu/XGameMainMenuExampleRootPage.h"


UXGameMainMenuExampleRootPage::UXGameMainMenuExampleRootPage( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	// Set up a margin, so we aren't on the edge of the screen.
	// Leave 100 pixels to the left and 300 to the top.
	SetMargin( 100.f, 300.f, 0.f, 0.f );
	
	// Set the size to 300 pixels square.
	SetSize( 300.f, 300.f );

	// Grid containers work on a (variable sized) list of rows and columns. We only need a single column, so add one full size (300px)
	AddColumn( 300.f );

	// Add 3 rows, 1 for each button
	AddRow( 100.f );
	AddRow( 100.f );
	AddRow( 100.f );



	// Create the play button and set the function called when the button is clicked.
	KUICreateDefaultSubobjectAssign( cmPlayButton, UXGameBlueButtonWidget, "Play Button" );
	cmPlayButton->SetClickDelegate( this, static_cast<FKUISimpleClickWidgetClickPrototype>( &UXGameMainMenuExampleRootPage::OnMainMenuButtonClick ) );

	KUICreateDefaultSubobject( cmPlayButtonText, UKUITextInterfaceComponent, "Play Button Text" );
	cmPlayButtonText->SetFontName( "Font_ExtraLarge" ); // Uses asset library.
	cmPlayButtonText->SetTextString( "Play" );
	cmPlayButtonText->SetDrawColor( 196.f, 223.f, 255.f );

	// Set the button "element" (the central part of the button.)
	Cast<UXGameBlueButtonWidget>( cmPlayButton )->SetElementComponent( EKUIButtonState::S_Default, cmPlayButtonText );



	// Same for the options button.
	KUICreateDefaultSubobjectAssign( cmOptionsButton, UXGameBlueButtonWidget, "Options Button" );
	cmOptionsButton->SetClickDelegate( this, static_cast<FKUISimpleClickWidgetClickPrototype>( &UXGameMainMenuExampleRootPage::OnMainMenuButtonClick ) );

	KUICreateDefaultSubobject( cmOptionsButtonText, UKUITextInterfaceComponent, "Options Button Text" );
	cmOptionsButtonText->SetFontName( "Font_ExtraLarge" );
	cmOptionsButtonText->SetTextString( "Options" );
	cmOptionsButtonText->SetDrawColor( 196.f, 223.f, 255.f );

	Cast<UXGameBlueButtonWidget>( cmOptionsButton )->SetElementComponent( EKUIButtonState::S_Default, cmOptionsButtonText );


	
	// And the exit button.
	KUICreateDefaultSubobjectAssign( cmExitButton, UXGameBlueButtonWidget, "Exit Button" );
	cmExitButton->SetClickDelegate( this, static_cast<FKUISimpleClickWidgetClickPrototype>( &UXGameMainMenuExampleRootPage::OnMainMenuButtonClick ) );

	KUICreateDefaultSubobject( cmExitButtonText, UKUITextInterfaceComponent, "Exit Button Text" );
	cmExitButtonText->SetFontName( "Font_ExtraLarge" );
	cmExitButtonText->SetTextString( "Exit" );
	cmExitButtonText->SetDrawColor( 196.f, 223.f, 255.f );

	Cast<UXGameBlueButtonWidget>( cmExitButton )->SetElementComponent( EKUIButtonState::S_Default, cmExitButtonText );



	// Add the buttons to the grid (row, col)
	SetGridSlot( 0, 0, cmPlayButton );
	SetGridSlot( 1, 0, cmOptionsButton );
	SetGridSlot( 2, 0, cmExitButton );
}


void UXGameMainMenuExampleRootPage::OnMainMenuButtonClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset )
{
	// Check which button was clicked.
	if ( cmClicked == cmPlayButton )
	{
		// Use the super log macro.
		KUILogUO( "Play Button Clicked %s", *this->GetInterface()->GetName() );
		
		// Swithc to the play page
		Cast<AXGameMainMenuHud>( this->GetInterface() )->SwitchToPage( EXGameMainMenuExamplePages::P_Play );
	}

	else if ( cmClicked == cmOptionsButton )
	{
		KUILogUO( "Options Button Clicked %s", *this->GetInterface()->GetName() );
		Cast<AXGameMainMenuHud>( this->GetInterface() )->SwitchToPage( EXGameMainMenuExamplePages::P_Options );
	}

	else if ( cmClicked == cmExitButton )
	{
		KUILogUO( "Exit Button Clicked %s", *this->GetInterface()->GetName() );

		// Runs the PlayerController console function on the first local player controller.
		KUIConsoleCommand( "quit" );
	}
}
