#include "hdtDefaultBBP.h"
#include "f4se/GameRTTI.h"
#include "f4se/NiRTTI.h"
#include "XmlReader.h"


#include <unordered_map>
#include <clocale>

#include "../hdtSSEUtils/NetImmerseUtils.h"

namespace hdt
{
	static bool bbpFileLoaded = false;
	std::unordered_map<std::string, std::string> bbpFileList;

	static void loadDefaultBBPs()
	{
		auto path = "F4SE/Plugins/hdtSkinnedMeshConfigs/defaultBBPs.xml";

		auto loaded = readAllFile(path);
		if (loaded.empty()) return;

		// Store original locale
		char saved_locale[32];
		strcpy_s(saved_locale, std::setlocale(LC_NUMERIC, nullptr));

		// Set locale to en_US
		std::setlocale(LC_NUMERIC, "en_US");

		XMLReader reader((uint8_t*)loaded.data(), loaded.size());

		reader.nextStartElement();
		if (reader.GetName() != "default-bbps")
			return;

		while (reader.Inspect())
		{
			if (reader.GetInspected() == Xml::Inspected::StartTag)
			{
				if (reader.GetName() == "map")
				{
					try
					{
						auto shape = reader.getAttribute("shape");
						auto file = reader.getAttribute("file");
						bbpFileList.insert(std::make_pair(shape, file));
					}
					catch (...)
					{
						_WARNING("defaultBBP(%d,%d) : invalid map", reader.GetRow(), reader.GetColumn());
					}
					reader.skipCurrentElement();
				}
				else
				{
					_WARNING("defaultBBP(%d,%d) : unknown element", reader.GetRow(), reader.GetColumn());
					reader.skipCurrentElement();
				}
			}
			else if (reader.GetInspected() == Xml::Inspected::EndTag)
				break;
		}

		// Restore original locale
		std::setlocale(LC_NUMERIC, saved_locale);

		bbpFileLoaded = true;
	}

	std::string scanDefaultBBP(NiNode* armor)
	{
		static std::mutex s_lock;
		std::lock_guard<std::mutex> l(s_lock);

		if (!bbpFileLoaded) loadDefaultBBPs();
		if (bbpFileList.empty()) return "";

		for (int i = 0; i < armor->m_children.m_arrayBufLen; ++i)
		{
			if (!armor->m_children.m_data[i]) continue;

			auto tri = armor->m_children.m_data[i]->GetAsBSTriShape();
			if (!tri) continue;

			std::string name = tri->m_name;
			auto idx = bbpFileList.find(name);
			if (idx != bbpFileList.end())
				return idx->second;
		}
		return "";
	}

	std::string scanBBP(NiNode* scan)
	{
		for (int i = 0; i < scan->m_extraData->count; ++i)
		{
			NiExtraData extraData;
			auto extraDataPtr = &extraData;
			scan->m_extraData->GetNthItem(i, extraDataPtr);
			auto stringData = DYNAMIC_CAST(extraDataPtr, NiExtraData, NiStringExtraData);
			if (stringData && !strcmp(stringData->m_string.c_str(), "HDT Skinned Mesh Physics Object") && stringData->m_string.c_str())
				return std::string(stringData->m_string.c_str());
		}

		return scanDefaultBBP(scan);
	}
}
