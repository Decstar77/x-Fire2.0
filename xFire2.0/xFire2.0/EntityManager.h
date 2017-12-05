#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "MemoryManager.h"
struct LocalPlayer
{
	DWORD BaseAddress;				////BaseAddress
	DWORD ClientState;				////Client base address
	int Team;						////Team number
	int Dorm;						////Dormancy
	int Health;						////Health
	float AngleX;					////Crosshair angle X
	float AngleY;					////Crosshair angle Y
	Vector3 Position;				////vec3 position
};

struct EntityPlayers
{
	bool Spotted = false;
	DWORD BaseAddress = NULL;		////BaseAddress
	DWORD GlowBaseAddress = NULL;	////GlowAddress for glow manager
	DWORD tempAddress = NULL;		////Address used for checking/cross referencing.
	int Team;						////Team number
	int Dorm;						////Dormancy
	int Health;
	float BaseAngleX;				////The base x-angle, first angle read. To cross reference. The angles that are relavent to the player. Note: Not set by EntityManager at all
	float BaseAngleY;				////The base y-angle, first angle read. To cross reference. The angles that are relavent to the player. Note: Not set by EntityManager at all
	float distanceAngle = 400;		////Distance from LocalPlayer crosshair, to the entity. (ViewMatrix wise)
	Vector3 Position;				////Position of entity
	Vector3 RelativePosition;		////Relative postion to local
};

class EntityManger
{
	private:
		MemoryManager *mem;			////Pointer to MemoryManager 
		OffSets *offsets;			////Pointer to offsets
		LocalPlayer Local;									////The player himself
		std::vector<EntityPlayers> Players;					////All other players, team and enemy included
		std::vector<EntityPlayers> PlayersFriendly;
		std::vector<EntityPlayers> PlayersEnemy;
		DWORD EntityListBase;
		int CurrentAmount;
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////Functions///////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		void getLocal();									////Gets values for the local player
		void getEntities();									////Gets values for all the entities
		void organizeByTeam();								////Sets PlayerFriendly values ect


	public:
		EntityManger();									//__Init__ of class //// Doesn't do much, all of the real magic happens in function Initialize();
		~EntityManger();
		int GetAmountOf(bool);							////Returns the size of the specifed vector. true for freindlies;
		bool Initialize(MemoryManager *, OffSets *);
		void UpdateEntitiesFull();						////Update team, dormancy state and position. Rechecks all base address And organises by Team
		void UpdateEntitiesQuick();						////Update Angles and position. Note: Requires teams to be organised. Does not recheck all base address;
		void UpdateLocalFull();							////Updates all local attributes. Includes base address. Just calls getLocal()
		void UpdateLocalQuick();						////Updates everything but base address. 
		EntityPlayers *GetEntityByTeam(bool, int);		////Get ents by team. Returns a pointer to each element. True for on local team
		LocalPlayer *GetLocalPlayer();					////Get localPlayer. Returns a pointer to this->Local


};


