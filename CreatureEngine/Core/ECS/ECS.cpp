/*=======================================================================*/
/*               # Entity Component System Module  #                     */
/*=======================================================================*/

/*************************************************************************/
/*                       This file is part of:                           */
/*                      Unknown_Engine ENGINE                            */
/*              https://github.com/xSeditx/Unknown-GameEngine            */
/*************************************************************************/
/* Copyright (c) 2019-2019 Sedit                                         */
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


#include<algorithm>

#include"Core\ECS\ECS.h"

using Entity_ID = unsigned int;

//-----------------------------------------------------------------------------------
typedef std::unordered_map <Component_Handle, std::vector<uint8_t>> Component_map;
namespace Unknown_Engine
{
	namespace EntityComponentSystem
	{
		EntityComponentSystem::~EntityComponentSystem()
		{
			for (Component_map::iterator it = Components.begin(); it != Components.end(); ++it)
			{
				size_t typeSize = BaseComponent::g_TypeSize(it->first);
				ComponentDeleteFunc freefn = BaseComponent::g_TypeDeleteFunction(it->first);
				for (uint32_t i = 0; i < it->second.size(); i += static_cast<uint32_t>(typeSize))
				{
					freefn((BaseComponent*)&it->second[i]);
				}
			}

			for_loop(i, Entities.size())
			{
				delete Entities[i];
			}
		}

		//Entity Methods
		EntityPTR EntityComponentSystem::MakeEntity(BaseComponent ** _comp, const Component_Handle * _componentIDs, size_t _size)
		{
			Entity_t *newEntity = new Entity_t();
			EntityPTR results = (EntityPTR)newEntity;

			for_loop(i, _size)
			{
				if (!BaseComponent::is_type_valid(_componentIDs[i]))
				{
					__debugbreak();
					//  DEBUG_LOG("ECS", LOG_ERROR, "'%u' is not a valid component type.", componentIDs[i]);
					//  delete newEntity;
					//  return NULL_ENTITY_HANDLE;
				}

				AddComponentInternal(results, newEntity->second, _componentIDs[i], _comp[i]);
			}

			newEntity->first = static_cast<decltype(newEntity->first)>(Entities.size());
			Entities.push_back(newEntity);

			for_loop(i, Listeners.size())
			{
				bool isValid = true;
				if (Listeners[i]->shouldNotifyOnAllEntityOperations())
				{
					Listeners[i]->onMakeEntity(results);
				}
				else
				{
					for_loop(j, Listeners[i]->g_ComponentIDs().size())
					{
						bool hasComponent = false;
						for_loop(k, _size)
						{
							if (Listeners[i]->g_ComponentIDs()[j] == _componentIDs[k])
							{
								hasComponent = true;
								break;
							}
						}
						if (!hasComponent)
						{
							isValid = false;
							break;
						}
					}
					if (isValid)
					{
						Listeners[i]->onMakeEntity(results);
					}
				}
			}

			return results;
		}


		void EntityComponentSystem::RemoveEntity(EntityPTR _handle)
		{
			EntityHandle_t& entity = HandleToEntity(_handle);

			for_loop(i, Listeners.size())
			{
				const std::vector<Component_Handle>& componentIDs = Listeners[i]->g_ComponentIDs();
				bool isValid = true;
				if (Listeners[i]->shouldNotifyOnAllEntityOperations()) {
					Listeners[i]->onRemoveEntity(_handle);
				}
				else {
					for_loop(j, componentIDs.size())
					{
						bool hasComponent = false;
						for_loop(k, entity.size())
						{
							if (componentIDs[j] == entity[k].first)
							{
								hasComponent = true;
								break;
							}
						}
						if (!hasComponent)
						{
							isValid = false;
							break;
						}
					}
					if (isValid) {
						Listeners[i]->onRemoveEntity(_handle);
					}
				}
			}

			for_loop(i, entity.size())
			{
				ComponentDelete(entity[i].first, entity[i].second);
			}

			idTag destIndex = HandleToEntityIndex(_handle);
			idTag srcIndex = static_cast<uint32_t>(Entities.size() - 1);
			delete Entities[destIndex];
			Entities[destIndex] = Entities[srcIndex];
			Entities[destIndex]->first = destIndex;
			Entities.pop_back();
		}

		void EntityComponentSystem::AddComponentInternal(EntityPTR handle, EntityHandle_t & entity, Component_Handle componentID, BaseComponent* component)
		{
			ComponentCreateFunc createfn = BaseComponent::g_TypeCreateFunction(componentID);
			std::pair<uint32_t, uint32_t> newPair;
			newPair.first = componentID;
			newPair.second = createfn(Components[componentID], handle, component);
			entity.push_back(newPair);
		}
		bool EntityComponentSystem::RemoveComponentInternal(EntityPTR _handle, Component_Handle _componentID)
		{
			EntityHandle_t& EntityComponents = HandleToEntity(_handle);
			for_loop(I, EntityComponents.size())
			{
				if (_componentID == EntityComponents[I].first)
				{
					///IMPLEMENT THIS	DeleteComponent(EntityComponents[I].first, EntityComponents[I].second);
					uint32_t DestinationIndex = I;
					uint32_t SourceIndex = static_cast<uint32_t>( EntityComponents.size() - 1);
					EntityComponents[DestinationIndex] = EntityComponents[SourceIndex];
					EntityComponents.pop_back();
					return true;
				}
			}
			return false;
		}

		BaseComponent * EntityComponentSystem::g_ComponentInternal(EntityHandle_t & entityComponents, std::vector<uint8_t>& array, Component_Handle componentID)
		{
			for_loop(i, entityComponents.size())
			{
				if (componentID == entityComponents[i].first)
				{
					return (BaseComponent*)&array[entityComponents[i].second];
				}
			}
			return nullptr;
		}


