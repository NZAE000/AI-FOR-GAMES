#pragma once
#include <engine/cmp/component.hpp>
#include <optional>

namespace ENG {

struct CmpVectorBase_t {

	virtual ~CmpVectorBase_t() = default;
	virtual ComponentBase_t* removeCmp(EntID_t) = 0;
};

template<typename CMP_t>
struct CmpVector_t : public CmpVectorBase_t {

	ComponentBase_t* removeCmp(EntID_t eid) override final
	{
		// linear search... CHANGE!!   
		auto opIt = findIteratorCmp(eid);
		if (!opIt) return nullptr;

		auto it = *opIt; // get iterator

		// components.erase(it); NOOOOO!!!
		// Para evitar el desplazamiento de elementos de un vector(si esque se elemina un elemento entre el rango) 
		// y punteros externos que apuntan a aquellos sitios del vector el cual fueron destruidos devido al desplazamiento,
		// la solucion (y no tan optima) es hacer una copia de los valores de el ultimo componente del vector a la posición del componente
		// que sea desea eliminar, y desde luego habra que actualizar la posicion en referencia a ese componente en la entidad. 
		if ((it+1) != end(components)) *it = components.back();	

		//std::cout<<"Cmp " << it->getCmpType() << " of Entity "<< eid <<"\n";
		
		components.pop_back();

		return it.base();
	}

	Vec_t<CMP_t> components;

private:

	constexpr auto findIteratorCmp(EntID_t eid) // find_if thow excep.
	{
		std::optional itOp = std::find_if(components.begin(), components.end()
		, [eid](auto& cmp)
		{
			return cmp.getEntityID() == eid;
		});

		if (*itOp == end(components)) itOp.reset();

		return itOp;
	}
};

}