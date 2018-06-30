#include "AimLock.h"

AimLock::AimLock()
{

}

AimLock::~AimLock()
{
	delete this->tempAngles;
	delete this->ent;
	delete this->mem;
	delete this->offsets;
}

Vector3 AimLock::TargetRelativeToPlayer(Vector3 m_Target, Vector3 m_local)
{
	Vector3 tmp;
	tmp.x = m_Target.x - m_local.x;
	tmp.y = m_Target.y - m_local.y;
	tmp.z = m_Target.z - m_local.z;
	return tmp;
}

void AimLock::GetValues()
{
	this->ent->UpdateEntitiesQuick();
	this->ent->UpdateLocalQuick();
	for (int i = 0; i < Enemies.size(); i++)
	{
		this->Enemies[i] = this->ent->GetEntityByTeam(false, i);
		this->Enemies[i]->RelativePosition = TargetRelativeToPlayer(this->Enemies[i]->Position, this->m_local->Position);
		//targets[i].BaseAngleX = atan2(targets[i].position.y, targets[i].position.x) * 180 / PI;
		this->Enemies[i]->BaseAngleX = atan2(this->Enemies[i]->RelativePosition.y, this->Enemies[i]->RelativePosition.x) * 180 / PI;
		float mag = sqrt(this->Enemies[i]->RelativePosition.x * this->Enemies[i]->RelativePosition.x + this->Enemies[i]->RelativePosition.y * this->Enemies[i]->RelativePosition.y + this->Enemies[i]->RelativePosition.z * this->Enemies[i]->RelativePosition.z);
		this->Enemies[i]->BaseAngleY = -atan2(this->Enemies[i]->RelativePosition.z, mag) * 180 / PI;
		this->Enemies[i]->distanceAngleX = abs(this->m_local->AngleX - this->Enemies[i]->BaseAngleX);
		this->tempAngles[i] = this->Enemies[i]->distanceAngleX;	
	}
	std::sort(this->tempAngles, this->tempAngles + this->ent->GetAmountOf(false));
}

void AimLock::DoAimLock()
{
	GetValues();
	for (int i = 0; i < Enemies.size(); i++)
	{
		if (this->tempAngles[0] == Enemies[i]->distanceAngleX)
		{
			if (this->Enemies[i]->Dorm == true)
				//break;
			if (this->Enemies[i]->Team == this->m_local->Team)
				break;
			if (this->Enemies[i]->Health <= 0)
				break;
			//if (this->Enemies[i]->Spotted == false)
			//	break;
			
			//this->m_local->AngleX = this->ent->Lerp(this->Enemies[i]->BaseAngleX, this->m_local->AngleX, 0.5);
			//this->m_local->AngleY = this->ent->Lerp(this->Enemies[i]->BaseAngleY, this->m_local->AngleY, 0.5);
			DWORD temp = this->mem->Read<DWORD>(this->offsets->EngineDllBaseAdress + (DWORD)(0x57F84C));
			this->mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles, this->Enemies[i]->BaseAngleY);
			this->mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles + 0x4, this->Enemies[i]->BaseAngleX);
			break;
		}
	}
}

bool AimLock::Initialize(MemoryManager *mem, EntityManger *ent, OffSets *offsets)
{
	this->mem = mem;
	this->ent = ent;
	this->offsets = offsets;
	this->Enemies.resize(ent->GetAmountOf(false));
	this->m_local = this->ent->GetLocalPlayer();
	this->tempAngles = new float[ent->GetAmountOf(false)];
	return true;
}