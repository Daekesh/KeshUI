// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Component/KUICanvasItemInterfaceComponent.h"
#include "KeshUI/KUIMacros.h"
#include "KUIMaterialInterfaceComponent.generated.h"

struct FMaterialTextureParameterPointer
{
	FName nParameterName;
	UTexture** tTexturePtr;
	UTexture* tTextureValue;
};


/**
* KeshUI UI Framework (KUI) Material render component.
*/
UCLASS( ClassGroup = "KeshUI | Component", Blueprintable, BlueprintType )
class KESHUI_API UKUIMaterialInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIMaterialInterfaceComponent )

	UKUIMaterialInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the material. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual UMaterialInterface* GetMaterial() const;

	/* Sets the material. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterial( UMaterialInterface* mMaterial );

	/* Sets the material by pointer. */
	virtual void SetMaterialPointer( UMaterialInterface** mMaterialPtr );

	/* Sets the material by asset name. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterialName( const FName& nMaterialName );

	/* Returns the material with the given parameter name. */
	virtual UTexture* GetTextureParameter( const FName& nParameterName ) const;

	/* Sets the material texture parameter by pointer. */
	virtual void SetTextureParameterPointer( const FName& nParameterName, UTexture** tTexturePtr );

	/* Sets the material texture parameter by name. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetTextureParameterName( const FName& nParameterName, const FName& nTextureName );

	/* Gets the material render proxy... in case you need it. */
	virtual FMaterialRenderProxy* GetMaterialRenderProxy() const;

	/* Gets the depth of the material from the screen. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual float GetDepth() const;

	/* Sets the material location. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetDepth( float fDepth );

	/* Gets the size of material on the screen (at 0 depth.) */
	virtual const FVector2D& GetSize() const override;

	/* Sets the size of the material on the screen (at 0 depth.) */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the material on the screen (at 0 depth.) */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the material coordinates. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual const FVector2D& GetMaterialCoords() const;

	/* Sets the material coordinates. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterialCoordsStruct( const FVector2D& v2MaterialCoords );

	/* Sets the material coordinates. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterialCoords( float fU, float fV );

	/* Gets the material size.  Material size is added on to the material coordinate. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual const FVector2D& GetMaterialSize() const;

	/* Sets the material size. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterialSizeStruct( const FVector2D& v2MaterialSize );

	/* Sets the material size. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetMaterialSize( float fUL, float fVL );

	/* Gets the rotation of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual const FRotator& GetRotation() const;

	/* Sets the rotation of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetRotationStruct( const FRotator& rRotation );

	/* Sets the rotation of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetRotation( float fPitch, float fYaw, float fRoll = 0.f );

	/* Gets the pivot point of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual const FVector2D& GetPivotPoint() const;

	/* Sets the pivot point of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetPivotPointStruct( const FVector2D& v2PivotPoint );

	/* Sets the pivot point of the component. */
	UFUNCTION( Category = "KeshUI | Component | Material", BlueprintCallable )
	virtual void SetPivotPoint( float fX, float fY );

	/* Overrides to disable render caching. */
	virtual void EnableRenderCache() override;

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	/* Sets various material item properties. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	UPROPERTY() // Ensures reference count
	UMaterialInterface* mMaterial;

	TArray<FMaterialTextureParameterPointer> arTextureParameters;

	UMaterialInterface** mMaterialPtr;
	FMaterialRenderProxy* mMaterialProxy;
	FVector2D v2Size;
	FVector2D v2MaterialCoords;
	FVector2D v2MaterialSize;
	FRotator rRotation;
	FVector2D v2PivotPoint;
	float fDepth;

	/* Tries to construct a new FCanvasTileItem! */
	virtual void ConstructNewItem() override;

};
