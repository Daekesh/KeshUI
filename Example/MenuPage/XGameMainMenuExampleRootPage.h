// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.

#pragma once

/*
 * The root main menu page for game X. Contains 3 buttons with text-based elements.
 */


// Include the base class, Grid Container, from the KeshUI module
#include <Container/KUIGridContainer.h>

// Some useful macros
#include "KUIMacros.h"

// Generated header
#include "XGameMainMenuExampleRootPage.generated.h"


// Forward declare classes so we don't need headers
class UKUISimpleClickWidget;
class UKUIButtonWidget;


/**
 * The root main menu page.
 */
UCLASS( ClassGroup = "X | UI | Main Menu", BlueprintType, Blueprintable )
class UXGameMainMenuExampleRootPage : public UKUIGridContainer
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UXGameMainMenuExampleRootPage )

	UXGameMainMenuExampleRootPage( const class FObjectInitializer& oObjectInitializer );

protected:

	// Store references to our 3 buttons. Make sure they are uproperties to keep the reference count.
	// The UPROPERTY isn't really required because they are ref counted via the Children property of
	// their container, but just for safety.

	UPROPERTY()
	UKUIButtonWidget* cmPlayButton;

	UPROPERTY()
	UKUIButtonWidget* cmOptionsButton;

	UPROPERTY()
	UKUIButtonWidget* cmExitButton;

	// The function called when one of our buttons is clicked
	UFUNCTION()
	virtual void OnMainMenuButtonClick( UKUISimpleClickWidget* cmClicked, const FVector2D& v2ClickOffset );

};
