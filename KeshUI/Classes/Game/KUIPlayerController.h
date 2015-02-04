// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIMacros.h"
#include "KUIPlayerController.generated.h"

class AKUIPlayerActor;
class AKUIPlayerState;

/**
* Exmaple Player Controller for Kesh UI
*/
UCLASS( ClassGroup = "KeshUI | Game", Blueprintable, BlueprintType )
class KESHUI_API AKUIPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( AKUIPlayerController )

public:

	// Updates the mouse
	virtual void PlayerTick( float fDeltaTime ) override;

	/* Called to process input. */
	virtual void ProcessInput();

	/* Gets mouse movement */
	UFUNCTION( Category = "KeshUI | Game | Player Controller", BlueprintCallable )
	virtual void OnMouseMoved( const FVector2D& v2Delta );

	/* Triggers key events for the UI. */
	virtual bool InputKey( FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad ) override;

	/* Calls input events. */
	virtual bool InputKeyEvent( FKey eKey, EInputEvent eEventType );

	/* Triggers text input for the ui. */
	virtual bool InputCharEvent( TCHAR chChar );

};
