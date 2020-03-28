#pragma once

#include "f4se/NiObjects.h"

#include "EventDispatcherImpl.h"
#include "f4se/NiNodes.h"

namespace hdt
{
	struct SkinAllHeadGeometryEvent
	{
		NiNode* skeleton = nullptr;
		BSFaceGenNiNode* headNode = nullptr;
		bool hasSkinned = false;
	};

	struct SkinSingleHeadGeometryEvent
	{
		NiNode* skeleton = nullptr;
		BSFaceGenNiNode* headNode = nullptr;
		BSGeometry* geometry = nullptr;
	};

	struct ArmorAttachEvent
	{
		NiNode* armorModel = nullptr;
		NiNode* skeleton = nullptr;
		NiAVObject* attachedNode = nullptr;
		bool hasAttached = false;
	};

	struct FrameEvent
	{
		bool gamePaused;
	};

	struct ShutdownEvent
	{
	};

	extern EventDispatcherImpl<FrameEvent> g_frameEventDispatcher;
	extern EventDispatcherImpl<ShutdownEvent> g_shutdownEventDispatcher;
	extern EventDispatcherImpl<ArmorAttachEvent> g_armorAttachEventDispatcher;
	extern EventDispatcherImpl<SkinAllHeadGeometryEvent> g_skinAllHeadGeometryEventDispatcher;
	extern EventDispatcherImpl<SkinSingleHeadGeometryEvent> g_skinSingleHeadGeometryEventDispatcher;
}
