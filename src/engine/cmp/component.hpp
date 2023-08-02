#pragma once
#include <engine/util/typeAliasess.hpp>


namespace ENG {

struct ComponentBase_t {

	explicit ComponentBase_t(EntID_t eid) : entID{eid} {}
	constexpr EntID_t getEntityID() const noexcept { return entID; }

protected:
	inline static std::size_t nextCmpType {0};

private:
	EntID_t entID;

};

template<typename CMP_t> // CRTP: CURIOSLY RECURRING TEMPLATE PATTERN
struct Component_t : public ComponentBase_t {
	
	explicit Component_t(EntID_t eid) : ComponentBase_t(eid) {}

	static CmpType_t getCmpType() noexcept
	{ 
		static CmpType_t cmpType { ++nextCmpType };
		return cmpType;
	}
};

}
