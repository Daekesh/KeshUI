// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterfaceContainer.h"
#include "Component/KUIMaterialInterfaceComponent.h"


UKUIMaterialInterfaceComponent::UKUIMaterialInterfaceComponent( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	mMaterial = NULL;
	mMaterialProxy = NULL;
	v2Size = FVector2D::ZeroVector;
	v2MaterialCoords = FVector2D::ZeroVector;
	v2MaterialSize = FVector2D( 1.f, 1.f );
	rRotation = FRotator::ZeroRotator;
	v2PivotPoint = FVector2D::ZeroVector;
	fDepth = 1.f;
	arTextureParameters.SetNum( 0 );
}


UMaterialInterface* UKUIMaterialInterfaceComponent::GetMaterial() const
{
	return mMaterial;
}


void UKUIMaterialInterfaceComponent::SetMaterial( UMaterialInterface* mMaterial )
{
	if ( this->mMaterial == mMaterial )
		return;

	this->mMaterial = mMaterial;

	Invalidate();

	if ( this->mMaterial != NULL )
		mMaterialProxy = mMaterial->GetRenderProxy( false, false );

	else
		mMaterialProxy = NULL;
}

UTexture* UKUIMaterialInterfaceComponent::GetTextureParameter( const FName& nParameterName ) const
{
	for ( int32 i = 0; i < arTextureParameters.Num(); ++i )
	{
		if ( arTextureParameters[ i ].nParameterName != nParameterName )
			continue;

		return arTextureParameters[ i ].tTextureValue;
	}

	return NULL;
}


void UKUIMaterialInterfaceComponent::SetTextureParameterPointer( const FName& nParameterName, UTexture** tTexturePtr )
{
	int32 iIndex = INDEX_NONE;

	for ( int32 i = 0; i < arTextureParameters.Num(); ++i )
	{
		if ( arTextureParameters[ i ].nParameterName != nParameterName )
			continue;

		iIndex = i;
		break;
	}

	if ( iIndex == INDEX_NONE )
	{
		iIndex = arTextureParameters.Num();
		arTextureParameters.Add( { nParameterName, tTexturePtr, NULL } );
	}

	else
		arTextureParameters[ iIndex ].tTexturePtr = tTexturePtr;

	UMaterialInstanceDynamic* const mDynamic = Cast<UMaterialInstanceDynamic>( mMaterial );

	if ( mDynamic != NULL )
	{
		mDynamic->SetTextureParameterValue( nParameterName, *tTexturePtr );
		arTextureParameters[ iIndex ].tTextureValue = *tTexturePtr;
	}
}


void UKUIMaterialInterfaceComponent::SetTextureParameterName( const FName& nParameterName, const FName& nTextureName )
{
	SetTextureParameterPointer( nParameterName, ( UTexture** ) GetAsset( nTextureName ) );
}


void UKUIMaterialInterfaceComponent::SetMaterialPointer( UMaterialInterface** mMaterialPtr )
{
	if ( this->mMaterialPtr == mMaterialPtr )
		return;

	this->mMaterialPtr = mMaterialPtr;

	if ( this->mMaterial != NULL && mMaterialPtr != NULL && *mMaterialPtr != NULL )
	{
		UMaterialInstance* const mCurrent = Cast<UMaterialInstance>( this->mMaterial );
		UMaterialInstanceDynamic* const mNew = Cast<UMaterialInstanceDynamic>( *mMaterialPtr );

		if ( mCurrent != NULL && mNew != NULL )
			mNew->CopyParameterOverrides( mCurrent );
	}

	SetMaterial( *mMaterialPtr );
}


void UKUIMaterialInterfaceComponent::SetMaterialName( const FName& nMaterialName )
{
	SetMaterialPointer( ( UMaterialInterface** ) GetAsset( nMaterialName ) );
}


FMaterialRenderProxy* UKUIMaterialInterfaceComponent::GetMaterialRenderProxy() const
{
	return mMaterialProxy;
}


float UKUIMaterialInterfaceComponent::GetDepth() const
{
	return fDepth;
}


void UKUIMaterialInterfaceComponent::SetDepth( float fDepth )
{
	if ( this->fDepth == fDepth )
		return;

	this->fDepth = fDepth;

	InvalidateContainerRenderCache();
}


FVector2D UKUIMaterialInterfaceComponent::GetSize() const
{
	return v2Size;
}


void UKUIMaterialInterfaceComponent::SetSizeStruct( const FVector2D& v2Size )
{
	SetSize( v2Size.X, v2Size.Y );
}


void UKUIMaterialInterfaceComponent::SetSize( float fWidth, float fHeight )
{
	if ( v2Size.X == fWidth && v2Size.Y == fHeight )
		return;

	v2Size.X = fWidth;
	v2Size.Y = fHeight;

	Super::SetSize( fWidth, fHeight );
}


FVector2D UKUIMaterialInterfaceComponent::GetMaterialCoords() const
{
	return v2MaterialCoords;
}


void UKUIMaterialInterfaceComponent::SetMaterialCoordsStruct( const FVector2D& v2MaterialCoords )
{
	SetMaterialCoords( v2MaterialCoords.X, v2MaterialCoords.Y );
}


void UKUIMaterialInterfaceComponent::SetMaterialCoords( float fU, float fV )
{
	if ( v2MaterialCoords.X == fU && v2MaterialCoords.Y == fV )
		return;

	v2MaterialCoords.X = fU;
	v2MaterialCoords.Y = fV;

	InvalidateContainerRenderCache();
}


