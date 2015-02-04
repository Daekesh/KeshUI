// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Widget/KUIButtonWidget.h"
#include "KUIMacros.h"
#include "KUIComponentButtonWidget.generated.h"

UENUM( BlueprintType )
namespace EKUIButtonState
{
	enum State
	{
		S_Default                    UMETA( DisplayName = "Default State" ),
		S_MouseOver                  UMETA( DisplayName = "Mouse Over State" ),
		S_Clicked                    UMETA( DisplayName = "Clicked State" ),
		S_MouseOver_Clicked          UMETA( DisplayName = "Mouse Over + Clicked State" ),
		S_Disabled                   UMETA( DisplayName = "Disabled State" ),
		S_MouseOver_Disabled         UMETA( DisplayName = "Mouse Over + Disabled State" ),
		S_Clicked_Disabled           UMETA( DisplayName = "Clicked + Disabled State" ),
		S_MouseOver_Clicked_Disabled UMETA( DisplayName = "Mouse Over + Clicked + Disabled State" ),
		S_Max                        UMETA( Hidden )
	};
}


/* Button component using standard textures. */
UCLASS( ClassGroup = "KeshUI | UI | Widget", BlueprintType, Blueprintable )
class KESHUI_API UKUIComponentButtonWidget : public UKUIButtonWidget
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIComponentButtonWidget )

public:

	/* Sets a button component. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual UKUIInterfaceElement* GetButtonComponent( EKUIButtonState::State eState ) const;

	/* Sets a button component. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual void SetButtonComponent( EKUIButtonState::State eState, UKUIInterfaceElement* oComponent );

	/* Sets the sizes of button components. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual void SetButtonComponentSize( float fX, float fY );

	/* Sets the sizes of button components. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual void SetButtonComponentSizeStruct( FVector2D v2Size );

	/* Sets an element component. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual UKUIInterfaceElement* GetElementComponent( EKUIButtonState::State eState ) const;

	/* Sets an element component. */
	UFUNCTION( Category = "KeshUI | Widget | Button", BlueprintCallable )
	virtual void SetElementComponent( EKUIButtonState::State eState, UKUIInterfaceElement* oComponent );

protected:

	UPROPERTY()
	TArray<UKUIInterfaceElement*> arButtonComponents;

	UPROPERTY()
	TArray<UKUIInterfaceElement*> arElementComponents;

	FVector2D v2ClickedElementOffset;

	virtual void OnWidgetStateChange() override;

	virtual EKUIButtonState::State GetComponentIndex( const TArray<UKUIInterfaceElement*>& arComponents, EKUIButtonState::State eState ) const;

};
