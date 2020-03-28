#pragma once

#include "f4se_common/Relocation.h"

#define DEFINE_MEMBER_FN_LONG_HOOK(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
	static inline uintptr_t* _##functionName##_GetPtrAddr(void)						\
	{																				\
		static uintptr_t _address = address + RelocationManager::s_baseAddr;				\
		return &_address;															\
	}																				\
																					\
	static inline _##functionName##_type * _##functionName##_GetPtr(void)			\
	{																				\
		return (_##functionName##_type *)_##functionName##_GetPtrAddr();			\
	}

#define DEFINE_MEMBER_FN_HOOK(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG_HOOK(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

namespace hdt
{
	void hookAll();
	void unhookAll();
}
