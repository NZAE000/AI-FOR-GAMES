#pragma once
#include <engine/util/typeAliasess.hpp>
#include <engine/man/cmpVectorBase.hpp>

namespace ENG {

struct ComponentStorage_t {

	explicit ComponentStorage_t(std::size_t numComponents)
	: numInitialCmps (numComponents)
	{}

	template<typename CMP_t>
	CMP_t& createCmp(EntID_t eid)
	{
		Vec_t<CMP_t>& vecCmp = getCmps<CMP_t>();
		return vecCmp.emplace_back(eid);
	}

	template<typename CMP_t>
	Vec_t<CMP_t>& createCmpVector()
	{
		auto cmpVec = std::make_unique<CmpVector_t<CMP_t>>();
		cmpVec->components.reserve(numInitialCmps);
		auto* ptr_cmpVec = cmpVec.get();

		auto cmpType = CMP_t::getCmpType();
		componentVectors[cmpType] = std::move(cmpVec);

		return ptr_cmpVec->components;
	}

	template<typename CMP_t>
	Vec_t<CMP_t>& getCmps()
	{
		Vec_t<CMP_t>* vecCmp { nullptr };
		auto cmpType = CMP_t::getCmpType();
		auto it 	 = componentVectors.find(cmpType);

		if (it != componentVectors.end()){
			auto* cmpVec = dynamic_cast<CmpVector_t<CMP_t>*>(it->second.get());
			vecCmp = &cmpVec->components;
		}
		else vecCmp = &createCmpVector<CMP_t>();

		return *vecCmp;
	}

	template<typename CMP_t>
	const Vec_t<CMP_t>& getCmps() const
	{
		Vec_t<CMP_t>* vecCmp { nullptr };
		auto cmpType = CMP_t::getCmpType();
		auto it 	 = componentVectors.find(cmpType);

		if (it != componentVectors.end()){
			auto* cmpVec = dynamic_cast<CmpVector_t<CMP_t>*>(it->second.get());
			vecCmp = &cmpVec->components;
		}
		else throw "Components not found"; // <-- excep.

		return *vecCmp;
	}

	ComponentBase_t* destroyCmp(CmpType_t, EntID_t);

private:
	Hash_t<CmpType_t, UPTR_t<CmpVectorBase_t>> componentVectors;
	const std::size_t numInitialCmps;
};

}