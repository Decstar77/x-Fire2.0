#include "Glow.h"

Glow::Glow()
{

}

Glow::~Glow()
{
	delete mem;
	delete ent;
	delete offsets;
}

void Glow::fillGlowBases()
{
	for (int i = 0; i < this->AmountOfGlowObjects; i++)
	{
		DWORD tempAddress = this->mem->Read<DWORD>(this->pointerToGlow + i * 0x38);
		if (tempAddress == NULL)
			continue;
		for (int ii = 0; ii < GlowBasesFriendly.size(); ii++)
		{
			if (this->ent->GetEntityByTeam(true, ii)->BaseAddress == tempAddress)
			{
				GlowBasesFriendly[ii] = this->ent->GetEntityByTeam(true, ii);
				GlowBasesFriendly[ii]->GlowBaseAddress = this->pointerToGlow + i * 0x38;
			}
		}
		for (int ii = 0; ii < GlowBasesEnemy.size(); ii++)
		{
			if (this->ent->GetEntityByTeam(false, ii)->BaseAddress == tempAddress)
			{
				GlowBasesEnemy[ii] = this->ent->GetEntityByTeam(false, ii);
				GlowBasesEnemy[ii]->GlowBaseAddress = this->pointerToGlow + i * 0x38;
			}
		}
	}
}

void Glow::DoGlow()
{
	this->ent->UpdateEntitiesQuick();
	fillGlowBases();
	for (int i = 0; i < GlowBasesFriendly.size(); i++)								
	{
		if (this->GlowBasesFriendly[i]->Dorm)
			continue;
		this->mem->Write<float>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0x4, 0);						
		this->mem->Write<float>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0x8, (float)140 / 225);
		this->mem->Write<float>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0xC, 0);
		this->mem->Write<float>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0x10, 1.0f);
		this->mem->Write<BOOL>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0x24, true);
		this->mem->Write<BOOL>(this->GlowBasesFriendly[i]->GlowBaseAddress + 0x25, false);
	}
	for (int i = 0; i < GlowBasesEnemy.size(); i++)
	{
		if (this->GlowBasesEnemy[i]->Dorm)
			continue;
		this->mem->Write<float>(GlowBasesEnemy[i]->GlowBaseAddress + 0x4, (float)140 / 225);
		this->mem->Write<float>(GlowBasesEnemy[i]->GlowBaseAddress + 0x8, 0);
		this->mem->Write<float>(GlowBasesEnemy[i]->GlowBaseAddress + 0xC, 0);
		this->mem->Write<float>(GlowBasesEnemy[i]->GlowBaseAddress + 0x10, 1.0f);
		this->mem->Write<BOOL>(GlowBasesEnemy[i]->GlowBaseAddress + 0x24, true);
		this->mem->Write<BOOL>(GlowBasesEnemy[i]->GlowBaseAddress + 0x25, false);
	}
}


bool Glow::Initialize(MemoryManager* mem, EntityManger* ent, OffSets* offsets)
{
	this->mem = mem;
	this->ent = ent;
	this->offsets = offsets;
	this->AmountOfGlowObjects = mem->Read<int>(this->offsets->ClientDllBaseAdress + this->offsets->m_dwGlow + 0x4);
	this->pointerToGlow = mem->Read<DWORD>(this->offsets->ClientDllBaseAdress + this->offsets->m_dwGlow);
	this->GlowBasesFriendly.resize(this->ent->GetAmountOf(true));
	this->GlowBasesEnemy.resize(this->ent->GetAmountOf(false));
	if ((AmountOfGlowObjects <= 0) || (pointerToGlow == NULL))
		return false;
	fillGlowBases();
	return true;
}

