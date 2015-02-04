// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Component/KUICanvasItemInterfaceComponent.h"
#include "KUIMacros.h"
#include "KUIBoxInterfaceComponent.generated.h"

/**
 * KeshUI UI Framework (KUI) Box render component.
 */
UCLASS(ClassGroup="KeshUI | Component", Blueprintable, BlueprintType)
class KESHUI_API UKUIBoxInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIBoxInterfaceComponent )

public:

	/* Gets the size of the component. */
	virtual FVector2D GetSize() const override;

	/* Sets the size of the component. */
	UFUNCTION(Category="KeshUI | Component | Box", BlueprintCallable)
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the component. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the thickness of the texture from the screen. */
	UFUNCTION(Category="KeshUI | Component | Box", BlueprintCallable)
	virtual float GetThickness() const;

	/* Sets the texture location. */
	UFUNCTION(Category="KeshUI | Component | Box", BlueprintCallable)
	virtual void SetThickness( float fThickness );

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	/* Updates size and thickness. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	FVector2D v2Size;
	float fThickness;

	/* Tries to construct a new FCanvasBoxItem! */
	virtual void ConstructNewItem() override;

};
