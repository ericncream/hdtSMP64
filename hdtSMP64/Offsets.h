#pragma once

#include <cstdint>

namespace hdt
{
	// signatures generated with IDA SigMaker plugin
	namespace offset
	{
		
		// hdtSkyrimPhysicsWorld.cpp
		// 74 35 45 33 C0 33 D2
		constexpr std::uintptr_t GameStepTimer_SlowTime = 0x02F6B948; // cant seem to find this in fallout, maybe not needed

		
		// Hooks.cpp
		// E8 ? ? ? ? 48 8B E8 FF C7 
		constexpr std::uintptr_t ArmorAttachFunction = 0x01CB6E20; // hard one to find :/

		// BSFaceGenNiNode last vfunc
		constexpr std::uintptr_t BSFaceGenNiNode_SkinAllGeometry = 0x00685720; // updated
		constexpr std::uintptr_t BSFaceGenNiNode_SkinSingleGeometry = 0x00685980; // updated

		// seems to be code specific to sse
		// .text:00000001403D88D4                 cmp     ebx, 8
		// patch 8 -> 7
		//constexpr std::uintptr_t BSFaceGenNiNode_SkinSingleGeometry_bug = BSFaceGenNiNode_SkinSingleGeometry + 0x96;

		// Hooks.cpp
		// function responsible for majority of main game thread loop
		// E8 ? ? ? ? 84 DB 74 24 
		constexpr std::uintptr_t GameLoopFunction = 0x02042430; // updated
		// E8 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? 48 8B 0D ? ? ? ? 48 85 C9 74 0C E8 ? ? ? ? 
		constexpr std::uintptr_t GameShutdownFunction = 0x014813F0; // not updated

		// FaceGeom string
		constexpr std::uintptr_t TESNPC_GetFaceGeomPath = 0x005B8F90; // updated

		// BSFaceGenModelExtraData Bone Limit
		// 8B 70 58 EB 02 
		constexpr std::uintptr_t BSFaceGenModelExtraData_BoneLimit = 0x0036B4C8;
	}
}
