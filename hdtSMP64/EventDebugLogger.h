#pragma once

#include "f4se/GameEvents.h"

#include "HookEvents.h"
#include "IEventListener.h"

namespace hdt
{
	class EventDebugLogger
		: public IEventListener<ArmorAttachEvent>
		  , public BSTEventSink<TESCellAttachDetachEvent>
		  , public BSTEventSink<TESMoveAttachDetachEvent>
	{
	protected:
		EventResult ReceiveEvent(TESCellAttachDetachEvent* evn, void * dispatcher) override;
		EventResult ReceiveEvent(TESMoveAttachDetachEvent* evn, void * dispatcher) override;

		void onEvent(const ArmorAttachEvent&) override;
	};
}
