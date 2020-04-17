#include "f4se/GameMenus.h"
#include "f4se/GameReferences.h"
#include "f4se/NiMaterials.h"
#include "f4se/ObScript.h"
#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se_common/SafeWrite.h"

#include "ActorManager.h"
#include "config.h"
#include "EventDebugLogger.h"
#include "hdtSkyrimPhysicsWorld.h"
#include "Hooks.h"
#include "HookEvents.h"

#include <shlobj_core.h>
#include "f4se/GameRTTI.h"
#include "f4se_common/BranchTrampoline.h"
#include "../../common/ITypes.h"

//namespace hdt
//{
//	IDebugLog gLog;
//	EventDebugLogger g_eventDebugLogger;
//
//	class FreezeEventHandler : public BSTEventSink<MenuOpenCloseEvent>
//	{
//	public:
//		FreezeEventHandler()
//		{
//		}
//
//		EventResult ReceiveEvent(MenuOpenCloseEvent* evn, void * dispatcher) override
//		{
//			auto mm = *g_ui;
//
//			if (evn && evn->isOpen && (!strcmp(evn->menuName.c_str(), "Loading Menu") || !strcmp(
//				evn->menuName.c_str(), "RaceSex Menu")))
//			{
//				_DMESSAGE("loading menu/racesexmenu detected, scheduling physics reset on world un-suspend");
//				SkyrimPhysicsWorld::get()->suspend(true);
//			}
//
//			if (evn && !evn->isOpen && !strcmp(evn->menuName.c_str(), "RaceSex Menu"))
//			{
//				_DMESSAGE("racemenu closed, reloading meshes");
//				ActorManager::instance()->reloadMeshes();
//			}
//
//			return kEvent_Continue;
//		}
//	} g_freezeEventHandler;
//
//	void checkOldPlugins()
//	{
//		auto framework = GetModuleHandleA("hdtSSEFramework");
//		auto physics = GetModuleHandleA("hdtSSEPhysics");
//		auto hh = GetModuleHandleA("hdtSSEHighHeels");
//
//		if (physics)
//		{
//			MessageBox(nullptr, TEXT(
//				           "hdtSSEPhysics.dll is loaded. This is an older verson of HDT-SMP and conflicts with hdtSMP64.dll. Please remove it."),
//			           TEXT("hdtSMP64"), MB_OK);
//		}
//
//		if (framework && !hh)
//		{
//			MessageBox(nullptr, TEXT(
//				           "hdtSSEFramework.dll is loaded but hdtSSEHighHeels.dll is not being used. You no longer need hdtSSEFramework.dll with this version of SMP. Please remove it."),
//			           TEXT("hdtSMP64"), MB_OK);
//		}
//	}
//
//	NiPointer<NiTexture>* GetTextureFromIndex(BSLightingShaderMaterialBase* material, UInt32 index)
//	{
//		switch (index)
//		{
//		case 0:
//			return &material->spDiffuseTexture;
//			break;
//		case 1:
//			return &material->spNormalTexture;
//			break;
//		case 2:
//			{
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Face)
//				{
//					return &static_cast<BSLightingShaderMaterialFace*>(material)->baseDiffuse;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Glowmap)
//				{
//					return &static_cast<BSLightingShaderMaterialFace*>(material)->baseDiffuse;
//				}
//				return &material->spRimSoftLightingTexture;
//			}
//			break;
//		case 3:
//			{
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Face)
//				{
//					return &static_cast<BSLightingShaderMaterialFace*>(material)->baseDiffuse;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Parallax)
//				{
//					return &static_cast<BSLightingShaderMaterialParallax*>(material)->spHeightTexture;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Parallax || material->GetType() ==
//					BSLightingShaderMaterialBase::kType_ParallaxOcc)
//				{
//					return &static_cast<BSLightingShaderMaterialParallaxOcc*>(material)->spHeightTexture;
//				}
//			}
//			break;
//		case 4:
//			{
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Eye)
//				{
//					return &static_cast<BSLightingShaderMaterialEye*>(material)->spEnvTexture;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Envmap)
//				{
//					return &static_cast<BSLightingShaderMaterialEnvmap*>(material)->spEnvTexture;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_MultiLayerParallax)
//				{
//					return &static_cast<BSLightingShaderMaterialMultiLayerParallax*>(material)->spEnvMaskTexture;
//				}
//			}
//			break;
//		case 5:
//			{
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Eye)
//				{
//					return &static_cast<BSLightingShaderMaterialEye*>(material)->spEnvMaskTexture;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Envmap)
//				{
//					return &static_cast<BSLightingShaderMaterialEnvmap*>(material)->spEnvMaskTexture;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_MultiLayerParallax)
//				{
//					return &static_cast<BSLightingShaderMaterialMultiLayerParallax*>(material)->spEnvMaskTexture;
//				}
//			}
//			break;
//		case 6:
//			{
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_Face)
//				{
//					return &static_cast<BSLightingShaderMaterialFace*>(material)->baseDiffuse;
//				}
//				if (material->GetType() == BSLightingShaderMaterialBase::kType_MultiLayerParallax)
//				{
//					return &static_cast<BSLightingShaderMaterialMultiLayerParallax*>(material)->spLayerTexture;
//				}
//			}
//			break;
//		case 7:
//			return &material->spSmoothnessSpecMaskTexture;
//			break;
//		}
//
//		return nullptr;
//	}
//
//	void DumpNodeChildren(NiAVObject* node)
//	{
//		_MESSAGE("{%s} {%s} {%X} [%f, %f, %f]", node->GetRTTI()->name, node->m_name, node, node->m_worldTransform.pos.x, node->m_worldTransform.pos.y, node->m_worldTransform.pos.z);
//		if (node->m_extraData->count > 0)
//		{
//			gLog.Indent();
//			for (UInt16 i = 0; i < node->m_extraData->count; i++)
//			{
//				NiExtraData* extraDataPtr; 
//				node->m_extraData->GetNthItem(i, extraDataPtr);
//				_MESSAGE("{%s} {%s} {%X}", extraDataPtr->GetRTTI()->name, extraDataPtr->m_name, node);
//			}
//			gLog.Outdent();
//		}
//
//		NiNode* niNode = node->GetAsNiNode();
//		if (niNode && niNode->m_children.m_emptyRunStart > 0)
//		{
//			gLog.Indent();
//			for (int i = 0; i < niNode->m_children.m_emptyRunStart; i++)
//			{
//				NiAVObject* object = niNode->m_children.m_data[i];
//				if (object)
//				{
//					NiNode* childNode = object->GetAsNiNode();
//					BSGeometry* geometry = object->GetAsBSGeometry();
//					if (geometry)
//					{
//						_MESSAGE("{%s} {%s} {%X} [%f, %f, %f] - Geometry", object->GetRTTI()->name, object->m_name, object, geometry->m_worldTransform.pos.x, geometry->m_worldTransform.pos.y, geometry->m_worldTransform.pos.z);
//						if (geometry->skinInstance)
//						{
//							gLog.Indent();
//							for (int i = 0; i < geometry->skinInstance->bones.count; i++)
//							{
//								auto bone = geometry->skinInstance->bones[i];
//								_MESSAGE("Bone %d - {%s} {%s} {%X} [%f, %f, %f]", i, bone->GetRTTI()->name, bone->m_name, bone, bone->m_worldTransform.pos.x, bone->m_worldTransform.pos.y, bone->m_worldTransform.pos.z);
//							}
//							gLog.Outdent();
//						}
//						NiPointer<BSShaderProperty> shaderProperty = niptr_cast<BSShaderProperty>(
//							geometry->effectState);
//						if (shaderProperty)
//						{
//							BSLightingShaderProperty* lightingShader =
//								ni_cast(shaderProperty, BSLightingShaderProperty);
//							if (lightingShader)
//							{
//								BSLightingShaderMaterial* material = static_cast<BSLightingShaderMaterial*>(
//									lightingShader->shaderMaterial);
//
//								gLog.Indent();
//								for (int i = 0; i < 10; ++i) // 10 is size of texture set
//								{
//									const char* texturePath = material->spTextureSet->GetTextureFilenameFS(i);
//									if (!texturePath)
//									{
//										continue;
//									}
//
//									const char* textureName = "";
//									NiPointer<NiTexture> * texture = GetTextureFromIndex(material, i);
//									if (texture)
//									{
//										textureName = (*texture)->name;
//									}
//
//									_MESSAGE("Texture %d - %s (%s)", i, texturePath, textureName);
//								}
//								_MESSAGE("Flags - %016X", lightingShader->flags);
//								gLog.Outdent();
//							}
//						}
//					}
//					else if (childNode)
//					{
//						DumpNodeChildren(childNode);
//					}
//					else
//					{
//						_MESSAGE("{%s} {%s} {%X} [%f, %f, %f]", object->GetRTTI()->name, object->m_name, object, object->m_worldTransform.pos.x, object->m_worldTransform.pos.y, object->m_worldTransform.pos.z);
//					}
//				}
//			}
//			gLog.Outdent();
//		}
//	}
//
//	void SMPDebug_PrintDetailed()
//	{
//		auto skeletons = ActorManager::instance()->getSkeletons();
//
//		for (auto skeleton : skeletons)
//		{
//			TESObjectREFR* skelOwner = nullptr;
//			TESFullName* ownerName = nullptr;
//
//			if (skeleton.skeleton->m_owner)
//			{
//				skelOwner = skeleton.skeleton->m_owner;
//				if (skelOwner->baseForm)
//					ownerName = DYNAMIC_CAST(skelOwner->baseForm, TESForm, TESFullName);
//			}
//
//			Console_Print("[HDT-SMP] %s skeleton - owner %s (refr formid %08x, base formid %08x)",
//			              skeleton.isActiveInScene() ? "active" : "inactive",
//			              ownerName ? ownerName->name.c_str() : "unk_name",
//			              skelOwner ? skelOwner->formID : 0x00000000,
//			              skelOwner && skelOwner->baseForm ? skelOwner->baseForm->formID : 0x00000000
//			);
//
//			for (auto armor : skeleton.armors)
//			{
//				Console_Print("[HDT-SMP] -- tracked armor addon %s, %s",
//				              armor.armorWorn->m_name,
//				              armor.physics
//					              ? armor.physics->m_world
//						                ? "has active physics system"
//						                : "has inactive physics system"
//					              : "has no physics system");
//
//				if (armor.physics)
//				{
//					for (auto mesh : armor.physics->m_meshes)
//						Console_Print("[HDT-SMP] ---- has collision mesh %s", mesh->m_name->cstr());
//				}
//			}
//
//			if (skeleton.head.headNode)
//			{
//				for (auto headPart : skeleton.head.headParts)
//				{
//					Console_Print("[HDT-SMP] -- tracked headpart %s, %s",
//					              headPart.headPart->m_name,
//					              headPart.physics
//						              ? headPart.physics->m_world
//							                ? "has active physics system"
//							                : "has inactive physics system"
//						              : "has no physics system");
//
//					if (headPart.physics)
//					{
//						for (auto mesh : headPart.physics->m_meshes)
//							Console_Print("[HDT-SMP] ---- has collision mesh %s", mesh->m_name->cstr());
//					}
//				}
//			}
//		}
//	}
//
//	bool SMPDebug_Execute(const ObScriptParam* paramInfo, ScriptData* scriptData, TESObjectREFR* thisObj,
//	                      TESObjectREFR* containingObj, Script* scriptObj, ScriptLocals* locals, double& result,
//	                      UInt32& opcodeOffsetPtr)
//	{
//		char buffer[MAX_PATH];
//		memset(buffer, 0, MAX_PATH);
//		char buffer2[MAX_PATH];
//		memset(buffer2, 0, MAX_PATH);
//
//		if (!ObjScript_ExtractArgs(paramInfo, scriptData, opcodeOffsetPtr, thisObj, containingObj, scriptObj, locals,
//		                           buffer, buffer2))
//		{
//			return false;
//		}
//
//		if (_strnicmp(buffer, "reset", MAX_PATH) == 0)
//		{
//			Console_Print("running full smp reset");
//			SkyrimPhysicsWorld::get()->resetTransformsToOriginal();
//			ActorManager::instance()->reloadMeshes();
//			SkyrimPhysicsWorld::get()->resetSystems();
//			return true;
//		}
//		if (_strnicmp(buffer, "dumptree", MAX_PATH) == 0)
//		{
//			if (thisObj)
//			{
//				Console_Print("dumping targeted reference's node tree");
//				DumpNodeChildren(thisObj->GetNiRootNode(0));
//			}
//			else
//			{
//				Console_Print("error: you must target a reference to dump their node tree");
//			}
//
//			return true;
//		}
//		if (_strnicmp(buffer, "detail", MAX_PATH) == 0)
//		{
//			SMPDebug_PrintDetailed();
//			return true;
//		}
//
//		auto skeletons = ActorManager::instance()->getSkeletons();
//
//		size_t activeSkeletons = 0;
//		size_t armors = 0;
//		size_t headParts = 0;
//		size_t activeArmors = 0;
//		size_t activeHeadParts = 0;
//		size_t activeCollisionMeshes = 0;
//
//		for (auto skeleton : skeletons)
//		{
//			if (skeleton.isActiveInScene())
//				activeSkeletons++;
//
//			for (const auto armor : skeleton.armors)
//			{
//				armors++;
//
//				if (armor.physics && armor.physics->m_world)
//				{
//					activeArmors++;
//
//					activeCollisionMeshes += armor.physics->m_meshes.size();
//				}
//			}
//
//			if (skeleton.head.headNode)
//			{
//				for (const auto headpart : skeleton.head.headParts)
//				{
//					headParts++;
//
//					if (headpart.physics && headpart.physics->m_world)
//					{
//						activeHeadParts++;
//
//						activeCollisionMeshes += headpart.physics->m_meshes.size();
//					}
//				}
//			}
//		}
//
//		Console_Print("[HDT-SMP] tracked skeletons: %d", skeletons.size());
//		Console_Print("[HDT-SMP] active skeletons: %d", activeSkeletons);
//		Console_Print("[HDT-SMP] tracked armor addons: %d", armors);
//		Console_Print("[HDT-SMP] tracked head parts: %d", headParts);
//		Console_Print("[HDT-SMP] active armor addons: %d", activeArmors);
//		Console_Print("[HDT-SMP] active head parts: %d", activeHeadParts);
//		Console_Print("[HDT-SMP] active collision meshes: %d", activeCollisionMeshes);
//
//		return true;
//	}
//}

