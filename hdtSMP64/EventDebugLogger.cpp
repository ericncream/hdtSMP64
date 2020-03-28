#include "f4se/GameReferences.h"
#include "f4se/NiObjects.h"
#include "f4se/NiNodes.h"

#include "EventDebugLogger.h"

namespace hdt
{
	EventResult EventDebugLogger::ReceiveEvent(TESCellAttachDetachEvent* evn, void * dispatcher)
	{
		if (evn && evn->reference && evn->reference->formType == Actor::kTypeID)
		{
			_DMESSAGE("received TESCellAttachDetachEvent(formID %08llX, name %s, attached=%s)", evn->reference->formID,
			          evn->reference->baseForm->GetFullName(), evn->attached ? "true" : "false");
		}
		return kEvent_Continue;
	}

	EventResult EventDebugLogger::ReceiveEvent(TESMoveAttachDetachEvent* evn, void * dispatcher)
	{
		if (evn && evn->reference && evn->reference->formType == Actor::kTypeID)
		{
			_DMESSAGE("received TESMoveAttachDetachEvent(formID %08llX, name %s, attached=%s)", evn->reference->formID,
			          evn->reference->baseForm->GetFullName(), evn->attached ? "true" : "false");
		}
		return kEvent_Continue;
	}

	void EventDebugLogger::onEvent(const ArmorAttachEvent& e)
	{
		_DMESSAGE(
			"received ArmorAttachEvent(armorModel=%s (%016llX), skeleton=%s (%016llX), attachedNode=%s (%016llX), hasAttached=%s)",
			e.armorModel ? e.armorModel->m_name : "null",
			(uintptr_t)e.armorModel,
			e.skeleton ? e.skeleton->m_name : "null",
			(uintptr_t)e.skeleton,
			e.attachedNode ? e.attachedNode->m_name : "null",
			(uintptr_t)e.attachedNode,
			static_cast<uintptr_t>(e.hasAttached) ? "true" : "false");
	}
}
