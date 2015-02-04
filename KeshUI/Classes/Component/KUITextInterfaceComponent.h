// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "Component/KUICanvasItemInterfaceComponent.h"
#include "KUIMacros.h"
#include "KUITextInterfaceComponent.generated.h"

/**
 * KeshUI UI Framework (KUI) Text render component.
 */
UCLASS(ClassGroup="KeshUI | Component", Blueprintable, BlueprintType)
class KESHUI_API UKUITextInterfaceComponent : public UKUICanvasItemInterfaceComponent
{
	GENERATED_UCLASS_BODY()
	KUI_CLASS_HEADER( UKUITextInterfaceComponent )

public:

	/* Gets the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FText GetText() const;

	/* Gets the text as a string. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FString GetTextString() const;

	/* Sets the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetText( const FText& txText );

	/* Sets the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetTextString( const FString& sString );

	/* Gets the font. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual UFont* GetFont() const;

	/* Sets the font. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetFont( UFont* foFont );

	/* Sets the font by pointer. */
	virtual void SetFontPointer( UFont** foFontPtr );

	/* Sets the font by asset name. */
	UFUNCTION( Category = "KeshUI | Component | Text", BlueprintCallable )
	virtual void SetFontName( const FName& nFontName );

	/* Is text centred horizontally? */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool IsCenteredHorizontally() const;

	/* Sets whether text is centred horizontally. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetCentredHorizontally( bool bCentred );

	/* Is text centred vertically? */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool IsCenteredVertically() const;

	/* Sets whether text is centred vertically. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetCentredVertically( bool bCentred );

	/* Gets the scale of the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FVector2D GetScale() const;

	/* Sets the scale of the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetScaleStruct( const FVector2D& v2Scale );

	/* Sets the scale of the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetScale( float fX, float fY = 1.f );

	/* Gets the depth of the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual float GetDepth() const;

	/* Sets the depth of the text. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetDepth( float fDepth );

	/* Returns true if the text has an outline. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool IsOutlined() const;

	/* Sets whether the text is outlined. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetOutlined( bool bOutlined );

	/* Gets the outline color. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FLinearColor GetOutlineColor() const;

	/* Sets the outline color. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetOutlineColor( const FLinearColor& lcColor );

	/* Gets the shadow offset. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FVector2D GetShadowOffset() const;

	/* Sets the shadow offset. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetShadowOffsetStruct( const FVector2D& v2Offset );

	/* Sets the shadow offset. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetShadowOffset( float fX, float fY = 0.f );

	/* Gets the shadow color. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FLinearColor GetShadowColor() const;

	/* Sets the shadow color. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetShadowColor( const FLinearColor& lcColor );

	/* Returns true if the text has a shadow. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool HasShadow() const;

	/* Sets whether the text is shadowed. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetShadow( bool bShadow );

	/* Gets whether the text is clipped. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool IsClipped() const;

	/* Sets whether the text is clipped. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetClipped( bool bClipped );

	/* Gets the horizontal spacing adjustment. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual float GetHorizontalSpacingAdjustment() const;

	/* Sets the horizontal spacing adjustment. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetHorizontalSpacingAdjustment( float fSpacing );

	/* Gets whether stereoscopic correction is on. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual bool IsCorrectingStereoscopic() const;

	/* Sets whether stereoscopic correction is on. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual void SetCorrectStereoscopic( bool bCorrect );

	/* Returns the font render info. */
	UFUNCTION(Category="KeshUI | Component | Text", BlueprintCallable)
	virtual FFontRenderInfo GetFontRenderInfo();

	/* Gets the size of this component. */
	virtual FVector2D GetSize() const override;

	/* Gets the size of the given string using this text component's settings. */
	UFUNCTION( Category = "KeshUI | Component | Text", BlueprintCallable )
	virtual FVector2D GetSizeText( const FText& txText ) const;

	/* Gets the size of the given string using this text component's settings. */
	UFUNCTION( Category = "KeshUI | Component | Text", BlueprintCallable )
	virtual FVector2D GetSizeString( const FString& strString ) const;

	/* Returns true if there's enough information to render. */
	virtual bool HasValidComponents() const override;

	virtual void Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject = NULL ) override;

	/* Splits a string up into an array of strings with the given parameters. */
	static TArray<FString> SplitString( const FString& strString, UFont* foFont, float fMaxWidth, const FString& strIndentString );

protected:

	FText txText;

	UPROPERTY() // Ensures reference count
	UFont* foFont;

	UFont** foFontPtr;
	bool bCentreX;
	bool bCentreY;
	FVector2D v2Scale;
	float fDepth;
	bool bOutlined;
	FLinearColor lcOutlineColor;
	bool bShadow;
	bool bUpdateShadowInfo;
	FVector2D v2ShadowOffset;
	FLinearColor lcShadowColor;
	bool bClipped;
	float fHorizontalSpacingAdjust;
	bool bDontCorrectStereoscopic;

	/* Tries to construct a new FCanvasTextItem! */
	virtual void ConstructNewItem() override;

};
