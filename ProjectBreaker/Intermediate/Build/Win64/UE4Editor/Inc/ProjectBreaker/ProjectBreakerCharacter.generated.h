// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef PROJECTBREAKER_ProjectBreakerCharacter_generated_h
#error "ProjectBreakerCharacter.generated.h already included, missing '#pragma once' in ProjectBreakerCharacter.h"
#endif
#define PROJECTBREAKER_ProjectBreakerCharacter_generated_h

#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_SPARSE_DATA
#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_RPC_WRAPPERS
#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAProjectBreakerCharacter(); \
	friend struct Z_Construct_UClass_AProjectBreakerCharacter_Statics; \
public: \
	DECLARE_CLASS(AProjectBreakerCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/ProjectBreaker"), NO_API) \
	DECLARE_SERIALIZER(AProjectBreakerCharacter)


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAProjectBreakerCharacter(); \
	friend struct Z_Construct_UClass_AProjectBreakerCharacter_Statics; \
public: \
	DECLARE_CLASS(AProjectBreakerCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/ProjectBreaker"), NO_API) \
	DECLARE_SERIALIZER(AProjectBreakerCharacter)


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AProjectBreakerCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AProjectBreakerCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AProjectBreakerCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AProjectBreakerCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AProjectBreakerCharacter(AProjectBreakerCharacter&&); \
	NO_API AProjectBreakerCharacter(const AProjectBreakerCharacter&); \
public:


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AProjectBreakerCharacter(AProjectBreakerCharacter&&); \
	NO_API AProjectBreakerCharacter(const AProjectBreakerCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AProjectBreakerCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AProjectBreakerCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AProjectBreakerCharacter)


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AProjectBreakerCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(AProjectBreakerCharacter, FollowCamera); }


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_9_PROLOG
#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_SPARSE_DATA \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_RPC_WRAPPERS \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_INCLASS \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_SPARSE_DATA \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_INCLASS_NO_PURE_DECLS \
	ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> PROJECTBREAKER_API UClass* StaticClass<class AProjectBreakerCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ProjectBreaker_Source_ProjectBreaker_ProjectBreakerCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
