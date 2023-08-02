#pragma once

#include <engine/util/typeAliasess.hpp>
#include <engine/cmp/entity.hpp>
#include "cmpStorage.hpp"
#include <iostream>

namespace ENG {

struct EntityManager_t {
	
	explicit EntityManager_t(std::size_t numEnt) : cmpStorage{numEnt}
	{
		entities.reserve(numEnt);
	}

	Entity_t& createEntity() { return entities.emplace_back(); }

	Entity_t* getEntityByID(EntID_t);
	const Entity_t* getEntityByID(EntID_t) const;

	const Vec_t<Entity_t>& getEntities() const { return entities; }
	Vec_t<Entity_t>& getEntities()       { return entities; }

	template<typename CMP_t>
	Vec_t<CMP_t>& getCmps() { return cmpStorage.getCmps<CMP_t>(); }

	template<typename CMP_t>
	const Vec_t<CMP_t>& getCmps() const { return cmpStorage.getCmps<CMP_t>(); }

	template<typename CMP_t>
	CMP_t& addCmp(Entity_t& ent)
	{
		CMP_t* cmp { ent.getComponent<CMP_t>() };
		if (!cmp){
			cmp = &cmpStorage.createCmp<CMP_t>(ent.getID());
			ent.addComponent(*cmp);		
		}

		return *cmp;
	}

	template<typename CMP_REQUI, typename CMP_DELIV>
	constexpr const CMP_REQUI* getRequiredCmp(const CMP_DELIV& cmpDeliv) const // getEntityById can throw excep.
	{
	    auto* entity = getEntityByID(cmpDeliv.getEntityID());
	    if (entity) return entity->template getComponent<CMP_REQUI>();

	    return nullptr;
	}

	template<typename CMP_REQUI, typename CMP_DELIV>
	constexpr CMP_REQUI* getRequiredCmp(const CMP_DELIV& cmpDeliv)
	{
	    const CMP_REQUI* cmpReq = const_cast<const EntityManager_t*>(this)->getRequiredCmp<CMP_REQUI>(cmpDeliv);
	    return const_cast<CMP_REQUI*>(cmpReq);
	}

	void destroyEntity(EntID_t eid);

	const std::optional<Vec_t<Entity_t>::const_iterator> findIteratorEntity(EntID_t) const;
	std::optional<Vec_t<Entity_t>::iterator> findIteratorEntity(EntID_t);

private:
	Vec_t<Entity_t> entities;
	ComponentStorage_t cmpStorage;
};

}
