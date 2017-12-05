#include "EntityManager.h"

EntityManger::EntityManger()
{
	this->EntityListBase = 0x0;
	this->CurrentAmount = 0;
}
EntityManger::~EntityManger()
{
	delete mem;
	delete offsets;
}
int EntityManger::GetAmountOf(bool team)
{
	if (team)
	{
		return PlayersFriendly.size();
	}
	else
	{
		return PlayersEnemy.size();
	}
}
void EntityManger::getLocal()
{
	Local.BaseAddress = this->mem->Read<DWORD>(this->offsets->ClientDllBaseAdress + this->offsets->m_dwLocalPlayer);		////Get baseAddress
	Local.Team = this->mem->Read<int>(Local.BaseAddress + this->offsets->m_iTeamNum);								////Get team number
	Local.Dorm = this->mem->Read<int>(Local.BaseAddress + this->offsets->m_dwDormant);								////Get dormancy
	Local.Position = this->mem->Read<Vector3>(Local.BaseAddress + this->offsets->m_vecOrigin);						////Get vec3 position
	Local.ClientState = this->mem->Read<DWORD>(this->offsets->EngineDllBaseAdress + this->offsets->dwClientState);		////Get client state base address
	Local.AngleX = this->mem->Read<float>(Local.ClientState + this->offsets->dwClientState_ViewAngles + 0x4);		////Get crosshair anglex
	Local.AngleY = this->mem->Read<float>(Local.ClientState + this->offsets->dwClientState_ViewAngles);				////Get crosshair angley
}
void EntityManger::getEntities()
{
	this->EntityListBase = this->offsets->ClientDllBaseAdress + this->offsets->m_dwEntityList;
	for (int i = 1; i < this->offsets->MagicNumber; i++)														////Start at 1 to ignore LocalPlayer
	{
		DWORD current_ent = mem->Read<DWORD>(this->EntityListBase + i * 0x10);									////Itterate through potiental ents
		if (current_ent == NULL || current_ent == 0x0)
			continue;
		Players[i].BaseAddress = current_ent;																		////Get baseaddress of certain ent
		Players[i].Dorm = this->mem->Read<int>(Players[i].BaseAddress + this->offsets->m_dwDormant);				////Get dormancy
		Players[i].Team = this->mem->Read<int>(Players[i].BaseAddress + this->offsets->m_iTeamNum);					////Get team number
		Players[i].Position = this->mem->Read<Vector3>(Players[i].BaseAddress + this->offsets->m_vecOrigin);		////Get position
		this->CurrentAmount++;
	}
}
void EntityManger::organizeByTeam()
{
	for (int i = 0; i < this->offsets->MagicNumber; i++)
	{
		if (Players[i].BaseAddress == 0x0)
			continue;
		if (Players[i].Team == Local.Team)
		{
			PlayersFriendly.push_back(Players[i]);
		}
		else
		{
			PlayersEnemy.push_back(Players[i]);
		}
	}
}
void EntityManger::UpdateEntitiesQuick()
{
	for (int i = 0; i < PlayersFriendly.size(); i++)
	{
		PlayersFriendly[i].Dorm = this->mem->Read<int>(PlayersFriendly[i].BaseAddress + this->offsets->m_dwDormant);				////Get dormancy
		PlayersFriendly[i].Position = this->mem->Read<Vector3>(PlayersFriendly[i].BaseAddress + this->offsets->m_vecOrigin);		////Get position
		PlayersFriendly[i].Health = this->mem->Read<int>(PlayersFriendly[i].BaseAddress + this->offsets->m_iHealth);				////Get health
	}
	for (int i = 0; i < PlayersEnemy.size(); i++)
	{
		PlayersEnemy[i].Dorm = this->mem->Read<int>(PlayersEnemy[i].BaseAddress + this->offsets->m_dwDormant);						////Get dormancy
		PlayersEnemy[i].Position = this->mem->Read<Vector3>(PlayersEnemy[i].BaseAddress + this->offsets->m_vecOrigin);				////Get position
		PlayersEnemy[i].Health = this->mem->Read<int>(PlayersEnemy[i].BaseAddress + this->offsets->m_iHealth);						////Get health
		PlayersEnemy[i].Spotted = this->mem->Read<bool>(PlayersEnemy[i].BaseAddress + this->offsets->m_bSpotted);					////Get visablity
	}
}
void EntityManger::UpdateEntitiesFull()
{
	this->CurrentAmount = 0;
	for (int i = 1; i < this->offsets->MagicNumber; i++)														////Start at 1 to ignore LocalPlayer
	{
		DWORD current_ent = this->mem->Read<DWORD>(this->EntityListBase + i * 0x10);							////Itterate through potiental ents
		if (current_ent == NULL || current_ent == 0x0)
			continue;
		Players[i].BaseAddress = current_ent;																	////Get baseaddress of certain ent
		Players[i].Dorm = this->mem->Read<int>(Players[i].BaseAddress + this->offsets->m_dwDormant);				////Get dormancy
		Players[i].Team = this->mem->Read<int>(Players[i].BaseAddress + this->offsets->m_iTeamNum);					////Get team number
		Players[i].Position = this->mem->Read<Vector3>(Players[i].BaseAddress + this->offsets->m_vecOrigin);		////Get position
		this->CurrentAmount++;
	}
	organizeByTeam();
}
void EntityManger::UpdateLocalFull()
{
	getLocal();
}
void EntityManger::UpdateLocalQuick()
{
	Local.Team = this->mem->Read<int>(Local.BaseAddress + this->offsets->m_iTeamNum);								////Get team number
	Local.Health = this->mem->Read<int>(Local.BaseAddress + this->offsets->m_iHealth);								////Get health
	Local.Dorm = this->mem->Read<int>(Local.BaseAddress + this->offsets->m_dwDormant);								////Get dormancy
	Local.Position = this->mem->Read<Vector3>(Local.BaseAddress + this->offsets->m_vecOrigin);						////Get vec3 position
	Local.AngleX = this->mem->Read<float>(Local.ClientState + this->offsets->dwClientState_ViewAngles + 0x4);		////Get crosshair anglex
	Local.AngleY = this->mem->Read<float>(Local.ClientState + this->offsets->dwClientState_ViewAngles);				////Get crosshair angley
}
EntityPlayers *EntityManger::GetEntityByTeam(bool team, int index)
{
	switch (team)
	{
		case true: 
			if (PlayersFriendly.size() < 1)
			{
				this->mem->Message<char*>("PlayersFiendly is wrong", "Players");
				return NULL;
			}
			return &PlayersFriendly[index];

		case false:
			if (PlayersEnemy.size() < 1)
			{
				this->mem->Message<char*>("PlayerEnemy is wrong", "Players");
				return NULL;
			}
			return &PlayersEnemy[index];
	}
}
LocalPlayer *EntityManger::GetLocalPlayer()
{
	return &Local;
}
bool EntityManger::Initialize(MemoryManager *pMem, OffSets *offsets)
{
	this->mem = pMem;
	this->offsets = offsets;
	this->Players.resize(offsets->MagicNumber);
	getLocal();
	getEntities();
	organizeByTeam();
	return true;
}