// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI.h"
#include "KUIInterface.h"
#include "KUIRenderCache.h"


UKUIRenderCache::UKUIRenderCache( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{
	bValidRenderCache = false;
}


void UKUIRenderCache::DestroyRenderCache()
{
	SetTexture( NULL );
	bValidRenderCache = false;
}


void UKUIRenderCache::CreateRenderCache( const FVector2D& v2Size )
{
	DestroyRenderCache();

	UTextureRenderTarget2D* const tRenderTarget = NewObject<UTextureRenderTarget2D>( this );
	tRenderTarget->bNeedsTwoCopies = false;
	tRenderTarget->InitAutoFormat( floor( v2Size.X ), floor( v2Size.Y ) );
	tRenderTarget->ClearColor = FLinearColor::Transparent;

	SetTexture( tRenderTarget );
}


void UKUIRenderCache::UpdateRenderCache( UKUIInterfaceElement* oElement )
{
	if ( oElement == NULL )
	{
		KUIErrorUO( "Trying to update render cache of null element" );
		return;
	}

	const FVector2D v2ElemSize = oElement->GetSize();

	if ( v2ElemSize.X < 1.f || v2ElemSize.Y < 1.f )
	{
		KUIErrorUO( "Trying to update render cache of 0 size element" );
		return;
	}

	const FVector2D v2MySize = GetSize();

	if ( v2ElemSize.X != v2MySize.X || v2ElemSize.Y != v2MySize.Y )
	{
		if ( v2ElemSize.X < 1.f || v2ElemSize.Y < 1.f )
		{
			KUIErrorUO( "New element size is 0" );
			return;
		}

		SetSizeStruct( v2ElemSize );
		CreateRenderCache( v2ElemSize );
	}

	if ( GetTexture() == NULL )
	{
		KUIErrorUO( "Null texture" );
		return;
	}

	if ( !GetTexture()->IsA<UTextureRenderTarget2D>() )
	{
		KUIErrorUO( "Texture is not a texture render target" );
		return;
	}

	UTextureRenderTarget2D* const tRenderTarget = Cast<UTextureRenderTarget2D>( GetTexture() );
	tRenderTarget->UpdateResource();
	tRenderTarget->UpdateResourceImmediate();

	UCanvas* uoCanvas = Cast<UCanvas>( StaticFindObjectFast( UCanvas::StaticClass(), GetTransientPackage(), FName( TEXT( "Render Cache Canvas" ) ) ) );

	if ( uoCanvas == NULL )
	{
		uoCanvas = ConstructObject<UCanvas>( UCanvas::StaticClass(), GetTransientPackage(), FName( TEXT( "Render Cache Canvas" ) ) );
		uoCanvas->AddToRoot();
	}

	uoCanvas->Init( floor( v2Size.X ), floor( v2Size.Y ), NULL );
	uoCanvas->Update();

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		RenderCacheRenderTargetMakeCurrentCommand,
		FTextureRenderTarget2DResource*,
		TextureRenderTarget,
		static_cast< FTextureRenderTarget2DResource*>( tRenderTarget->GameThread_GetRenderTargetResource() ),
		{
			SetRenderTarget( RHICmdList, TextureRenderTarget->GetRenderTargetTexture(), FTexture2DRHIRef() );
			RHICmdList.SetViewport( 0, 0, 0.0f, TextureRenderTarget->GetSizeXY().X, TextureRenderTarget->GetSizeXY().Y, 1.0f );
		}
	)


	FCanvas oCanvas( tRenderTarget->GameThread_GetRenderTargetResource(), NULL, GetWorld(), GetInterface()->GetWorld()->FeatureLevel );
	uoCanvas->Canvas = &oCanvas;
	oCanvas.Clear( tRenderTarget->ClearColor );

	KUILogDebugUO( "Updating render cache" );
	oElement->Render( GetInterface(), uoCanvas, FVector2D::ZeroVector, oElement );

	uoCanvas->Canvas = NULL;
	
	if ( IsInGameThread() )
		oCanvas.Flush_GameThread();

	else if ( IsInRenderingThread() )
	{
		FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();
		oCanvas.Flush_RenderThread( RHICmdList );
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		RenderCacheRenderTargetResolveCommand,
		FTextureRenderTargetResource*,
		RenderTargetResource,
		static_cast< FTextureRenderTarget2DResource*>( tRenderTarget->GameThread_GetRenderTargetResource() ),
		{
			RHICmdList.CopyToResolveTarget( RenderTargetResource->GetRenderTargetTexture(), RenderTargetResource->TextureRHI, true, FResolveParams() );
		}
	)

	bValidRenderCache = true;
}


bool UKUIRenderCache::IsRenderCacheValid() const
{
	return bValidRenderCache;
}


void UKUIRenderCache::InvalidateRenderCache()
{
	bValidRenderCache = false;
}


void UKUIRenderCache::Render( AKUIInterface* aHud, UCanvas* oCanvas, const FVector2D& v2Origin, UKUIInterfaceElement* oRenderCacheObject )
{
	if ( GetTexture() == NULL )
	{
		KUIErrorUO( "Null texture" );
		return;
	}

	if ( !GetTexture()->IsA<UTextureRenderTarget2D>() )
	{
		KUIErrorUO( "Texture is not a render target" );
		return;
	}

	UTextureRenderTarget2D* const tRenderTarget = Cast<UTextureRenderTarget2D>( GetTexture() );

	oCanvas->Canvas->DrawTile(
		v2Origin.X,
		v2Origin.Y,
		GetSize().X,
		GetSize().Y,
		GetTextureCoords().X,
		GetTextureCoords().Y,
		GetTextureCoords().X + GetTextureSize().X,
		GetTextureCoords().Y + GetTextureSize().Y,
		GetDrawColor().ReinterpretAsLinear(),
		tRenderTarget->Resource,
		true
	);
}


void UKUIRenderCache::InvalidateContainerRenderCache()
{

}
