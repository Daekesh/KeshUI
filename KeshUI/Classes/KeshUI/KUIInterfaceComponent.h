// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceElement.h"
#include "KeshUI/KUIMacros.h"
#include "KUIInterfaceComponent.generated.h"

/**
 * The base KeshUI class for all ui components.  Lines, textures, etc.
 */
UCLASS(Abstract, Category="KeshUI", BlueprintType, Blueprintable)
class KESHUI_API UKUIInterfaceComponent : public UKUIInterfaceElement
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIInterfaceComponent )

	UKUIInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns whether bSmooth is enabled on the canvas. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual bool IsSmoothEnabled() const;

	/* Sets smooth enabled or not for this component. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual void SetSmoothEnabled( bool bSmooth );

	/* Returns the default draw colour for this element. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual FColor GetDrawColor() const;

	/* Sets the default draw color. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual void SetDrawColorStruct( const FColor& stColor );

	/* Sets the default draw color. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual void SetDrawColor( float fRed, float fGreen, float fBlue, float fAlpha = 255.f );

	/* Returns true if this can have its location changed. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual bool IsPositionable() const;

	/* Returns true if this can be rendered. */
	UFUNCTION(Category="KeshUI|Component", BlueprintCallable)
	virtual bool CanRender() const;

	/* Called to render the element on the screen.  The Canvas is reset before each render call. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	bool bSmooth;
	FColor coColor;

};
