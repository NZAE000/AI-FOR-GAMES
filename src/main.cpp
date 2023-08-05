#include <iostream>
#include <engine/man/entityMan.hpp>
#include <app/util/goFactory.hpp>
#include <app/sys/renderSys.hpp>
#include <app/sys/physicSys.hpp>
#include <app/sys/inputSys.hpp>
#include <app/sys/collSys.hpp>
#include <app/sys/cameraSys.hpp>
#include <app/sys/AISys.hpp>
#include <app/cmp/physicsCmp.hpp>
#include <app/cmp/AICmp.hpp>
//#include <cmath>

constexpr uint32_t WIDTH_SCR  { 800 };
constexpr uint32_t HEIGHT_SCR { 600 };
constexpr uint32_t FPS { 60 };

ENG::Entity_t& createObserver(GOFactory_t&);
void createEntities(GOFactory_t&);

int main(void)
try 
{
	//float angle = std::atan2(1, -1);
	//std::cout<<angle<< " : "<<(angle*360)/(2*PhysicsCmp_t::PI);
	ENG::EntityManager_t entMan(10000);
	GOFactory_t goFactory;
	goFactory.setMan(entMan);
	createEntities(goFactory);

	RenderSys_t    RenSys(WIDTH_SCR, HEIGHT_SCR);
	PhysicSys_t    PhySys{};
	InputSys_t     InpSys{};
	//CollisionSys_t CollSys(WIDTH_SCR, HEIGHT_SCR);
	//CameraSys_t    CamSys{};
	AISys_t		   AISys{};

	while(!InpSys.isExitEvent()) // !DEVSys.finished() 
	{
		RenSys.update(entMan);
		InpSys.update(entMan);
		PhySys.update(entMan, 1.0/FPS);
		AISys.update(entMan);
		//CollSys.update(entMan);
		//CamSys.update(entMan);
	}

	return 0;
}
catch (const std::exception& e){
	std::cerr<< "[[EXCP]]: " << e.what() << '\n';
}
catch (...){
	std::cerr<< "[[EXCP]]: " << '\n';
}


ENG::Entity_t& createObserver(GOFactory_t& goFactory)
{
	auto& observer = goFactory.createObserver(50, 100);
	goFactory.createCamera(0, 0, WIDTH_SCR-1, HEIGHT_SCR-1, observer.getID());
	
	return observer;
}

void createEntities(GOFactory_t& goFactory)
{
	auto& obs = createObserver(goFactory);
	obs.getComponent<PhysicsCmp_t>()->friction = .9;

	// ENTITY-1 WITH AI
	auto& ent1   = goFactory.createVisitor(500, 300, obs.getID());
	auto* aiCmp1 = ent1.getComponent<AICmp_t>();
	aiCmp1->targetActive  = true;
	aiCmp1->arrivalRadius = 15.0;
	aiCmp1->arrivalTime   = 0.5;
	aiCmp1->stBehavior	  = AICmp_t::SB::SEEK;

	// ENTITY-2 WITH AI
	auto& ent2   = goFactory.createVisitor(100, 450, obs.getID());
	auto* aiCmp2 = ent2.getComponent<AICmp_t>();
	aiCmp2->targetActive  = true;
	aiCmp2->arrivalRadius = 5.0;
	aiCmp2->arrivalTime   = 1.0;
	aiCmp2->stBehavior	  = AICmp_t::SB::PURSUE;
}