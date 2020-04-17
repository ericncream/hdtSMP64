#include "hdtSkyrimPhysicsWorld.h"
#include <ppl.h>
#include "Offsets.h"
#include "f4se/GameMenus.h"

namespace hdt
{
	static const float* timeStamp = (float*)0x12E355C;

	SkyrimPhysicsWorld::SkyrimPhysicsWorld(void)
	{
		gDisableDeactivation = true;
		setGravity(btVector3(0, 0, -9.8 * scaleSkyrim));
		m_windSpeed.setValue(0, 0, 5 * scaleSkyrim);

		getSolverInfo().m_friction = 0;
		m_averageInterval = m_timeTick;
		m_accumulatedInterval = 0;
	}

	SkyrimPhysicsWorld::~SkyrimPhysicsWorld(void)
	{
	}

	//void hdtSkyrimPhysicsWorld::suspend()
	//{
	//	m_suspended++;
	//}

	//void hdtSkyrimPhysicsWorld::resume()
	//{
	//	--m_suspended;
	//}

	//void hdtSkyrimPhysicsWorld::switchToSeperateClock()
	//{
	//	m_lock.lock();
	//	m_useSeperatedClock = true;
	//	m_timeLastUpdate = clock()*0.001;
	//	m_lock.unlock();
	//}

	//void hdtSkyrimPhysicsWorld::switchToInternalClock()
	//{
	//	m_lock.lock();
	//	m_useSeperatedClock = false;
	//	m_timeLastUpdate = *timeStamp;
	//	m_lock.unlock();
	//}

	SkyrimPhysicsWorld* SkyrimPhysicsWorld::get()
	{
		static SkyrimPhysicsWorld g_World;
		return &g_World;
	}

	void SkyrimPhysicsWorld::doUpdate(float interval)
	{
		_MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

		//ScanHair();

		m_averageInterval = m_averageInterval * 0.875f + interval * 0.125f;
		auto tick = std::min(m_averageInterval, m_timeTick);

		m_accumulatedInterval += interval;
		if (m_accumulatedInterval > tick * 0.25f)
		{
			interval = std::min<float>(m_accumulatedInterval, tick * 5);

			readTransform(interval);
			updateActiveState();
			auto offset = applyTranslationOffset();
			stepSimulation(interval, 5, tick);
			restoreTranslationOffset(offset);
			m_accumulatedInterval = 0;
			writeTransform();
		}
	}

	btVector3 SkyrimPhysicsWorld::applyTranslationOffset()
	{
		btVector3 center;
		center.setZero();
		int count = 0;
		for (int i = 0; i < m_collisionObjects.size(); ++i)
		{
			auto rig = btRigidBody::upcast(m_collisionObjects[i]);
			if (rig)
			{
				center += rig->getWorldTransform().getOrigin();
				++count;
			}
		}

		if (count > 0)
		{
			center /= count;
			for (int i = 0; i < m_collisionObjects.size(); ++i)
			{
				auto rig = btRigidBody::upcast(m_collisionObjects[i]);
				if (rig)
				{
					rig->getWorldTransform().getOrigin() -= center;
				}
			}
			return center;
		}
		return center;
	}

	void SkyrimPhysicsWorld::restoreTranslationOffset(const btVector3& offset)
	{
		for (int i = 0; i < m_collisionObjects.size(); ++i)
		{
			auto rig = btRigidBody::upcast(m_collisionObjects[i]);
			if (rig)
			{
				rig->getWorldTransform().getOrigin() += offset;
			}
		}
	}

