// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Component/KUICanvasItemInterfaceComponent.h"
#include "KUIMacros.h"
#include "KUIBorderInterfaceComponent.generated.h"

#define KUI_BORDER_NOT_SET -1.f
#define KUI_BORDER_NONE 0.f
#define KUI_BORDER_NO_CHANGE -2.f

/**
 * KeshUI UI Framework (KUI) Border render component.
 */
UCLASS(ClassGroup="KeshUI | Component", Blueprintable, BlueprintType)
class KESHUI_API UKUIBorderInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUIBorderInterfaceComponent )

public:

	/* Gets the background texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetBackgroundTexture() const;

	/* Sets the background texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetBackgroundTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetBackgroundTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBackgroundTextureName( const FName& nTexture );

	/* Gets the left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetLeftBorderTexture() const;

	/* Sets the left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetLeftBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetLeftBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetLeftBorderTextureName( const FName& nTexture );

	/* Gets the right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetRightBorderTexture() const;

	/* Sets the right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetRightBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetRightBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetRightBorderTextureName( const FName& nTexture );

	/* Gets the top border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetTopBorderTexture() const;

	/* Sets the top border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetTopBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetTopBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetTopBorderTextureName( const FName& nTexture );

	/* Gets the bottom border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetBottomBorderTexture() const;

	/* Sets the bottom border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetBottomBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetBottomBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBottomBorderTextureName( const FName& nTexture );

	/* Gets the top left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetTopLeftBorderTexture() const;

	/* Sets the top left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetTopLeftBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetTopLeftBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetTopLeftBorderTextureName( const FName& nTexture );

	/* Gets the top right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetTopRightBorderTexture() const;

	/* Sets the top right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetTopRightBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetTopRightBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetTopRightBorderTextureName( const FName& nTexture );

	/* Gets the bottom left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetBottomLeftBorderTexture() const;

	/* Sets the bottom left border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetBottomLeftBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetBottomLeftBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBottomLeftBorderTextureName( const FName& nTexture );

	/* Gets the bottom right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual UTexture2D* GetBottomRightBorderTexture() const;

	/* Sets the bottom right border texture. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetBottomRightBorderTexture( UTexture2D* tTexture );

	/* Sets the background texture pointer. */
	virtual void SetBottomRightBorderTexturePointer( UTexture2D** tTexture );

	/* Sets the background texture name. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBottomRightBorderTextureName( const FName& nTexture );

	/* Gets the size of the component. */
	virtual FVector2D GetSize() const override;

	/* Sets the size of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the component. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual FRotator GetRotation() const;

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetRotationStruct( const FRotator& rRotation );

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetRotation( float fPitch, float fYaw, float fRoll = 0.f );

	/* Gets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual FVector2D GetPivotPoint() const;

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetPivotPointStruct( const FVector2D& v2PivotPoint );

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI | Component | Border", BlueprintCallable)
	virtual void SetPivotPoint( float fX, float fY );

	/* Gets the border metrics. X = Left, Y = Right, Z = Top, W = Bottom. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual FVector4 GetBorderMetrics() const;

	/* Sets the border metrics. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBorderMetrics( float fLeft, float fRight, float fTop, float fBottom );

	/* Sets the border metrics. X = Left, Y = Right, Z = Top, W = Bottom. */
	UFUNCTION( Category = "KeshUI | Component | Border", BlueprintCallable )
	virtual void SetBorderMetricsStruct( const FVector4& v4Metrics );

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	/* Sets rotation and pivot point and stuff. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBackground;
	UTexture2D** tTextureBackgroundPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderLeft;
	UTexture2D** tTextureBorderLeftPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderRight;
	UTexture2D** tTextureBorderRightPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderTop;
	UTexture2D** tTextureBorderTopPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderBottom;
	UTexture2D** tTextureBorderBottomPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderTopLeft;
	UTexture2D** tTextureBorderTopLeftPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderTopRight;
	UTexture2D** tTextureBorderTopRightPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderBottomLeft;
	UTexture2D** tTextureBorderBottomLeftPtr;

	UPROPERTY() // Ensures reference count
	UTexture2D* tTextureBorderBottomRight;
	UTexture2D** tTextureBorderBottomRightPtr;

	FVector2D v2Size;
	FRotator rRotation;
	FVector2D v2PivotPoint; 
	FVector4 v4BorderMetrics;

	/* Tries to construct a new FCanvasBorderItem! */
	virtual void ConstructNewItem() override;

};
