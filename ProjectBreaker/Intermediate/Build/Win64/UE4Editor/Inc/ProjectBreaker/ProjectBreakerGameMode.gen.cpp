// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProjectBreaker/ProjectBreakerGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProjectBreakerGameMode() {}
// Cross Module References
	PROJECTBREAKER_API UClass* Z_Construct_UClass_AProjectBreakerGameMode_NoRegister();
	PROJECTBREAKER_API UClass* Z_Construct_UClass_AProjectBreakerGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_ProjectBreaker();
// End Cross Module References
	void AProjectBreakerGameMode::StaticRegisterNativesAProjectBreakerGameMode()
	{
	}
	UClass* Z_Construct_UClass_AProjectBreakerGameMode_NoRegister()
	{
		return AProjectBreakerGameMode::StaticClass();
	}
	struct Z_Construct_UClass_AProjectBreakerGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AProjectBreakerGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_ProjectBreaker,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AProjectBreakerGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "ProjectBreakerGameMode.h" },
		{ "ModuleRelativePath", "ProjectBreakerGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AProjectBreakerGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AProjectBreakerGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AProjectBreakerGameMode_Statics::ClassParams = {
		&AProjectBreakerGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_AProjectBreakerGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AProjectBreakerGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AProjectBreakerGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AProjectBreakerGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AProjectBreakerGameMode, 463780855);
	template<> PROJECTBREAKER_API UClass* StaticClass<AProjectBreakerGameMode>()
	{
		return AProjectBreakerGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AProjectBreakerGameMode(Z_Construct_UClass_AProjectBreakerGameMode, &AProjectBreakerGameMode::StaticClass, TEXT("/Script/ProjectBreaker"), TEXT("AProjectBreakerGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AProjectBreakerGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