	void SkyrimPhysicsWorld::updateActiveState()
	{
		struct Group
		{
			std::unordered_set<IDStr> tags;
			std::unordered_map<IDStr, std::vector<SkyrimBody*>> list;
		};

		std::unordered_map<NiNode*, Group> maps;

		IDStr invalidString;
		for (auto& i : m_systems)
		{
			auto system = static_cast<SkyrimSystem*>(i());
			auto& map = maps[system->m_skeleton];
			for (auto& j : i->m_meshes)
			{
				auto shape = static_cast<SkyrimBody*>(j());
				if (!shape) continue;

				if (shape->m_disableTag == invalidString)
				{
					for (auto& k : shape->m_tags)
						map.tags.insert(k);
				}
				else
				{
					map.list[shape->m_disableTag].push_back(shape);
				}
			}
		}

		for (auto& i : maps)
		{
			for (auto& j : i.second.list)
			{
				if (i.second.tags.find(j.first) != i.second.tags.end())
				{
					for (auto& k : j.second)
						k->m_disabled = true;
				}
				else if (j.second.size())
				{
					std::sort(j.second.begin(), j.second.end(), [](SkyrimBody* a, SkyrimBody* b)
					{
						if (a->m_disablePriority != b->m_disablePriority)
							return a->m_disablePriority > b->m_disablePriority;
						return a < b;
					});

					for (auto& k : j.second)
						k->m_disabled = true;
					j.second[0]->m_disabled = false;
				}
			}
		}
	}

	void SkyrimPhysicsWorld::addSkinnedMeshSystem(SkinnedMeshSystem* system)
	{
		std::lock_guard<decltype(m_lock)> l(m_lock);
		auto s = dynamic_cast<SkyrimSystem*>(system);
		if (!s) return;

		s->m_initialized = false;
		SkinnedMeshWorld::addSkinnedMeshSystem(system);
	}

	void SkyrimPhysicsWorld::removeSkinnedMeshSystem(SkinnedMeshSystem* system)
	{
		std::lock_guard<decltype(m_lock)> l(m_lock);

		SkinnedMeshWorld::removeSkinnedMeshSystem(system);
	}

	void SkyrimPhysicsWorld::removeSystemByNode(void* root)
	{
		std::lock_guard<decltype(m_lock)> l(m_lock);

		for (int i = 0; i < m_systems.size();)
		{
			Ref<SkyrimSystem> s = m_systems[i].cast<SkyrimSystem>();
			if (s && s->m_skeleton == root)
				SkinnedMeshWorld::removeSkinnedMeshSystem(s);
			else ++i;
		}
	}

	void SkyrimPhysicsWorld::resetTransformsToOriginal()
	{
		std::lock_guard<decltype(m_lock)> l(m_lock);
		SkinnedMeshWorld::resetTransformsToOriginal();
	}


	void SkyrimPhysicsWorld::resetSystems()
	{
		std::lock_guard<decltype(m_lock)> l(m_lock);
		for (auto& i : m_systems)
			i->readTransform(RESET_PHYSICS);
	}

	void SkyrimPhysicsWorld::onEvent(const FrameEvent& e)
	{
		auto mm = *g_ui;

		if ((e.gamePaused || mm->numPauseGame) && !m_suspended)
			suspend();
		else if (!(e.gamePaused || mm->numPauseGame) && m_suspended)
			resume();

		std::lock_guard<decltype(m_lock)> l(m_lock);
		float interval = 1.0;// *(float*)(RelocationManager::s_baseAddr + offset::GameStepTimer_SlowTime);

		if (interval > FLT_EPSILON && !m_suspended && !m_systems.empty())
		{
			doUpdate(interval);
		}
		else if (m_suspended && !m_loading)
		{
			writeTransform();
		}
	}

	void SkyrimPhysicsWorld::onEvent(const ShutdownEvent& e)
	{
		for (auto system : m_systems)
		{
			for (int i = 0; i < system->m_meshes.size(); ++i)
				removeCollisionObject(system->m_meshes[i]);
			for (int i = 0; i < system->m_constraints.size(); ++i)
				removeConstraint(system->m_constraints[i]->m_constraint);
			for (int i = 0; i < system->m_bones.size(); ++i)
				removeRigidBody(&system->m_bones[i]->m_rig);

			for (auto i : system->m_constraintGroups)
				for (auto j : i->m_constraints)
					removeConstraint(j->m_constraint);
		}

		m_systems.clear();
	}

	//EventResult SkyrimPhysicsWorld::ReceiveEvent(F4SECameraEvent* evn, void * dispatcher)
	//{
	//	if (evn && evn->oldState && evn->newState)
	//		if (evn->oldState->stateID == 0 && evn->newState->stateID == 9)
	//		{
	//			m_resetPc = 3;
	//		}
	//	return kEvent_Continue;
	//}
}
