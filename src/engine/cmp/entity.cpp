#include "entity.hpp"

namespace ENG {

void Entity_t::updateCmp(CmpType_t cmpType, ComponentBase_t* cmp) noexcept
{   
    auto iter = components.find(cmpType); //findIteratorCmp(type);
    if (iter == components.end()) return;

    iter->second = cmp;
}

}