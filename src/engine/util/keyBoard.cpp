#include <engine/util/keyBoard.hpp>

namespace ENG {

KeyBoard_t::opIterKey
KeyBoard_t::getKeyIterator(KeySym key) noexcept
{
	opIterKey itOpt = m_keys.find(key);
	return (*itOpt != end(m_keys))? itOpt : opIterKey{}; // si no existe la tecla, devuelve un optional con nada mediante constructor por defecto.
}

bool KeyBoard_t::isKeyPress(KeySym key) noexcept 
{
	opIterKey itOpt = getKeyIterator(key);
	return (itOpt)? (*itOpt)->second : false;
}

void KeyBoard_t::keyPressed(KeySym key)  noexcept { setKey(key, true);  }
void KeyBoard_t::keyReleased(KeySym key) noexcept { setKey(key, false); }

void KeyBoard_t::setKey(KeySym key, bool state) noexcept
{
	if (auto it = getKeyIterator(key)) (*it)->second = state;
}

void KeyBoard_t::reset() noexcept
{
	for (auto& [___, val] : m_keys) val = false;
}

}
