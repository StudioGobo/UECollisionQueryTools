// ----------------------------------------------------------------------------
// Copyright (c) Studio Gobo Ltd 2023
// Licensed under the MIT license.  
// See LICENSE.TXT in the project root for license information.
// ----------------------------------------------------------------------------
// File			-> CollisionQueryTestActor.cpp
// Created		-> October 2023
// Author		-> George Prosser (Studio Gobo)

#include "CollisionQueryTestActor.h"
#include "CollisionQueryDrawDebugHelpers.h"
#include "Engine/World.h"

ACollisionQueryTestActor::ACollisionQueryTestActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	StartComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Start"));
#if WITH_EDITORONLY_DATA
	StartComponent->bVisualizeComponent = true;
#endif
	RootComponent = StartComponent;

	EndComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("End"));
	EndComponent->SetupAttachment(StartComponent);
	EndComponent->SetRelativeLocation(FVector(200.f, 0.f, 0.f));
#if WITH_EDITORONLY_DATA
	EndComponent->bVisualizeComponent = true;
#endif
}

void ACollisionQueryTestActor::Tick(float DeltaSeconds)
{
	const float LineThickness = 0.f;
	const float PointSize = 16.f;
	
	UWorld* World = GetWorld();

	const FVector Pos = GetActorLocation();
	const FQuat Rot = GetActorQuat();

	const FVector Start = Pos;
	const FVector End = EndComponent->GetComponentLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = bTraceComplex;
	QueryParams.bFindInitialOverlaps = bFindInitialOverlaps;
	QueryParams.bIgnoreBlocks = bIgnoreBlocks;
	QueryParams.bIgnoreTouches = bIgnoreTouches;
	QueryParams.bSkipNarrowPhase = bSkipNarrowPhase;
	QueryParams.MobilityType = ConvertToQueryMobilityType(MobilityType);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse = CollisionResponses;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams = FCollisionObjectQueryParams(UEngineTypes::ConvertToCollisionChannel(ObjectType));

	FCollisionShape CollisionShape;
	if (Shape == ECollisionQueryTestShape::Box)
	{
		CollisionShape = FCollisionShape::MakeBox(BoxHalfExtent);
	}
	else if (Shape == ECollisionQueryTestShape::Sphere)
	{
		CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	}
	else if (Shape == ECollisionQueryTestShape::Capsule)
	{
		CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	}

	bool bResult = false;
	FHitResult Hit;
	TArray<FHitResult> Hits;
	TArray<FOverlapResult> Overlaps;

	if (Query == ECollisionQueryTestType::LineTrace)
	{
		if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Single)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->LineTraceSingleByProfile(Hit, Start, End, CollisionProfileName, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			if (bResult)
			{
				DrawDebugLine(World, Start, Hit.Location, FColor::Green, false, 0.f, 0, LineThickness);
				DrawDebugPoint(World, Hit.ImpactPoint, PointSize, FColor::Green, false, 0.f, 0);
			}
			else
			{
				DrawDebugLine(World, Start, End, FColor::Red, false, 0.f, 0, LineThickness);
			}
#endif // ENABLE_DRAW_DEBUG
		}
		else if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Multi)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->LineTraceMultiByChannel(Hits, Start, End, Channel, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->LineTraceMultiByObjectType(Hits, Start, End, ObjectQueryParams, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->LineTraceMultiByProfile(Hits, Start, End, CollisionProfileName, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			if (bResult)
			{
				FVector TraceEnd = Hits.Last().Location;
				if (By == ECollisionQueryTestBy::ObjectType)
				{
					TraceEnd = End; // trace by object type does not stop on first blocking hit
				}

				DrawDebugLine(World, Start, TraceEnd, FColor::Green, false, 0.f, 0, LineThickness);
			}
			else
			{
				DrawDebugLine(World, Start, End, Hits.Num() > 0 ? FColor::Blue : FColor::Red, false, 0.f, 0, LineThickness);
			}

			for (const FHitResult& HitResult : Hits)
			{
				DrawDebugPoint(World, HitResult.ImpactPoint, PointSize, HitResult.bBlockingHit ? FColor::Green : FColor::Blue, false, 0.f, 0);
			}
#endif // ENABLE_DRAW_DEBUG
		}
		else if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Test)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->LineTraceTestByChannel(Start, End, Channel, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->LineTraceTestByObjectType(Start, End, ObjectQueryParams, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->LineTraceTestByProfile(Start, End, CollisionProfileName, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			DrawDebugLine(World, Start, End, bResult ? FColor::Green : FColor::Red, false, 0.f, 0, LineThickness);
#endif // ENABLE_DRAW_DEBUG
		}
	}
	else if (Query == ECollisionQueryTestType::Sweep)
	{
		if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Single)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->SweepSingleByChannel(Hit, Start, End, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->SweepSingleByObjectType(Hit, Start, End, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->SweepSingleByProfile(Hit, Start, End, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			if (bResult)
			{
				DrawDebugPoint(World, Hit.ImpactPoint, PointSize, FColor::Green, false, 0.f, 0);
				DrawDebugSweptCollisionShape(World, Start, Hit.Location, Rot, CollisionShape, FColor::Green, false, 0.f, 0, LineThickness);
			}
			else
			{
				DrawDebugSweptCollisionShape(World, Start, End, Rot, CollisionShape, FColor::Red, false, 0.f, 0, LineThickness);
			}
#endif // ENABLE_DRAW_DEBUG
		}
		else if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Multi)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->SweepMultiByChannel(Hits, Start, End, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->SweepMultiByObjectType(Hits, Start, End, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->SweepMultiByProfile(Hits, Start, End, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			if (bResult)
			{
				FVector SweepEnd = Hits.Last().Location;
				if (By == ECollisionQueryTestBy::ObjectType)
				{
					SweepEnd = End; // sweep by object type does not stop on first blocking hit
				}
				else if (Hits.Last().bStartPenetrating)
				{
					SweepEnd = End; // sweeps do not stop on initial blocking overlaps
				}

				DrawDebugSweptCollisionShape(World, Start, SweepEnd, Rot, CollisionShape, FColor::Green, false, 0.f, 0, LineThickness);
			}
			else
			{
				DrawDebugSweptCollisionShape(World, Start, End, Rot, CollisionShape, Hits.Num() > 0 ? FColor::Blue : FColor::Red, false, 0.f, 0, LineThickness);
			}

			for (const FHitResult& HitResult : Hits)
			{
				DrawDebugPoint(World, HitResult.ImpactPoint, PointSize, HitResult.bBlockingHit ? FColor::Green : FColor::Blue, false, 0.f, 0);
			}
#endif // ENABLE_DRAW_DEBUG
		}
		else if (SingleMultiOrTest == ECollisionQueryTestSingleMultiOrTest::Test)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->SweepTestByChannel(Start, End, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->SweepTestByObjectType(Start, End, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->SweepTestByProfile(Start, End, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}

#if ENABLE_DRAW_DEBUG
			DrawDebugSweptCollisionShape(World, Start, End, Rot, CollisionShape, bResult ? FColor::Green : FColor::Red, false, 0.f, 0, LineThickness);
#endif // ENABLE_DRAW_DEBUG
		}
	}
	else if (Query == ECollisionQueryTestType::Overlap)
	{
		if (BlockingAnyOrMulti == ECollisionQueryTestBlockingAnyOrMulti::BlockingTest)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->OverlapBlockingTestByChannel(Pos, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				// NB: there is no OverlapBlockingTestByObjectType function because hits/overlaps from queries by object type are always considered blocking
				bResult = World->OverlapAnyTestByObjectType(Pos, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->OverlapBlockingTestByProfile(Pos, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}
		}
		else if (BlockingAnyOrMulti == ECollisionQueryTestBlockingAnyOrMulti::AnyTest)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->OverlapAnyTestByChannel(Pos, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->OverlapAnyTestByObjectType(Pos, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->OverlapAnyTestByProfile(Pos, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}
		}
		else if (BlockingAnyOrMulti == ECollisionQueryTestBlockingAnyOrMulti::Multi)
		{
			if (By == ECollisionQueryTestBy::Channel)
			{
				bResult = World->OverlapMultiByChannel(Overlaps, Pos, Rot, Channel, CollisionShape, QueryParams, ResponseParams);
			}
			else if (By == ECollisionQueryTestBy::ObjectType)
			{
				bResult = World->OverlapMultiByObjectType(Overlaps, Pos, Rot, ObjectQueryParams, CollisionShape, QueryParams);
			}
			else if (By == ECollisionQueryTestBy::Profile)
			{
				bResult = World->OverlapMultiByProfile(Overlaps, Pos, Rot, CollisionProfileName, CollisionShape, QueryParams);
			}
		}

#if ENABLE_DRAW_DEBUG
		DrawDebugCollisionShape(World, Pos, Rot, CollisionShape, bResult ? FColor::Green : FColor::Red, false, 0.f, 0, LineThickness);
#endif // ENABLE_DRAW_DEBUG
	}
}

TArray<FName> ACollisionQueryTestActor::GetCollisionProfileOptions() const
{
	TArray<FName> OutOptions;

	const UCollisionProfile* CollisionProfile = UCollisionProfile::Get();
	const int32 NumProfiles = CollisionProfile->GetNumOfProfiles();

	OutOptions.Reserve(NumProfiles);

	for (int32 ProfileIdx = 0; ProfileIdx < NumProfiles; ++ProfileIdx)
	{
		const FCollisionResponseTemplate* ProfileTemplate = CollisionProfile->GetProfileByIndex(ProfileIdx);
		OutOptions.Add(ProfileTemplate->Name);
	}

	return OutOptions;
}

EQueryMobilityType ACollisionQueryTestActor::ConvertToQueryMobilityType(ECollisionQueryTestMobilityType TestType)
{
	switch (TestType)
	{
	case ECollisionQueryTestMobilityType::Any: return EQueryMobilityType::Any;
	case ECollisionQueryTestMobilityType::Static: return EQueryMobilityType::Static;
	case ECollisionQueryTestMobilityType::Dynamic: return EQueryMobilityType::Dynamic;
	default: return EQueryMobilityType::Any;
	}
}
