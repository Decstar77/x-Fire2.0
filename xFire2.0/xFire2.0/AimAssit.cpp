#include "AimAssit.h"
#include <string>


AimAssit::AimAssit()
{
	this->crouched = false;
	this->recoil = false;
	this->My_Angle_X = 0;
	this->My_Angle_Y = 0;
	this->fov = 0.5;			////1
	this->lerpValue = 0.90;
	this->time = 0;				
	this->totalDistance = 0;
}

AimAssit::~AimAssit()
{

}

Vector3 AimAssit::TargetRelativeToPlayer(Vector3 m_Target, Vector3 m_local, bool iscrouched)
{
	Vector3 tmp;
	tmp.x = m_Target.x - m_local.x;
	tmp.y = m_Target.y - m_local.y;
	tmp.z = m_Target.z - m_local.z + iscrouched * 20;
	return tmp;
}

void AimAssit::GetValues()
{
	this->ent->UpdateEntitiesQuick();
	this->ent->UpdateLocalQuick();
	isCrouched();
	hasRecoil();
	for (int i = 0; i < Enemies.size(); i++)
	{
		this->Enemies[i] = this->ent->GetEntityByTeam(false, i);
		this->Enemies[i]->RelativePosition = TargetRelativeToPlayer(this->Enemies[i]->Position, this->m_local->Position, this->crouched);
		this->Enemies[i]->BaseAngleX = atan2(this->Enemies[i]->RelativePosition.y, this->Enemies[i]->RelativePosition.x) * 180 / PI;
		float mag = sqrt(this->Enemies[i]->RelativePosition.x * this->Enemies[i]->RelativePosition.x + this->Enemies[i]->RelativePosition.y * this->Enemies[i]->RelativePosition.y + this->Enemies[i]->RelativePosition.z * this->Enemies[i]->RelativePosition.z);
		this->Enemies[i]->BaseAngleY = -atan2(this->Enemies[i]->RelativePosition.z, mag) * 180 / PI;
		this->Enemies[i]->distanceAngleX = abs(this->m_local->AngleX - this->Enemies[i]->BaseAngleX);
		this->Enemies[i]->distanceAngleY = abs(this->m_local->AngleY - this->Enemies[i]->BaseAngleY);
		this->tempAnglesX[i] = this->Enemies[i]->distanceAngleX;
		this->tempAnglesY[i] = this->Enemies[i]->distanceAngleY;
			
	}
	std::sort(this->tempAnglesX, this->tempAnglesX + 64);
	std::sort(this->tempAnglesY, this->tempAnglesY + 64);
	
	/////////////////////////////////////Finding View Angles with Cliet.dll/////////////////////////
	//DWORD base = this->offsets->ClientDllBaseAdress + this->offsets->m_dwLocalPlayer;/////////////
	//DWORD newBase = this->mem->Read<DWORD>(base) + 0x128;/////////////////////////////////////////
	//float amY = this->mem->Read<float>(newBase);//////////////////////////////////////////////////



}

float AimAssit::Lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
void AimAssit::isCrouched()
{
	if (GetAsyncKeyState(VK_CONTROL))
	{
		this->crouched = true;
	}
	if (!GetAsyncKeyState(VK_CONTROL))
	{
		this->crouched = false;
	}
}
void AimAssit::hasRecoil()
{
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		this->time++;
	}
	else
	{
		this->recoil = false;
	}
	if (this->time >= 10 && this->recoil == false)
	{
		this->time = 0;
		this->recoil = true;
	}
}
void AimAssit::TestLock()
{
	GetValues();
	for (int i = 0; i < Enemies.size(); i++)
	{
		//MessageBoxA(NULL, LPCSTR(std::to_string(tempAnglesX[i]).c_str()), LPCSTR(std::to_string(i).c_str()), MB_OK);	
		if (this->tempAnglesX[0] == Enemies[i]->distanceAngleX)
		{
			
			this->m_local->AngleX = Lerp(this->Enemies[i]->BaseAngleX, this->m_local->AngleX, 0.5);
			this->m_local->AngleY = Lerp(this->Enemies[i]->BaseAngleY, this->m_local->AngleY, 0.5);
			DWORD temp = this->mem->Read<DWORD>(this->offsets->EngineDllBaseAdress + (DWORD)(0x57F84C));
			this->mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles, this->Enemies[i]->BaseAngleY);
			this->mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles + 0x4, this->Enemies[i]->BaseAngleX);
			break;
		}
	}
}

float AimAssit::normalizeAngle(float angle, float upper, float lower) {
	while (lower > angle || angle > upper) {
		if (angle < lower) angle += 360.0f;
		if (angle > upper) angle -= 360.0f;
	}
	return angle;
}



void AimAssit::DoAimAssit()
{
	GetValues();
	for (int i = 0; i < Enemies.size(); i++)
	{
		totalDistance = sqrt(this->Enemies[i]->distanceAngleX * this->Enemies[i]->distanceAngleX + this->Enemies[i]->distanceAngleY *this->Enemies[i]->distanceAngleY);
		
		if (totalDistance <= 1)
		{
			//this->mem->Message<char*>("1", "Test");
			if (this->Enemies[i]->Dorm == true)
				continue;
			//this->mem->Message<char*>("1", "3");
			if (this->Enemies[i]->Health <= 0)
				continue;
			//this->mem->Message<char*>("1", "4");
			if (this->Enemies[i]->Spotted == false)
				continue;
			//this->mem->Message<char*>("1", "5");
			if (this->recoil)
			{
				break;
			}
			//this->mem->Message<char*>("1", "6");
			////Lerp the angles
			this->My_Angle_X = Lerp(this->Enemies[i]->BaseAngleX, this->m_local->AngleX, this->lerpValue);
			this->My_Angle_Y = Lerp(this->Enemies[i]->BaseAngleY, this->m_local->AngleY, this->lerpValue);
			////Clamp the angle 
			this->My_Angle_X = normalizeAngle(My_Angle_X, 180.0f, -180.0f);
			this->My_Angle_Y = normalizeAngle(My_Angle_Y, 180.0f, -180.0f);
			////Write to memory
			DWORD temp = this->mem->Read<DWORD>(this->offsets->EngineDllBaseAdress + (DWORD)(this->offsets->dwClientState));  //0x0057D894
			mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles, My_Angle_Y);
			mem->Write<float>(temp + this->offsets->dwClientState_ViewAngles + 0x4, My_Angle_X);
			break;
		}
	}
}


bool AimAssit::Initialize(MemoryManager* mem, EntityManger* ent, OffSets* offset)
{
	this->mem = mem;
	this->ent = ent;
	this->offsets = offset;
	Enemies.resize(ent->GetAmountOf(false));					////Could cause probs
	this->m_local = this->ent->GetLocalPlayer();
	std::fill_n(tempAnglesX, 64, 400);
	std::fill_n(tempAnglesY, 64, 400);
	return true;
}