FVector2D UKUIMaterialInterfaceComponent::GetMaterialSize() const
{
	return v2MaterialSize;
}


void UKUIMaterialInterfaceComponent::SetMaterialSizeStruct( const FVector2D& v2MaterialSize )
{
	SetMaterialSize( v2MaterialSize.X, v2MaterialSize.Y );
}


void UKUIMaterialInterfaceComponent::SetMaterialSize( float fUL, float fVL )
{
	if ( v2MaterialSize.X == fUL && v2MaterialSize.Y == fVL )
		return;

	v2MaterialSize.X = fUL;
	v2MaterialSize.Y = fVL;

	InvalidateContainerRenderCache();
}


FRotator UKUIMaterialInterfaceComponent::GetRotation() const
{
	return rRotation;
}


void UKUIMaterialInterfaceComponent::SetRotationStruct( const FRotator& rRotation )
{
	SetRotation( rRotation.Pitch, rRotation.Yaw, rRotation.Yaw );
}


void UKUIMaterialInterfaceComponent::SetRotation( float fPitch, float fYaw, float fRoll )
{
	if ( rRotation.Pitch == fPitch && rRotation.Yaw == fYaw && rRotation.Roll == fRoll )
		return;

	rRotation.Pitch = fPitch;
	rRotation.Yaw = fYaw;
	rRotation.Roll = fRoll;

	InvalidateContainerRenderCache();
}


FVector2D UKUIMaterialInterfaceComponent::GetPivotPoint() const
{
	return v2PivotPoint;
}


void UKUIMaterialInterfaceComponent::SetPivotPointStruct( const FVector2D& v2PivotPoint )
{
	SetPivotPoint( v2PivotPoint.X, v2PivotPoint.Y );
}


void UKUIMaterialInterfaceComponent::SetPivotPoint( float fX, float fY )
{
	if ( v2PivotPoint.X == fX && v2PivotPoint.Y == fY )
		return;

	v2PivotPoint.X = fX;
	v2PivotPoint.Y = fY;

	InvalidateContainerRenderCache();
}


bool UKUIMaterialInterfaceComponent::HasValidComponents() const
{
	if ( v2Size.X <= 0 || v2Size.Y <= 0 )
		return false;

	if ( mMaterialProxy == NULL )
		return false;

	return true;
}


void UKUIMaterialInterfaceComponent::ConstructNewItem()
{
	if ( !HasValidComponents() )
		return;

	stItem.Reset();

	stItem = TSharedPtr<FCanvasTileItem>( new FCanvasTileItem(
		FVector2D::ZeroVector,
		mMaterialProxy,
		v2Size,
		v2MaterialCoords,
		v2MaterialCoords + v2MaterialSize
		) );

	( ( FCanvasTileItem* ) stItem.Get() )->SetColor( GetDrawColor().ReinterpretAsLinear() );
	( ( FCanvasTileItem* ) stItem.Get() )->Rotation = rRotation;
	( ( FCanvasTileItem* ) stItem.Get() )->PivotPoint = v2PivotPoint;
	( ( FCanvasTileItem* ) stItem.Get() )->Z = fDepth;

	Super::ConstructNewItem();
}


void UKUIMaterialInterfaceComponent::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( mMaterialPtr != NULL && *mMaterialPtr != mMaterial )
	{
		if ( this->mMaterial != NULL && mMaterialPtr != NULL && *mMaterialPtr != NULL )
		{
			UMaterialInstance* const mCurrent = Cast<UMaterialInstance>( this->mMaterial );
			UMaterialInstanceDynamic* const mNew = Cast<UMaterialInstanceDynamic>( *mMaterialPtr );

			if ( mCurrent != NULL && mNew != NULL )
				mNew->CopyParameterOverrides( mCurrent );
		}

		SetMaterial( *mMaterialPtr );
	}

	if ( mMaterial != NULL )
	{
		UMaterialInstanceDynamic* const mDynamic = Cast<UMaterialInstanceDynamic>( mMaterial );

		if ( mDynamic != NULL )
		{
			for ( int32 i = 0; i < arTextureParameters.Num(); ++i )
			{
				if ( *( arTextureParameters[ i ].tTexturePtr ) == arTextureParameters[ i ].tTextureValue )
					continue;
					
				mDynamic->SetTextureParameterValue( arTextureParameters[ i ].nParameterName, *( arTextureParameters[ i ].tTexturePtr ) );
				arTextureParameters[ i ].tTextureValue = *( arTextureParameters[ i ].tTexturePtr );
			}
		}
	}

	if ( stItem.IsValid() )
	{
		( ( FCanvasTileItem* ) stItem.Get() )->Size = v2Size;
		( ( FCanvasTileItem* ) stItem.Get() )->UV0 = v2MaterialCoords;
		( ( FCanvasTileItem* ) stItem.Get() )->UV1 = v2MaterialCoords + v2MaterialSize;
		( ( FCanvasTileItem* ) stItem.Get() )->Rotation = rRotation;
		( ( FCanvasTileItem* ) stItem.Get() )->PivotPoint = v2PivotPoint;
		( ( FCanvasTileItem* ) stItem.Get() )->Z = fDepth;
	}

	Super::Render( aHud, oCanvas, v2Origin, oRenderCacheObject );
}


void UKUIMaterialInterfaceComponent::EnableRenderCache()
{
	// Stops the parent method from being called, stopping render caching - It's cannot be used by this type of component.
}
