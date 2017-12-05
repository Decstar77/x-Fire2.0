#pragma once
#include <vector>
#include "MemoryManager.h"
#include "EntityManager.h"




class Glow
{
	private:
		MemoryManager* mem;									////Pointer to MemoryManager
		EntityManger* ent;									////Pointer to entity
		OffSets* offsets;									////Pointer to offsets
		
		
		int AmountOfGlowObjects;							////Amount of glow objs, 64
		DWORD pointerToGlow;								////Pointer to the glow manager

		std::vector<EntityPlayers*> GlowBasesFriendly;				////Friendly bases address for glow
		std::vector<EntityPlayers*> GlowBasesEnemy;					////Enemy bases address for glow
		/////////////////////////////////////////////////
		/////////////Functions///////////////////////////
		/////////////////////////////////////////////////
		void fillGlowBases();								////Get all glow bases for the vectors

	public:
		Glow();
		~Glow();
		bool Initialize(MemoryManager*, EntityManger*, OffSets*);
		void DoGlow();
};




