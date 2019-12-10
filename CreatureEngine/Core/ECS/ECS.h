#pragma once
/*=======================================================================*/
/*               # Entity Component System Module  #                     */
/*=======================================================================*/



/*************************************************************************/
/*                       This file is part of:                           */
/*                      Unknown_Engine ENGINE                            */
/*              https://github.com/xSeditx/Unknown-GameEngine            */
/*************************************************************************/
/* Copyright (c) 2019 Sedit                                              */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*=======================================================================*/


#include<vector>
#include<utility>
#include<unordered_map>

#include"Core\Common.h"

#include"ECScomponent.h"
#include"ECSsystem.h"

#define NULL_ENTITY     nullptr

using Entity_ID = unsigned int;


class CREATURE_API ECSListener
{
public:
	virtual void onMakeEntity(EntityPTR handle) {}
	virtual void onRemoveEntity(EntityPTR handle) {}
	virtual void onAddComponent(EntityPTR handle, Component_Handle id) {}
	virtual void onRemoveComponent(EntityPTR handle, Component_Handle id) {}

	const std::vector<Component_Handle>& g_ComponentIDs()
	{
		return ComponentIDs;
	}

	inline bool shouldNotifyOnAllComponentOperations()
	{
		return notifyOnAllComponentOperations;
	}
	inline bool shouldNotifyOnAllEntityOperations()
	{
		return notifyOnAllEntityOperations;
	}

protected:
	void setNotificationSettings(bool shouldNotifyOnAllComponentOperations, bool shouldNotifyOnAllEntityOperations)
	{
		notifyOnAllComponentOperations = shouldNotifyOnAllComponentOperations;
		notifyOnAllEntityOperations = shouldNotifyOnAllEntityOperations;
	}
	void addComponentID(Component_Handle id) {
		ComponentIDs.push_back(id);
	}
private:
	std::vector<Component_Handle> ComponentIDs;
	bool notifyOnAllComponentOperations = false;
	bool notifyOnAllEntityOperations = false;
};


//-----------------------------------------------------------------------------------
class CREATURE_API EntityComponentSystem
{
public:
	NO_COPY_OR_ASSIGNMENT(EntityComponentSystem);

	EntityComponentSystem() = default;
	~EntityComponentSystem();


	// Component Methods
	template<class T_component> inline void AddComponent(EntityPTR _entity, T_component* component)
	{
		AddComponentInternal(_entity, HandleToEntity(_entity), T_component::ID, component);
		for_loop(i, Listeners.size())
		{
			const std::vector<idTag>& componentIDs = Listeners[i]->g_ComponentIDs();
			if (Listeners[i]->shouldNotifyOnAllComponentOperations())
			{
				Listeners[i]->onAddComponent(_entity, T_component::ID);
			}
			else
			{
				for_loop(j, componentIDs.size())
				{
					if (componentIDs[j] == T_component::ID)
					{
						Listeners[i]->onAddComponent(_entity, T_component::ID);
						break;
					}
				}
			}
		}
	}
	template<class T_component> void RemoveComponent(EntityPTR _entity)
	{
		for_loop(i, Listeners.size())
		{
			const std::vector<idTag>& ComponentIDs = Listeners[i]->g_ComponentIDs();

			for_loop(j, ComponentIDs.size())
			{
				if (Listeners[i]->shouldNotifyOnAllComponentOperations()) {
					Listeners[i]->onRemoveComponent(_entity, T_component::ID);
				}
				else
				{
					if (ComponentIDs[j] == Component::ID)
					{
						Listeners[i]->onRemoveComponent(_entity, T_component::ID);
						break;
					}
				}
			}
		}
		return RemoveComponentInternal(_entity, Component::ID);
	}
	template<class T_component> T_component* g_Component(EntityPTR _entity)
	{
		return (T_component*)g_ComponentInternal(HandleToEntity(_entity), Components[T_component::ID], T_component::ID);
	}
	void ComponentDelete(Component_Handle _componentID, unsigned int index);

	//System Methods
	void UpdateSystems(SystemList& systems, float delta);


	// Entity Methods
	EntityPTR MakeEntity(BaseComponent **_comp, const Component_Handle *_componentIDs, size_t _size);
	template<class ...ARGS>  EntityPTR MakeEntity(ARGS&... _args)
	{
		BaseComponent* components[] = { &_args... };
		unsigned int componentIDs[] = { ARGS::ID... };
		return MakeEntity(components, componentIDs, sizeof...(ARGS));
	}

	void RemoveEntity(EntityPTR _handle);

private:
	BaseComponent* g_ComponentInternal(EntityHandle_t& entityComponents, std::vector<uint8_t>& array, Component_Handle componentID);

	void AddComponentInternal(EntityPTR handle, EntityHandle_t& entity, Component_Handle componentID, BaseComponent* component);
	bool RemoveComponentInternal(EntityPTR _handle, Component_Handle _componentID);

	std::vector<Entity_t* >  Entities;
	Component_map            Components;
	std::vector<BaseSystem*> Systems;
	std::vector<ECSListener*> Listeners;

	uint32_t HandleToEntityIndex(EntityPTR _handle)
	{
		return RawTypeHandle(_handle)->first;
	}
	Entity_t* RawTypeHandle(EntityPTR _handle)
	{
		return (Entity_t*)_handle;
	}
	EntityHandle_t& HandleToEntity(EntityPTR _handle)
	{
		return RawTypeHandle(_handle)->second;
	}



	void UpdateSystemWithMultipleComponents
	(//TODO: Fix this Clusterfuck
		uint32_t index,
		SystemList& systems,
		const std::vector<uint32_t>& _componentTypes,
		std::vector<BaseComponent*>& _componentParam,
		std::vector<std::vector<uint8_t>*>& _componentArrays,
		float delta
	);

	idTag FindLeastCommonComponent(const std::vector<Component_Handle>& componentTypes, const std::vector<Component_Handle>& componentFlags);
	static unsigned int EntityCount;

};




class TestSystem : public BaseSystem
{
	TestSystem(std::vector<Component_Handle>& comptypeIn)
		:
		BaseSystem()
	{}

	void UpdateComponents(float _delta, BaseComponent **comp)
	{}
};
