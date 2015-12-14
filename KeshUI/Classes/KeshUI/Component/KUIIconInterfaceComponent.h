// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/KUIInterfaceComponent.h"
#include "KeshUI/KUIMacros.h"
#include "KUIIconInterfaceComponent.generated.h"

/**
 * KeshUI UI Framework (KUI) Texture render component.
 */
UCLASS(ClassGroup="KeshUI|Component", Blueprintable, BlueprintType)
class KESHUI_API UKUIIconInterfaceComponent : public UKUIInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIIconInterfaceComponent )

	UKUIIconInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the texture. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual UTexture* GetTexture() const;

	/* Sets the texture. Sets the screen size to match this if it's zero. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetTexture( UTexture* tTexture );

	/* Sets the texture by pointer. */
	virtual void SetTexturePointer( UTexture** tTexturePtr );

	/* Sets the texture by asset name. */
	UFUNCTION( Category = "KeshUI|Component|Icon", BlueprintCallable )
	virtual void SetTextureName( const FName& nTextureName );

	/* Gets the texture coordinates. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual const FVector2D& GetTextureCoords() const;

	/* Sets the texture coordinates. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetTextureCoordsStruct( const FVector2D& v2TextureCoords );

	/* Sets the texture coordinates. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetTextureCoords( float fU, float fV );

	/* Gets the texture size.  Texture size is added on to the texture coordinate. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual const FVector2D& GetTextureSize() const;

	/* Sets the texture size. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetTextureSizeStruct( const FVector2D& v2TextureSize );

	/* Sets the texture size. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetTextureSize( float fUL, float fVL );

	/* Returns the scale of the icon. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual float GetIconScale() const;

	/* Sets the scale of the icon. */
	UFUNCTION(Category="KeshUI|Component|Icon", BlueprintCallable)
	virtual void SetIconScale( float fScale );

	/* Gets the size of this component. */
	virtual const FVector2D& GetSize() const override;

	/* Overrides to disable. */
	virtual void EnableRenderCache() override;

	/* Returns true if this component can render. */
	virtual bool CanRender() const override;

	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	FCanvasIcon stIcon;
	float fScale;
	UTexture** tTexturePtr;
	FVector2D v2Size;
	FVector2D v2TexSize;
	FVector2D v2TexCoords;

};
