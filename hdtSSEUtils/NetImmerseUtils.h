#pragma once

#include <common\IPrefix.h>
#include <f4se\NiNodes.h>
#include <f4se\NiExtraData.h>
#include <f4se\BsGeometry.h>
#include "Ref.h"

// note: both of these offsets are 0xDEADBEEF guards and were completely wrong prior to this, but things worked anyway I guess
// 89 75 C4 48 8D 05 ? ? ? ? 
class NiCloningProcess
{
public:
	uint32_t	unk00 = 0;
	uint32_t	unk04 = 0;
	uint32_t	unk08 = 0;
	uint32_t	unk0C = 0;
	uint32_t	unk10 = 0;
	uint32_t	unk14 = 0;
	uintptr_t	unk18 = RelocationManager::s_baseAddr + 0x01DB348C;
	void*		unk20 = 0;
	void*		unk28 = 0;
	uint32_t	unk30 = 0;
	uint32_t	unk34 = 0;
	uint32_t	unk38 = 0;
	uint32_t	unk3c = 0;
	uint32_t	unk40 = 0;
	uint32_t	unk44 = 0;
	uintptr_t	unk48 = RelocationManager::s_baseAddr + 0x01DB3488;
	void*		unk50 = 0;
	void*		unk58 = 0;
	uint32_t	cloneType = 1;
	char		appendChar = '$';
	float		unk68 = 1.0f;
	float		unk6c = 1.0f;
	float		unk70 = 1.0f;
	uint32_t	unk74 = 0;
};

namespace hdt
{
	inline void setBSFixedString(BSFixedString* ref, const char* value)
	{
		CALL_MEMBER_FN(ref, Set)(value);
	}

	inline void setNiNodeName(NiNode* node, const char* name)
	{
		setBSFixedString(&node->m_name, name);
	}

	inline NiNode* castNiNode(NiAVObject* obj) { return obj ? obj->GetAsNiNode() : nullptr; }
	inline BSTriShape* castBSTriShape(NiAVObject* obj) { return obj ? obj->GetAsBSTriShape() : nullptr; }
	inline BSDynamicTriShape* castBSDynamicTriShape(NiAVObject* obj) { return obj ? obj->GetAsBSDynamicTriShape() : nullptr; }

	NiNode* addParentToNode(NiNode* node, const char* name);

	NiAVObject* findObject(NiAVObject* obj, const BSFixedString& name);
	NiNode* findNode(NiNode* obj, const BSFixedString& name);

	inline float length(const NiPoint3& a) { return sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }
	inline float distance(const NiPoint3& a, const NiPoint3& b) { return length(a - b); }

	namespace ref
	{
		inline void retain(NiRefObject* object) { object->IncRef(); }
		inline void release(NiRefObject* object) { object->DecRef(); }
	}

	std::string readAllFile(const char* path);
	std::string readAllFile2(const char* path);

	void updateTransformUpDown(NiAVObject* obj, bool dirty);
}

