#pragma once

#include <vector>
#include <algorithm>
#include "MemoryManager.h"
#include "EntityManager.h"
#define PI 3.14159


class AimLock
{
	private:
		MemoryManager *mem;								////Pointer to MemoryManager
		EntityManger *ent;								////Pointer to EntityManager
		OffSets *offsets;								////Pointer to offsets
		float *tempAngles;								////Store temp Angle for cross referencing
		std::vector<EntityPlayers*> Enemies;			////Vector of pointers to enemy structs
		LocalPlayer *m_local;							////Pointer to local struct
		/////////////////////////////////////
		//////////////functions//////////////
		/////////////////////////////////////
		void GetValues();									////Calculates all values, distance, mag, relative pos ect
		Vector3 TargetRelativeToPlayer(Vector3, Vector3);	////Calculates relative postion in respect to local


	public:
		AimLock();
		~AimLock();
		bool Initialize(MemoryManager*, EntityManger*, OffSets*);
		void DoAimLock();


};


