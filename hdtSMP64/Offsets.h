#pragma once

#include <cstdint>

namespace hdt
{
	// signatures generated with IDA SigMaker plugin
	namespace offset
	{
		
		// hdtSkyrimPhysicsWorld.cpp
		// F3 44 0F 58 E1 F3 0F 59 05 ? ? ? ? 
		constexpr std::uintptr_t GameStepTimer_SlowTime = 0x05B5B6D0; // maybe?

		
		// Hooks.cpp
		// E8 ? ? ? ? 48 8B 0E 48 89 01 FF C3 
		constexpr std::uintptr_t ArmorAttachFunction = 0x001C3A10; // updated

		// BSFaceGenNiNode last vfunc
		constexpr std::uintptr_t BSFaceGenNiNode_SkinAllGeometry = 0x00685720; // updated
		constexpr std::uintptr_t BSFaceGenNiNode_SkinSingleGeometry = 0x00685980; // updated

		// seems to be code specific to sse
		// .text:00000001403D88D4                 cmp     ebx, 8
		// patch 8 -> 7
		//constexpr std::uintptr_t BSFaceGenNiNode_SkinSingleGeometry_bug = BSFaceGenNiNode_SkinSingleGeometry + 0x96;

		// Hooks.cpp
		// function responsible for majority of main game thread loop
		// E8 ? ? ? ? 48 8B 0D ? ? ? ? E8 ? ? ? ? B9 ? ? ? ? 48 83 C4 28 
		constexpr std::uintptr_t GameLoopFunction = 0x02042430; // updated? 0D38E60 is the equivalent in fo4
		// 48 8B C4 55 48 81 EC ? ? ? ? 33 ED 
		constexpr std::uintptr_t GameShutdownFunction = 0x027D4DF0; // updated

		// FaceGeom string
		constexpr std::uintptr_t TESNPC_GetFaceGeomPath = 0x005B8F90; // updated

		// BSFaceGenModelExtraData Bone Limit
		// 48 8D 34 CB 48 3B DE 74 24 0F 1F 84 00 ? ? ? ? 
		constexpr std::uintptr_t BSFaceGenModelExtraData_BoneLimit = 0x006935DF; // updated
	}
}
