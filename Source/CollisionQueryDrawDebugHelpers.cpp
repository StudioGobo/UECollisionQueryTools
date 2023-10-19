// ----------------------------------------------------------------------------
// Copyright (c) Studio Gobo Ltd 2023
// Licensed under the MIT license.  
// See LICENSE.TXT in the project root for license information.
// ----------------------------------------------------------------------------
// File			-> CollisionQueryDrawDebugHelpers.cpp
// Created		-> October 2023
// Author		-> George Prosser (Studio Gobo)

#include "CollisionQueryDrawDebugHelpers.h"

#if ENABLE_DRAW_DEBUG

void DrawDebugSweptBox(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FVector& HalfSize, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	DrawDebugBox(World, Start, HalfSize, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugBox(World, End, HalfSize, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);

	FVector Vertices[8];
	Vertices[0] = Start + Rotation.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, -HalfSize.Z));
	Vertices[1] = Start + Rotation.RotateVector(FVector(-HalfSize.X, HalfSize.Y, -HalfSize.Z));
	Vertices[2] = Start + Rotation.RotateVector(FVector(-HalfSize.X, -HalfSize.Y, HalfSize.Z));
	Vertices[3] = Start + Rotation.RotateVector(FVector(-HalfSize.X, HalfSize.Y, HalfSize.Z));
	Vertices[4] = Start + Rotation.RotateVector(FVector(HalfSize.X, -HalfSize.Y, -HalfSize.Z));
	Vertices[5] = Start + Rotation.RotateVector(FVector(HalfSize.X, HalfSize.Y, -HalfSize.Z));
	Vertices[6] = Start + Rotation.RotateVector(FVector(HalfSize.X, -HalfSize.Y, HalfSize.Z));
	Vertices[7] = Start + Rotation.RotateVector(FVector(HalfSize.X, HalfSize.Y, HalfSize.Z));

	const FVector TraceVec = End - Start;
	for (int32 VertexIdx = 0; VertexIdx < 8; ++VertexIdx)
	{
		DrawDebugLine(World, Vertices[VertexIdx], Vertices[VertexIdx] + TraceVec, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
}

void DrawDebugSweptSphere(const UWorld* World, const FVector& Start, const FVector& End, float Radius, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	const FVector TraceVec = End - Start;
	const float Dist = TraceVec.Size();

	// draw the sweep of the sphere as a capsule
	const FVector Center = Start + TraceVec * 0.5f;
	const float CapsuleHalfHeight = (Dist * 0.5f) + Radius;
	const FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	DrawDebugCapsule(World, Center, CapsuleHalfHeight, Radius, CapsuleRot, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);

	// draw additional circles for the spheres at each end of the capsule
	DrawCircle(World, Start, CapsuleRot.GetAxisY(), CapsuleRot.GetAxisZ(), Color, Radius, 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawCircle(World, Start, CapsuleRot.GetAxisX(), CapsuleRot.GetAxisZ(), Color, Radius, 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawCircle(World, End, CapsuleRot.GetAxisY(), -CapsuleRot.GetAxisZ(), Color, Radius, 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawCircle(World, End, CapsuleRot.GetAxisX(), -CapsuleRot.GetAxisZ(), Color, Radius, 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
}

void DrawDebugSweptCapsule(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, float HalfHeight, float Radius, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	DrawDebugCapsule(World, Start, HalfHeight, Radius, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugCapsule(World, End, HalfHeight, Radius, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);

	const FVector Up = Rotation.GetUpVector();

	const FMatrix Mat = FRotationMatrix::MakeFromZX(End - Start, Up);
	const FVector YAxis = Mat.GetUnitAxis(EAxis::Y);
	const FVector XAxis =  Mat.GetUnitAxis(EAxis::X);

	const float HalfLength = HalfHeight - Radius;
	const FVector StartTop = Start + HalfLength*Up;
	const FVector EndTop = End + HalfLength*Up;

	DrawDebugLine(World, StartTop + Radius*XAxis, EndTop + Radius*XAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugLine(World, StartTop + Radius*YAxis, EndTop + Radius*YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugLine(World, StartTop - Radius*YAxis, EndTop - Radius*YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);

	const FVector StartBottom = Start - HalfLength*Up;
	const FVector EndBottom = End - HalfLength*Up;

	DrawDebugLine(World, StartBottom - Radius*XAxis, EndBottom - Radius*XAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugLine(World, StartBottom + Radius*YAxis, EndBottom + Radius*YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	DrawDebugLine(World, StartBottom - Radius*YAxis, EndBottom - Radius*YAxis, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
}

void DrawDebugSweptCollisionShape(const UWorld* World, const FVector& Start, const FVector& End, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	switch (Shape.ShapeType)
	{
	case ECollisionShape::Line:
		DrawDebugLine(World, Start, End, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ECollisionShape::Box:
		DrawDebugSweptBox(World, Start, End, Rotation, Shape.GetBox(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ECollisionShape::Sphere:
		DrawDebugSweptSphere(World, Start, End, Shape.GetSphereRadius(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ECollisionShape::Capsule:
		DrawDebugSweptCapsule(World, Start, End, Rotation, Shape.GetCapsuleHalfHeight(), Shape.GetCapsuleRadius(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	}
}

void DrawDebugCollisionShape(const UWorld* World, const FVector& Pos, const FQuat& Rotation, const FCollisionShape& Shape, const FColor& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	switch (Shape.ShapeType)
	{
	case ECollisionShape::Line:
		// don't do anything
		break;
	case ECollisionShape::Box:
		DrawDebugBox(World, Pos, Shape.GetBox(), Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ECollisionShape::Sphere:
		DrawCircle(World, Pos, Rotation.GetAxisX(), Rotation.GetAxisY(), Color, Shape.GetSphereRadius(), 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
		DrawCircle(World, Pos, Rotation.GetAxisX(), Rotation.GetAxisZ(), Color, Shape.GetSphereRadius(), 16, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	case ECollisionShape::Capsule:
		DrawDebugCapsule(World, Pos, Shape.GetCapsuleHalfHeight(), Shape.GetCapsuleRadius(), Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		break;
	}
}

#endif // ENABLE_DRAW_DEBUG