#include "config.h"
#include "XmlReader.h"

#include "hdtSkyrimPhysicsWorld.h"

#include <clocale>

namespace hdt
{
	static void solver(XMLReader& reader)
	{
		while (reader.Inspect())
		{
			switch (reader.GetInspected())
			{
			case XMLReader::Inspected::StartTag:
				if (reader.GetLocalName() == "numIterations")
					SkyrimPhysicsWorld::get()->getSolverInfo().m_numIterations = btClamped(reader.readInt(), 4, 128);
				else if (reader.GetLocalName() == "groupIterations")
					ConstraintGroup::MaxIterations = btClamped(reader.readInt(), 0, 4096);
				else if (reader.GetLocalName() == "groupEnableMLCP")
					ConstraintGroup::EnableMLCP = reader.readBool();
				else if (reader.GetLocalName() == "erp")
					SkyrimPhysicsWorld::get()->getSolverInfo().m_erp = btClamped(reader.readFloat(), 0.01f, 1.0f);
				else if (reader.GetLocalName() == "min-fps")
					SkyrimPhysicsWorld::get()->m_timeTick = 1.0f / (btClamped(reader.readInt(), 1, 300));
				else
				{
					_WARNING("Unknown config : ", reader.GetLocalName());
					reader.skipCurrentElement();
				}
				break;
			case XMLReader::Inspected::EndTag:
				return;
			}
		}
	}

	//static void wind(XMLReader& reader)
	//{
	//	while (reader.Inspect())
	//	{
	//		switch (reader.GetInspected())
	//		{
	//		case XMLReader::Inspected::StartTag:
	//			if (reader.GetLocalName() == "windStrength")
	//				SkyrimPhysicsWorld::get()->getWindCtrl()->m_windStrength = btClamped(reader.readFloat(), 0.f, 10000.f);
	//			else
	//			{
	//				_WARNING("Unknown config : ", reader.GetLocalName());
	//				reader.skipCurrentElement();
	//			}
	//			break;
	//		case XMLReader::Inspected::EndTag:
	//			return;
	//		}
	//	}
	//}

	static void smp(XMLReader& reader)
	{
		while (reader.Inspect())
		{
			switch (reader.GetInspected())
			{
			case XMLReader::Inspected::StartTag:
				if (reader.GetLocalName() == "logLevel")
					gLog.SetLogLevel(static_cast<IDebugLog::LogLevel>(reader.readInt()));
				else if (reader.GetLocalName() == "enableNPCFaceParts")
					ActorManager::instance()->m_skinNPCFaceParts = reader.readBool();
				else if (reader.GetLocalName() == "clampRotations")
					SkyrimPhysicsWorld::get()->m_clampRotations = reader.readBool();
				else if (reader.GetLocalName() == "unclampedResets")
					SkyrimPhysicsWorld::get()->m_unclampedResets = reader.readBool();
				else if (reader.GetLocalName() == "unclampedResetAngle")
					SkyrimPhysicsWorld::get()->m_unclampedResetAngle = reader.readFloat();
				else
				{
					_WARNING("Unknown config : ", reader.GetLocalName());
					reader.skipCurrentElement();
				}
				break;
			case XMLReader::Inspected::EndTag:
				return;
			}
		}
	}

	static void config(XMLReader& reader)
	{
		while (reader.Inspect())
		{
			switch (reader.GetInspected())
			{
			case XMLReader::Inspected::StartTag:
				if (reader.GetLocalName() == "solver")
					solver(reader);
					//else if (reader.GetLocalName() == "wind")
					//	wind(reader);
				else if (reader.GetLocalName() == "smp")
					smp(reader);
				else
				{
					_WARNING("Unknown config : ", reader.GetLocalName());
					reader.skipCurrentElement();
				}
				break;
			case XMLReader::Inspected::EndTag:
				return;
			}
		}
	}

	void loadConfig()
	{
		auto bytes = readAllFile2("data/f4se/plugins/hdtSkinnedMeshConfigs/configs.xml");
		if (bytes.empty()) return;

		// Store original locale
		char saved_locale[32];
		strcpy_s(saved_locale, std::setlocale(LC_NUMERIC, nullptr));

		// Set locale to en_US
		std::setlocale(LC_NUMERIC, "en_US");

		XMLReader reader((uint8_t*)bytes.data(), bytes.size());

		while (reader.Inspect())
		{
			if (reader.GetInspected() == XMLReader::Inspected::StartTag)
			{
				if (reader.GetLocalName() == "configs")
					config(reader);
				else
				{
					_WARNING("Unknown config : ", reader.GetLocalName());
					reader.skipCurrentElement();
				}
			}
		}

		// Restore original locale
		std::setlocale(LC_NUMERIC, saved_locale);
	}
}
