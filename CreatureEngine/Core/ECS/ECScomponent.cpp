#include"Core\ECS\ECScomponent.h"
  /// COMPLETE
std::vector<BaseComponent::Properties> BaseComponent::ComponentTypes;


_static idTag BaseComponent::RegisterComponentType
(
    ComponentCreateFunc _createfunc,
    ComponentDeleteFunc _deletefunc,
    uint32_t _size
)
{
	idTag results = static_cast<idTag>(ComponentTypes.size());
    ComponentTypes.push_back({ _createfunc ,_deletefunc ,_size });
    ComponentTypes.back().DebugTypeID = static_cast<idTag>(ComponentTypes.size());
    return results;
}     
 