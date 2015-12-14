// Copyright 2014-2015 Matthew Chapman, Inc. All Rights Reserved.

#pragma once

#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "KUIMacros.h"
#include "KUIGameInstance.generated.h"

class UKUIAssetLibrary;

/**
* Game instance for various misc functions.
*/
UCLASS( ClassGroup = "KeshUI|Game", Blueprintable, BlueprintType )
class KESHUI_API UKUIGameInstance : public UGameInstance
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIGameInstance )

	UKUIGameInstance( const class FObjectInitializer& oObjectInitializer );

public:

	/* Returns the asset identified by the given FName from the asset libraries. */
	virtual UObject** GetAsset( const FName& nName ) const;

	template<class T>
	FORCEINLINE T** GetAsset( const FName& nName ) const
	{
		return reinterpret_cast< T** >( GetAsset( nName ) );
	}

protected:

	UPROPERTY( Category = "KeshUI|Interface", EditAnywhere, BlueprintReadWrite, Instanced, Meta = ( DisplayName = "Asset Libraries" ) )
	TArray<UKUIAssetLibrary*> arAssetLibraries;

};
