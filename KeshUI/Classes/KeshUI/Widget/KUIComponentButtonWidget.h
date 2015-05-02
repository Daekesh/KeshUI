// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Widget/KUIButtonWidget.h"
#include "KeshUI/KUIMacros.h"
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
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIComponentButtonWidget )

	UKUIComponentButtonWidget( const class FObjectInitializer& oObjectInitializer );

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

	UPROPERTY( Category = "KeshUI | UI | Widget | Component Button", VisibleInstanceOnly, BlueprintReadOnly )
	uint8 iButtonComponentIndex;

	UPROPERTY( Category = "KeshUI | UI | Widget | Component Button", VisibleInstanceOnly, BlueprintReadOnly )
	uint8 iElementComponentIndex;

	virtual void OnWidgetStateChange() override;

	virtual EKUIButtonState::State GetComponentIndex( const TArray<UKUIInterfaceElement*>& arComponents, EKUIButtonState::State eState ) const;

};
