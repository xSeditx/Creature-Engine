#include"Core\ECS\ECScomponent.h"
  /// COMPLETE
std::vector<BaseComponent::Properties> BaseComponent::ComponentTypes;


_static idTag BaseComponent::RegisterComponentType
(
    ComponentCreateFunc _createfunc,
    ComponentDeleteFunc _deletefunc,
    size_t _size
)
{
	idTag results = static_cast<idTag>(ComponentTypes.size());
    ComponentTypes.push_back({ _createfunc ,_deletefunc ,_size });
    ComponentTypes.back().DebugTypeID = static_cast<idTag>(ComponentTypes.size());
    return results;
}     
 