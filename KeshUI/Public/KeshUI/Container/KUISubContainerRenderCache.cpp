// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#include "KeshUI/KeshUI.h"
#include "KeshUI/Container/KUISubContainer.h"
#include "KeshUI/KUIInterface.h"
#include "KeshUI/Container/KUISubContainerRenderCache.h"


UKUISubContainerRenderCache::UKUISubContainerRenderCache( const class FObjectInitializer& oObjectInitializer )
: Super( oObjectInitializer )
{

}


void UKUISubContainerRenderCache::UpdateRenderCache( UKUIInterfaceElement* oElement )
{
	if ( oElement == NULL )
	{
		KUIErrorUO( "Null element" );
		return;
	}

	KUILogUO( "Updating Render Cache" );

	UKUISubContainer* const ctSub = Cast<UKUISubContainer>( oElement );
	const FVector2D v2ElemSize = ctSub->GetTotalSize();

	if ( v2ElemSize.X < 1.f || v2ElemSize.Y < 1.f )
	{
		KUIErrorUO( "Element is zero size" );
		return;
	}

	bool bRebuildTexture = false;

	if ( GetTexture() == NULL )
		bRebuildTexture = true;

	else if ( v2ElemSize.X != GetTexture()->GetSurfaceWidth() || v2ElemSize.Y != GetTexture()->GetSurfaceHeight() )
		bRebuildTexture = true;

	if ( bRebuildTexture )
		CreateRenderCache( v2ElemSize );

	if ( GetTexture() == NULL )
	{
		KUIErrorUO( "Texture is null" );
		return;
	}

	if ( !GetTexture()->IsA<UTextureRenderTarget2D>() )
	{
		KUIErrorUO( "Texture is not a render target" );
		return;
	}

	UTextureRenderTarget2D* const tRenderTarget = Cast<UTextureRenderTarget2D>( GetTexture() );
	tRenderTarget->UpdateResource();
	tRenderTarget->UpdateResourceImmediate();

	UCanvas* uoCanvas = Cast<UCanvas>( StaticFindObjectFast( UCanvas::StaticClass(), GetTransientPackage(), FName( TEXT( "Sub Container Render Cache Canvas" ) ) ) );

	if ( uoCanvas == NULL )
	{	
		uoCanvas = NewObject<UCanvas>( GetTransientPackage(), FName( TEXT( "Sub Container Render Cache Canvas" ) ) );
		uoCanvas->AddToRoot();
	}

	uoCanvas->Init( floor( v2ElemSize.X ), floor( v2ElemSize.Y ), NULL );
	uoCanvas->Update();

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		CanvasRenderTargetMakeCurrentCommand,
		FTextureRenderTarget2DResource*,
		TextureRenderTarget,
		static_cast<FTextureRenderTarget2DResource*>( tRenderTarget->GameThread_GetRenderTargetResource() ),
		{
			SetRenderTarget( RHICmdList, TextureRenderTarget->GetRenderTargetTexture(), FTexture2DRHIRef() );
			RHICmdList.SetViewport( 0, 0, 0.0f, TextureRenderTarget->GetSizeXY().X, TextureRenderTarget->GetSizeXY().Y, 1.0f );
		}
	)

	FCanvas oCanvas( tRenderTarget->GameThread_GetRenderTargetResource(), NULL, GetWorld(), GetInterface()->GetWorld()->FeatureLevel );
	uoCanvas->Canvas = &oCanvas;
	oCanvas.Clear( tRenderTarget->ClearColor );

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
		CanvasRenderTargetResolveCommand,
		FTextureRenderTargetResource*,
		RenderTargetResource,
		static_cast<FTextureRenderTarget2DResource*>( tRenderTarget->GameThread_GetRenderTargetResource() ),
		{
			RHICmdList.CopyToResolveTarget( RenderTargetResource->GetRenderTargetTexture(), RenderTargetResource->TextureRHI, true, FResolveParams() );
		}
	)

	bValidRenderCache = true;
}
