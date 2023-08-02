#include "inputSys.hpp"
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/inputCmp.hpp>
#include <engine/man/entityMan.hpp>

extern "C" {
	#include <tinyPTC.ua/src/tinyptc.h>
}

InputSys_t::InputSys_t() { setOn(); }

void InputSys_t::setOn() const noexcept
{
	// Set pointer to func
	ptc_set_on_keypress(onKeyPress);
	ptc_set_on_keyrelease(onKeyRelease);

	// Set all keys to realease
	keyboard.reset();
}

void InputSys_t::onKeyPress(KeySym key) noexcept
{
	keyboard.keyPressed(key);
}

void InputSys_t::onKeyRelease(KeySym key) noexcept
{
	keyboard.keyReleased(key);
}

bool InputSys_t::isKeyPress(KeySym key) const noexcept
{
	return keyboard.isKeyPress(key);
}

bool InputSys_t::isExitEvent() const noexcept
{
	return isKeyPress(XK_Escape);
}

void InputSys_t::update(ENG::EntityManager_t& entMan) const
{
	ptc_process_events();

	auto& inputCmps = entMan.getCmps<InputCmp_t>();
	for (auto& incmp : inputCmps) 
	{	
		auto* phycmp = entMan.template getRequiredCmp<PhysicsCmp_t>(incmp);
		if (!phycmp) continue;

		phycmp->aLinear = phycmp->vAngular = 0;

		const ENG::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>>& actions = incmp.getActions();
		ENG::Hash_t<KeySym, std::function<void(PhysicsCmp_t&)>>::const_iterator nextAction 
		= actions.begin();

		while(nextAction != actions.end())
		{
			if (keyboard.isKeyPress(nextAction->first))
				nextAction->second(*phycmp);
			++nextAction;
		}
	}
}