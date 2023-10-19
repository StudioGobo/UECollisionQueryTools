// ----------------------------------------------------------------------------
// Copyright (c) Studio Gobo Ltd 2023
// Licensed under the MIT license.  
// See LICENSE.TXT in the project root for license information.
// ----------------------------------------------------------------------------
// File			-> CollisionQueryDrawDebugHelpers.h
// Created		-> October 2023
// Author		-> George Prosser (Studio Gobo)

#pragma once

#include "DrawDebugHelpers.h"
#include "CollisionShape.h"

#if ENABLE_DRAW_DEBUG

void DrawDebugSweptBox(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FVector& HalfSize, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
void DrawDebugSweptSphere(const UWorld* World, const FVector& Start, const FVector& End, float Radius, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0);
void DrawDebugSweptCapsule(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, float HalfHeight, float Radius, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
void DrawDebugSweptCollisionShape(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
void DrawDebugCollisionShape(const UWorld* World, const FVector& Pos, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);

#endif // ENABLE_DRAW_DEBUG