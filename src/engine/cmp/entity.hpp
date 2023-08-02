#include <engine/util/typeAliasess.hpp>
#include "component.hpp"

namespace ENG {

struct Entity_t {
	
	explicit Entity_t() = default;
	
	template<typename CMP_t>
	const CMP_t* getComponent() const
	{
		auto cmpType = CMP_t::getCmpType();
		auto it = components.find(cmpType);

		return (it != components.end())? static_cast<CMP_t*>(it->second) : nullptr;
	}

	template<typename CMP_t>
	CMP_t* getComponent()
	{
		auto* cmp = const_cast<const Entity_t*>(this)->getComponent<CMP_t>();
		return const_cast<CMP_t*>(cmp);
	}

	template<typename CMP_t>
	void addComponent(CMP_t& cmp)
	{
		auto cmpType = CMP_t::getCmpType();
		components[cmpType] = &cmp; 
	}

	EntID_t getID() const noexcept { return eid; }
	void updateCmp(CmpType_t, ComponentBase_t*) noexcept;

	Hash_t<CmpType_t, ComponentBase_t*>::iterator begin() 		            { return components.begin(); }
    Hash_t<CmpType_t, ComponentBase_t*>::iterator end()                     { return components.end();   }
    const Hash_t<CmpType_t, ComponentBase_t*>::const_iterator begin() const { return components.begin(); }
    const Hash_t<CmpType_t, ComponentBase_t*>::const_iterator end()   const { return components.end();   }

private:
	Hash_t<CmpType_t, ComponentBase_t*> components;

	EntID_t eid { ++ nextEntityID };
	inline static std::size_t nextEntityID { 0 };
		
};

}
