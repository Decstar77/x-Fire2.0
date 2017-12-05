#include <iostream>

#include "MemoryManager.h"
#include "EntityManager.h"
#include "Glow.h"
#include "AimLock.h"

int main()
{
	OffSets Offsets;
	MemoryManager Mem;
	Mem.Initialize(&Offsets);
	EntityManger Ent;
	Ent.Initialize(&Mem, &Offsets);
	Glow Glow_;
	Glow_.Initialize(&Mem, &Ent, &Offsets);
	AimLock Aim;
	Aim.Initialize(&Mem, &Ent, &Offsets);
	while (true)
	{
		if (GetAsyncKeyState(VK_F11))
		{
			Ent.Initialize(&Mem, &Offsets);
			Glow_.Initialize(&Mem, &Ent, &Offsets);
		}
		if (GetAsyncKeyState(0x45))		////E/////
		{
			Glow_.DoGlow();
		}
		if (GetAsyncKeyState(VK_XBUTTON1))		////H/////
		{
			Aim.DoAimLock();
		}
	}
	Mem.Message("Should not be here", "Check zeta");

}



