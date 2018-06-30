#pragma once
#include "MemoryManager.h"
#include "EntityManager.h"
#include <vector>
#include <algorithm>
#define PI 3.14159

class AimAssit
{

	private:
		MemoryManager *mem;										////Pointer to Memory manager
		EntityManger *ent;										////Pointer to Entity manager
		OffSets *offsets;										////Pointer to the offsets
		bool crouched;											////Is player crouched
		bool recoil;											////Fix for spray control
		int time;												////Time for recoil to come into play
		float fov;												////Fov circle radius
		float totalDistance;									////Total distance from crosshair
		float My_Angle_X;										////Temp angleX for local
		float My_Angle_Y;										////Temp angleY for local
		float lerpValue;										////Lerp values
		float tempAnglesX[64];									////Pointer to tempAngleX
		float tempAnglesY[64];									////Pointer to tempAngleY
		std::vector<EntityPlayers*> Enemies;					////Vector of pointers to the entities
		LocalPlayer *m_local;									////Pointer to m_local
		void GetValues();										////Calculate all values
		void isCrouched();										////Check if player crouched
		void hasRecoil();
		float Lerp(float, float, float);						////Smooths a set of float values
		float normalizeAngle(float, float, float);				////Check angles are real
		Vector3 TargetRelativeToPlayer(Vector3, Vector3, bool);	////Target relative to player(0, 0, 0)

	public:
		AimAssit();
		~AimAssit();
		bool Initialize(MemoryManager*, EntityManger*, OffSets*);
		void DoAimAssit();
		void TestLock();
};

