// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KeshUI/Component/KUICanvasItemInterfaceComponent.h"
#include "KeshUI/KUIMacros.h"
#include "KUITriangleListInterfaceComponent.generated.h"

/**
 * KeshUI UI Framework (KUI) TriangleList render component.
 */
UCLASS(ClassGroup="KeshUI|Component", Blueprintable, BlueprintType)
class KESHUI_API UKUITriangleListInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUITriangleListInterfaceComponent )

	UKUITriangleListInterfaceComponent( const class FObjectInitializer& oObjectInitializer );

public:

	/* Gets the current number of triangles. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	int32 GetTriangleCountBP() const { return GetTriangleCount(); }

	virtual int16 GetTriangleCount() const;

	/* Sets the current number of triangles. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	void SetTriangleCountBP( int32 iTriangleCount ) { SetTriangleCount( iTriangleCount ); }

	virtual void SetTriangleCount( int16 );

	/* Gets a triangle. */
	UFUNCTION( Category = "KeshUI|Component|Triangle List", BlueprintCallable )
	virtual const FCanvasUVTri& GetTriangleBP( int32 iIndex ) const { return GetTriangle( iIndex ); }

	virtual const FCanvasUVTri& GetTriangle( int32 iIndex ) const;

	/* Adds a triangle. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	virtual void AddTriangleStruct( const FCanvasUVTri& stTriangle );

	/* Adds a triangle. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	virtual void AddTriangle( FVector2D v2ScreenPointA, FVector2D v2ScreenPointB, FVector2D v2ScreenPointC, 
		FVector2D v2TexturePointA = FVector2D::ZeroVector, 
		FVector2D v2TexturePointB = FVector2D::ZeroVector, 
		FVector2D v2TexturePointC = FVector2D::ZeroVector,
		FLinearColor lcPointA = FLinearColor::White,
		FLinearColor lcPointB = FLinearColor::White,
		FLinearColor lcPointC = FLinearColor::White );

	/* Sets a triangle. */
	UFUNCTION( Category = "KeshUI|Component|Triangle List", BlueprintCallable )
	virtual void SetTriangleStruct( int32 iIndex, const FCanvasUVTri& stTriangle );

	/* Sets a triangle. */
	UFUNCTION( Category = "KeshUI|Component|Triangle List", BlueprintCallable )
	virtual void SetTriangle( int32 iIndex, 
		FVector2D v2ScreenPointA, FVector2D v2ScreenPointB, FVector2D v2ScreenPointC,
		FVector2D v2TexturePointA = FVector2D::ZeroVector,
		FVector2D v2TexturePointB = FVector2D::ZeroVector,
		FVector2D v2TexturePointC = FVector2D::ZeroVector,
		FLinearColor lcPointA = FLinearColor::White,
		FLinearColor lcPointB = FLinearColor::White,
		FLinearColor lcPointC = FLinearColor::White );

	/* Gets the texture. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	virtual UTexture* GetTexture() const;

	/* Sets the texture. Sets the screen size to match this if it's zero. */
	UFUNCTION(Category="KeshUI|Component|Triangle List", BlueprintCallable)
	virtual void SetTexture( UTexture* tTexture );

	/* Gets the size of this component. */
	virtual const FVector2D& GetSize() const override;

	/* Returns true if this can have its location changed. */
	virtual bool IsPositionable() const override;

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

protected:

	FVector4 v4Bounds;
	FVector2D v2Size;
	TArray<FCanvasUVTri> arTriangles;

	UPROPERTY() // Ensures reference count
	UTexture* tTexture;

	/* Tries to construct a new FCanvasTriangleListItem! */
	virtual void ConstructNewItem() override;

};
