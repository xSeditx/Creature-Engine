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
	idTag results = ComponentTypes.size();
    ComponentTypes.push_back({ _createfunc ,_deletefunc ,_size });
    ComponentTypes.back().DebugTypeID = ComponentTypes.size();
    return results;
}     
 