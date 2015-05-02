// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Component/KUICanvasItemInterfaceComponent.h"
#include "KeshUI/KUIMacros.h"
#include "KUITextureInterfaceComponent.generated.h"

/**
 * KeshUI UI Framework (KUI) Texture render component.
 */
UCLASS(ClassGroup="KeshUI | Component", Blueprintable, BlueprintType)
class KESHUI_API UKUITextureInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUITextureInterfaceComponent )

	UKUITextureInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the texture. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual UTexture* GetTexture() const;

	/* Sets the texture. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetTexture( UTexture* tTexture );

	/* Sets the texture by pointer. */
	virtual void SetTexturePointer( UTexture** tTexturePtr );

	/* Set texture by asset name. */
	UFUNCTION( Category = "KeshUI | Component | Texture", BlueprintCallable )
	virtual void SetTextureName( const FName& nTextureName );

	/* Gets the depth of the texture from the screen. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual float GetDepth() const;

	/* Sets the texture location. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetDepth( float fDepth );

	/* Gets the size of texture on the screen (at 0 depth.) */
	virtual const FVector2D& GetSize() const override;

	/* Sets the size of the texture on the screen (at 0 depth.) */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the texture on the screen (at 0 depth.) */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the texture coordinates. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual const FVector2D& GetTextureCoords() const;

	/* Sets the texture coordinates. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetTextureCoordsStruct( const FVector2D& v2TextureCoords );

	/* Sets the texture coordinates. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetTextureCoords( float fU, float fV );

	/* Gets the texture size.  Texture size is added on to the texture coordinate. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual const FVector2D& GetTextureSize() const;

	/* Sets the texture size. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetTextureSizeStruct( const FVector2D& v2TextureSize );

	/* Sets the texture size. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetTextureSize( float fUL, float fVL );

	/* Gets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual const FRotator& GetRotation() const;

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetRotationStruct( const FRotator& rRotation );

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetRotation( float fPitch, float fYaw, float fRoll = 0.f );

	/* Gets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual const FVector2D& GetPivotPoint() const;

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetPivotPointStruct( const FVector2D& v2PivotPoint );

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Texture", BlueprintCallable)
	virtual void SetPivotPoint( float fX, float fY );

	/* Overrides to disable render caching. */
	virtual void EnableRenderCache() override;

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	/* Sets various texture item properties. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	UPROPERTY() // Ensures reference count
	UTexture* tTexture;

	UTexture** tTexturePtr;
	FVector2D v2Size;
	FVector2D v2TextureCoords;
	FVector2D v2TextureSize;
	FRotator rRotation;
	FVector2D v2PivotPoint;
	float fDepth;

	/* Tries to construct a new FCanvasTileItem! */
	virtual void ConstructNewItem() override;

};
