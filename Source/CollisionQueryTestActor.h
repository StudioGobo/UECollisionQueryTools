// ----------------------------------------------------------------------------
// Copyright (c) Studio Gobo Ltd 2023
// Licensed under the MIT license.  
// See LICENSE.TXT in the project root for license information.
// ----------------------------------------------------------------------------
// File			-> CollisionQueryTestActor.h
// Created		-> October 2023
// Author		-> George Prosser (Studio Gobo)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"

#include "CollisionQueryTestActor.generated.h"

UENUM()
enum class ECollisionQueryTestType : uint8
{
	LineTrace,
	Sweep,
	Overlap
};

UENUM()
enum class ECollisionQueryTestSingleMultiOrTest : uint8
{
	Single,
	Multi,
	Test
};

UENUM()
enum class ECollisionQueryTestBlockingAnyOrMulti : uint8
{
	BlockingTest,
	AnyTest,
	Multi
};

UENUM()
enum class ECollisionQueryTestShape : uint8
{
	Box,
	Sphere,
	Capsule
};

UENUM()
enum class ECollisionQueryTestBy : uint8
{
	Channel,
	ObjectType,
	Profile
};

UENUM()
enum class ECollisionQueryTestMobilityType : uint8
{
	Any,
	Static,
	Dynamic
};

/**
 * Test actor that performs a custom line trace/sweep/overlap test on tick and draws the result.
 */
UCLASS(HideCategories=(Rendering, Replication, Collision, HLOD, Physics, Networking, Input, Actor, Cooking))
class ACollisionQueryTestActor : public AActor
{
	GENERATED_BODY()

public:
	ACollisionQueryTestActor(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;


	UPROPERTY(EditAnywhere)
	ECollisionQueryTestType Query = ECollisionQueryTestType::LineTrace;

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::Overlap", EditConditionHides))
	ECollisionQueryTestSingleMultiOrTest SingleMultiOrTest = ECollisionQueryTestSingleMultiOrTest::Single;

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query==ECollisionQueryTestType::Overlap", EditConditionHides))
	ECollisionQueryTestBlockingAnyOrMulti BlockingAnyOrMulti = ECollisionQueryTestBlockingAnyOrMulti::Multi;

	UPROPERTY(EditAnywhere)
	ECollisionQueryTestBy By = ECollisionQueryTestBy::Channel;
	

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::LineTrace", EditConditionHides))
	ECollisionQueryTestShape Shape = ECollisionQueryTestShape::Capsule;

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::LineTrace&&Shape==ECollisionQueryTestShape::Box", EditConditionHides))
	FVector BoxHalfExtent = { 25, 25, 50.f };

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::LineTrace&&Shape==ECollisionQueryTestShape::Sphere", EditConditionHides))
	float SphereRadius = 40.f;

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::LineTrace&&Shape==ECollisionQueryTestShape::Capsule", EditConditionHides))
	float CapsuleRadius = 40.f;

	UPROPERTY(EditAnywhere, meta=(EditCondition="Query!=ECollisionQueryTestType::LineTrace&&Shape==ECollisionQueryTestShape::Capsule", EditConditionHides))
	float CapsuleHalfHeight = 80.f;

	UPROPERTY(EditAnywhere, meta=(EditCondition="By==ECollisionQueryTestBy::Channel", EditConditionHides))
	TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Pawn;

	UPROPERTY(EditAnywhere, meta=(EditCondition="By==ECollisionQueryTestBy::ObjectType", EditConditionHides))
	TEnumAsByte<EObjectTypeQuery> ObjectType = EObjectTypeQuery::ObjectTypeQuery1;

	UPROPERTY(EditAnywhere, meta=(EditCondition="By==ECollisionQueryTestBy::Profile", EditConditionHides, GetOptions="GetCollisionProfileOptions"))
	FName CollisionProfileName = TEXT("BlockAll");

	UPROPERTY(EditAnywhere, meta=(EditCondition="By==ECollisionQueryTestBy::Channel", EditConditionHides))
	FCollisionResponseContainer CollisionResponses;


	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bTraceComplex = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bFindInitialOverlaps = true;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bIgnoreBlocks = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bIgnoreTouches = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bSkipNarrowPhase = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	ECollisionQueryTestMobilityType MobilityType = ECollisionQueryTestMobilityType::Any;

	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> StartComponent = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> EndComponent = nullptr;

	
	UFUNCTION()
	TArray<FName> GetCollisionProfileOptions() const;

	static EQueryMobilityType ConvertToQueryMobilityType(ECollisionQueryTestMobilityType TestType);

};