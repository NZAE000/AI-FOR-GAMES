#include "cmpStorage.hpp"

namespace ENG {

ComponentBase_t* ComponentStorage_t::
destroyCmp(CmpType_t cmptype, EntID_t eid)
{
	auto iter = componentVectors.find(cmptype); //findIteratorUptrCmpVec(cmptype); // busco el UPTRtoCmpVector(vector) asociado al typo cmp
	if (iter == componentVectors.end()) return nullptr;

	return iter->second.get()->removeCmp(eid); // iterador a un 'pair'(key, value), entonces se obtiene el second(value), aquel es un UPTR, entonces obtener el puntero crudo (get()) y llamar a su metodo(->).
}

}