extern "C" {
bool F4SEPlugin_Query(const F4SEInterface* f4se, PluginInfo* info)
{
	// populate info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "hdtSSEPhysics";
	info->version = 1;

	//hdt::gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\hdtSMP64.log");
	//hdt::gLog.SetLogLevel(IDebugLog::LogLevel::kLevel_Message);

	_MESSAGE("hdtSMP64 2.0");

	if (f4se->isEditor)
	{
		return false;
	}

	if (f4se->runtimeVersion != CURRENT_RELEASE_RUNTIME)
	{
		_FATALERROR("attempted to load plugin into unsupported game version, exiting");
		return false;
	}

	if (!g_branchTrampoline.Create(1024 * 1))
	{
		_FATALERROR("couldn't create branch trampoline. this is fatal. skipping remainder of init process.");
		return false;
	}

	if (!g_localTrampoline.Create(1024 * 1, nullptr))
	{
		_FATALERROR("couldn't create codegen buffer. this is fatal. skipping remainder of init process.");
		return false;
	}

	return true;
}

bool F4SEPlugin_Load(const F4SEInterface* f4se)
{
	hdt::g_frameEventDispatcher.addListener(hdt::ActorManager::instance());
	hdt::g_frameEventDispatcher.addListener(hdt::SkyrimPhysicsWorld::get());
	hdt::g_shutdownEventDispatcher.addListener(hdt::ActorManager::instance());
	hdt::g_shutdownEventDispatcher.addListener(hdt::SkyrimPhysicsWorld::get());
	hdt::g_armorAttachEventDispatcher.addListener(hdt::ActorManager::instance());
	hdt::g_skinSingleHeadGeometryEventDispatcher.addListener(hdt::ActorManager::instance());
	hdt::g_skinAllHeadGeometryEventDispatcher.addListener(hdt::ActorManager::instance());
//
	hdt::hookAll();
//
//	const auto messageInterface = reinterpret_cast<F4SEMessagingInterface*>(f4se->QueryInterface(kInterface_Messaging));
//	if (messageInterface)
//	{
//		const auto cameraDispatcher = static_cast<BSTEventDispatcher<F4SECameraEvent>*>(messageInterface->
//			GetEventDispatcher(F4SEMessagingInterface::kDispatcher_CameraEvent));
//
//		if (cameraDispatcher)
//			cameraDispatcher->AddEventSink(hdt::SkyrimPhysicsWorld::get());
//
//		messageInterface->RegisterListener(f4se->GetPluginHandle(), "F4SE", [](F4SEMessagingInterface::Message* msg)
//		{
//			if (msg && msg->type == F4SEMessagingInterface::kMessage_InputLoaded)
//			{
//				UI* mm = *g_ui;
//				if (mm)
//				{
//					mm->menuOpenCloseEventSource.AddEventSink(&hdt::g_freezeEventHandler);
//				}
//				hdt::checkOldPlugins();
//				hdt::loadConfig();
//#ifdef DEBUG
//						hdt::g_armorAttachEventDispatcher.addListener(&hdt::g_eventDebugLogger);
//						GetEventDispatcherList()->unk1B8.AddEventSink(&hdt::g_eventDebugLogger);
//						GetEventDispatcherList()->unk840.AddEventSink(&hdt::g_eventDebugLogger);
//#endif
//			}
//		});
//	}

	//ObScriptCommand* hijackedCommand = nullptr;
	//for (ObScriptCommand* iter = g_firstConsoleCommand; iter->opcode < kObScript_NumConsoleCommands +
	//     kObScript_ConsoleOpBase; ++iter)
	//{
	//	if (!strcmp(iter->longName, "ShowRenderPasses"))
	//	{
	//		hijackedCommand = iter;
	//		break;
	//	}
	//}
	//if (hijackedCommand)
	//{
	//	static ObScriptParam params[1];
	//	params[0].typeID = ObScriptParam::kType_String;
	//	params[0].typeStr = "String (optional)";
	//	params[0].isOptional = 1;

	//	ObScriptCommand cmd = *hijackedCommand;

	//	cmd.longName = "SMPDebug";
	//	cmd.shortName = "smp";
	//	cmd.helpText = "smp <reset>";
	//	cmd.needsParent = 0;
	//	cmd.numParams = 1;
	//	cmd.params = params;
	//	cmd.execute = hdt::SMPDebug_Execute;
	//	cmd.flags = 0;
	//	SafeWriteBuf(reinterpret_cast<uintptr_t>(hijackedCommand), &cmd, sizeof(cmd));
	//}

	return true;
}
}
