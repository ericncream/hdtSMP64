#pragma once

#include "f4se/GameEvents.h"

#include "HookEvents.h"
#include "IEventListener.h"

struct TESCellAttachDetachEvent
{
	TESObjectREFR* reference;	// 00
	UInt8			attached;	// 08	- 01 - attached, 00 - detached
};

struct TESMoveAttachDetachEvent
{
	TESObjectREFR* reference;	// 00
	UInt8			attached;	// 08	- 01 - attached, 00 - detached
};

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