		void EntityComponentSystem::ComponentDelete(Component_Handle _componentID, unsigned int index)
		{
			std::vector<uint8_t>& ComponentArray = Components[_componentID];
			ComponentDeleteFunc FreeFunction = BaseComponent::g_TypeDeleteFunction(_componentID);
			uint32_t typeSize = BaseComponent::g_TypeSize(_componentID);

			uint32_t SourceIndex = static_cast<uint32_t>(ComponentArray.size() - typeSize);

			BaseComponent* destComponent = (BaseComponent*)&ComponentArray[index];
			BaseComponent* srcComponent = (BaseComponent*)&ComponentArray[SourceIndex];
			FreeFunction(destComponent);

			if (index == SourceIndex)
			{
				ComponentArray.resize(SourceIndex);
				return;
			}
			std::memcpy(destComponent, srcComponent, typeSize);

			EntityHandle_t& srcComponents = HandleToEntity(srcComponent->Entity);
			for_loop(i, srcComponents.size())
			{
				if (_componentID == srcComponents[i].first && SourceIndex == srcComponents[i].second)
				{
					srcComponents[i].second = index;
					break;
				}
			}
			ComponentArray.resize(SourceIndex);
		}


		idTag EntityComponentSystem::FindLeastCommonComponent(const std::vector<Component_Handle>& _componentTypes, const std::vector<Component_Handle>& _componentFlags)
		{
			uint32_t minSize = (uint32_t)-1;
			uint32_t minIndex = (uint32_t)-1;
			for_loop(i, _componentTypes.size())
			{
				if ((_componentFlags[i] & BaseSystem::FLAG_OPTIONAL) != 0)
				{
					continue;
				}
				uint32_t typeSize = BaseComponent::g_TypeSize(_componentTypes[i]);
				uint32_t size = static_cast<uint32_t>(Components[_componentTypes[i]].size() / typeSize);
				if (size <= minSize)
				{
					minSize = size;
					minIndex = i;
				}
			}
			return minIndex;
		}

		void EntityComponentSystem::UpdateSystems(SystemList& _systems, float _delta)
		{// Update a list of Systems
			std::vector<BaseComponent*> componentParam;
			std::vector<std::vector<unsigned char>*> componentArrays;

			for_loop(i, _systems.size())
			{// Cycles over all systems
				const std::vector<unsigned int>& componentTypes = _systems[i]->g_ComponentTypes();

				if (componentTypes.size() == 1)
				{
					size_t typeSize = BaseComponent::g_TypeSize(componentTypes[0]);
					std::vector<uint8_t>& array = Components[componentTypes[0]];
					for (uint32_t j{ 0 }; j < array.size(); j += static_cast<uint32_t>(typeSize))
					{
						BaseComponent* component = (BaseComponent*)&array[j];
						_systems[i]->UpdateComponents(_delta, &component);
					}
				}
				else
				{
					UpdateSystemWithMultipleComponents(i, _systems, componentTypes, componentParam, componentArrays, _delta);
				}
			}
		}// End UpdateSystems
		void EntityComponentSystem::UpdateSystemWithMultipleComponents
		(/// I despise this function and its signature
			uint32_t _index,
			SystemList& _systems,
			const std::vector<Component_Handle>& _componentTypes,
			std::vector<BaseComponent*>& _componentParam,
			std::vector<std::vector<uint8_t>*>& _componentArrays,
			float _delta
		)
		{// Updates all the Components of a given system

			_componentParam.resize(std::max(_componentParam.size(), _componentTypes.size()));
			_componentArrays.resize(std::max(_componentArrays.size(), _componentTypes.size()));

			const std::vector<uint32_t>& componentFlags = _systems[_index]->g_ComponentFlags();

			for_loop(i, _componentTypes.size())
			{
				_componentArrays[i] = &Components[_componentTypes[i]];
			}

			uint32_t minSizeIndex = FindLeastCommonComponent(_componentTypes, componentFlags);
			uint32_t typeSize = BaseComponent::g_TypeSize(_componentTypes[minSizeIndex]);
			std::vector<uint8_t>& array = *_componentArrays[minSizeIndex];

			for (uint32_t i = 0; i < array.size(); i += typeSize)
			{// Cycles over all the elements in the current Component
				_componentParam[minSizeIndex] = (BaseComponent*)&array[i];
				EntityHandle_t& entityComponents = HandleToEntity(_componentParam[minSizeIndex]->Entity);

				bool isValid = true;
				for_loop(j, _componentTypes.size())
				{
					if (j == minSizeIndex)
					{
						continue;
					}

					_componentParam[j] =
						g_ComponentInternal
						(
							entityComponents,
							*_componentArrays[j],
							_componentTypes[j]
						);

					if (_componentParam[j] == nullptr && (componentFlags[j] & BaseSystem::FLAG_OPTIONAL) == 0)
					{
						isValid = false;
						break;
					}
				}

				if (isValid)
				{
					_systems[_index]->UpdateComponents(_delta, &_componentParam[0]);
				}
			}
		}// UpdateSystemWithMultipleComponents End
	}// Entity Component Namespace
}// Unknown_Engine NameSpace


// https://www.youtube.com/watch?v=51qSGUtaJwc
// BENNY: https://www.youtube.com/watch?v=4-LRor0UG6o&list=PLEETnX-uPtBUrfzE3Dxy3PWyApnW6YEMm&index=7
// https://gamedev.stackexchange.com/questions/4898/are-there-existing-foss-component-based-frameworks/4966#4966
// OTHERDUDE: https://www.youtube.com/watch?v=WbwXxms80Z4
// Springs: https://www.myphysicslab.com/springs/single-spring-en.html








