#include "entityMan.hpp"
#include <algorithm>
#include <optional>

namespace ENG {

const Entity_t* EntityManager_t::getEntityByID(EntID_t eid) const
{
	auto it = std::find_if(entities.begin(), entities.end(), 
		[&eid](const auto& entity){
			return entity.getID() == eid;
		});

	if (it != entities.end()) return it.base();
	return nullptr;
}

Entity_t* EntityManager_t::getEntityByID(EntID_t eid)
{
	const auto ent = const_cast<const EntityManager_t*>(this)->getEntityByID(eid);
	return const_cast<Entity_t*>(ent);
}

const std::optional<Vec_t<Entity_t>::const_iterator>
EntityManager_t::findIteratorEntity(EntID_t eid) const // find_if throw excep.
{
    std::optional<Vec_t<Entity_t>::const_iterator> itOpt = std::find_if(entities.begin(), entities.end()
        , [&](const Entity_t& ent)
        {
            return ent.getID() == eid;
        });

    if (*itOpt == end(entities)) itOpt.reset(); // el opcional a iterador setearlo a 0 (nada)

    return itOpt;
}

// FIX NOW!!
std::optional<Vec_t<Entity_t>::iterator>
EntityManager_t::findIteratorEntity(EntID_t eid) // find_of throw excep.
{
    //auto itOpt = const_cast<const EntityManager_t*>(this)->findIteratorEntity(eid);
    //return const_cast<std::optional<Vec_t<Entity_t>::iterator>>(itOpt);

    std::optional<Vec_t<Entity_t>::iterator> itOpt = std::find_if(entities.begin(), entities.end()
        , [&](const Entity_t& ent)
        {
            return ent.getID() == eid;
        });

    if (*itOpt == end(entities)) itOpt.reset(); // el iterador del opcional setearlo a 0 (nada)

    return itOpt;
}


void EntityManager_t::destroyEntity(EntID_t eid) // getEntityById and others can throw excep.
{
    auto* entity = getEntityByID(eid);
    if (!entity) return;

    // Para cada componente perteneciente a esta entidad ..
    for (auto& [cmptype, _] : *entity) // ->second (cmp) unused.
    {
        auto* cmp = cmpStorage.destroyCmp(cmptype, eid);
        if (!cmp) continue;

        auto* entMoved = getEntityByID(cmp->getEntityID());
        entMoved->updateCmp(cmptype, cmp);
    }
      
    auto itOpt = findIteratorEntity(eid);
    entities.erase(*itOpt);
}

}