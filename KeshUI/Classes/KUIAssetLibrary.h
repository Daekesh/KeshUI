// Copyright 2014-2015 Matt Chapman. All Rights Reserved.

#pragma once

#include "KUIMacros.h"
#include "KUIAssetLibrary.generated.h"

USTRUCT( BlueprintType )
struct FKUIAssetLibraryEntry
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Asset Name" ) )
	FName nAssetName;

	UPROPERTY( Category = "KeshUI", EditInstanceOnly, BlueprintReadWrite, Meta = ( DisplayName = "Asset Reference" ) )
	UObject* oAsset;
};


/**
* Asset registry for creating lists of async-loading assets.
*/
UCLASS( ClassGroup = "KeshUI", Blueprintable, BlueprintType )
class KESHUI_API UKUIAssetLibrary : public UObject
{
	GENERATED_BODY()
	KUI_CLASS_HEADER( UKUIAssetLibrary )

	UKUIAssetLibrary( const class FObjectInitializer& oObjectInitializer );

public:

	virtual UObject** GetAsset( const FName& nName ) const;

	template<class T>
	FORCEINLINE T** GetAsset( const FName& nName ) const
	{
		return reinterpret_cast<T**>( GetAsset( nName ) );
	}

protected:

	UPROPERTY( Category = "KeshUI", EditAnywhere, BlueprintReadWrite, Instanced, Meta = ( DisplayName = "Parent Library" ) )
	UKUIAssetLibrary* oParentLibrary;

	UPROPERTY( Category = "KeshUI", EditAnywhere, BlueprintReadWrite, Instanced, Meta = ( DisplayName = "Child Library" ) )
	UKUIAssetLibrary* oChildLibrary;
	
	UPROPERTY( Category = "KeshUI", EditAnywhere, BlueprintReadWrite, Meta = ( DisplayName = "Asset Library" ) )
	TArray<FKUIAssetLibraryEntry> arAssetLibrary;

};
