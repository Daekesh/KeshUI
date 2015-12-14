// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Component/KUICanvasItemInterfaceComponent.h"
#include "KeshUI/KUIMacros.h"
#include "KUIBorderInterfaceComponent.generated.h"

#define KUI_BORDER_NOT_SET -1.f
#define KUI_BORDER_NONE 0.f
#define KUI_BORDER_NO_CHANGE -2.f

UENUM( BlueprintType)
namespace EBCBorderTexture
{
	enum TextureIndex
	{
		TI_Centre = 0      UMETA( DisplayName = "Center Texture" ),
		TI_Left = 1        UMETA( DisplayName = "Left Texture" ),
		TI_Right = 2       UMETA( DisplayName = "Right Texture" ),
		TI_Top = 3         UMETA( DisplayName = "Top Texture" ),
		TI_Bottom = 4      UMETA( DisplayName = "Bottom Texture" ),
		TI_TopLeft = 5     UMETA( DisplayName = "Top Left Texture" ),
		TI_TopRight = 6    UMETA( DisplayName = "Top Right Texture" ),
		TI_BottomLeft = 7  UMETA( DisplayName = "Bottom Left Texture" ),
		TI_BottomRight = 8 UMETA( DisplayName = "Bottom Right Texture" ),
		TI_Max = 9         UMETA( Hidden )
	};
}


/**
 * KeshUI UI Framework (KUI) Border render component.
 */
UCLASS(ClassGroup="KeshUI|Component", Blueprintable, BlueprintType)
class KESHUI_API UKUIBorderInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIBorderInterfaceComponent )

	UKUIBorderInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the indexed texture. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual UTexture2D* GetTexture( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex ) const;

	/* Sets the indexed texture. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetTexture( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, UTexture2D* tTexture );

	/* Sets the indexed texture pointer. */
	virtual void SetTexturePointer( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, UTexture2D** tTexturePtr );

	/* Sets the indexed texture name. */
	UFUNCTION( Category = "KeshUI|Component|Border", BlueprintCallable )
	virtual void SetTextureName( TEnumAsByte<EBCBorderTexture::TextureIndex> eIndex, const FName& nTextureName );

	/* Gets the size of the component. */
	virtual const FVector2D& GetSize() const override;

	/* Sets the size of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetSizeStruct( const FVector2D& v2Size );

	/* Sets the size of the component. */
	virtual void SetSize( float fWidth, float fHeight ) override;

	/* Gets the rotation of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual const FRotator& GetRotation() const;

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetRotationStruct( const FRotator& rRotation );

	/* Sets the rotation of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetRotation( float fPitch, float fYaw, float fRoll = 0.f );

	/* Gets the pivot point of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual const FVector2D& GetPivotPoint() const;

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetPivotPointStruct( const FVector2D& v2PivotPoint );

	/* Sets the pivot point of the component. */
	UFUNCTION(Category="KeshUI|Component|Border", BlueprintCallable)
	virtual void SetPivotPoint( float fX, float fY );

	/* Gets the border metrics. X = Left, Y = Right, Z = Top, W = Bottom. */
	UFUNCTION( Category = "KeshUI|Component|Border", BlueprintCallable )
	virtual const FVector4& GetBorderMetrics() const;

	/* Sets the border metrics. */
	UFUNCTION( Category = "KeshUI|Component|Border", BlueprintCallable )
	virtual void SetBorderMetrics( float fLeft, float fTop, float fRight, float fBottom );

	/* Sets the border metrics. X = Left, Y = Right, Z = Top, W = Bottom. */
	UFUNCTION( Category = "KeshUI|Component|Border", BlueprintCallable )
	virtual void SetBorderMetricsStruct( const FVector4& v4Metrics );

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	/* Sets rotation and pivot point and stuff. */
	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

protected:

	UPROPERTY() // Ensures reference count
	TArray<UTexture2D*> arTextures;
	TArray<UTexture2D**> arTexturePtrs;
	TArray<TSharedPtr<FCanvasTileItem>> arItems;

	FVector2D v2Size;
	FRotator rRotation;
	FVector2D v2PivotPoint; 
	FVector4 v4BorderMetrics;

	/* Tries to construct a new FCanvasBorderItem! */
	virtual void ConstructNewItem() override;

};
