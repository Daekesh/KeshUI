// Copyright 2015 Matt Chapman, Inc. All Rights Reserved.


#include "XGame.h"
#include <Component/KUITextureInterfaceComponent.h>
#include "UI/Widget/XGameBlueButtonWidget.h"

// Macro to save some space... decipher at will :P
#define XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( TextureName, ButtonState, ImageFilename ) \
	static KUIFindObj( tTexture##TextureName, UTexture2D, "Texture2D'/Game/Textures/UserInterface/Generic/Buttons/Blue/" L#ImageFilename L"." L#ImageFilename L"'" ); \
	KUICreateDefaultSubobject( cmTexture##TextureName, UKUITextureInterfaceComponent, #TextureName " Texture" ); \
	cmTexture##TextureName->SetTexture( tTexture##TextureName.Object ); \
	SetButtonComponent( EKUIButtonState::S_##ButtonState, cmTexture##TextureName );

UXGameBlueButtonWidget::UXGameBlueButtonWidget( const class FObjectInitializer& oObjectInitializer )
	: Super( oObjectInitializer )
{
	// Set the texture for each button state
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( Default,              Default,                    default );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( Hover,                MouseOver,                  hover );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( Clicked,              Clicked,                    click );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( HoverClicked,         MouseOver_Clicked,          click_hover );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( Disabled,             Disabled,                   disabled );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( HoverDisabled,        MouseOver_Disabled,         disabled_hover );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( ClickedDisabled,      Clicked_Disabled,           disabled_click );
	XGAME_BLUE_BUTTON_SET_STATE_COMPONENT( HoverClickedDisabled, MouseOver_Clicked_Disabled, disabled_click_hover );

	// Set the default alignment of the button to be in the centre of its container
	SetHorizontalAlignment( EKUIInterfaceHAlign::HA_Centre );
	SetVerticalAlignment( EKUIInterfaceVAlign::VA_Centre );

	// Set the size of the button to the size of the background (component) texture
	SetSize( cmTextureDefault->GetSize().X, cmTextureDefault->GetSize().Y );
}
