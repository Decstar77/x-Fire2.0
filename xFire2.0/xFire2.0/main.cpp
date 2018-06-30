#include <iostream>
#include "MemoryManager.h"
#include "EntityManager.h"
#include "Glow.h"
#include "AimLock.h"
#include "AimAssit.h"

int main()
{
	bool wait = true;
	bool turnOffGlow = true;
	OffSets Offsets;
	MemoryManager Mem;
	Mem.Initialize(&Offsets);
	EntityManger Ent;
	Ent.Initialize(&Mem, &Offsets);
	Glow Glow_;
	Glow_.Initialize(&Mem, &Ent, &Offsets);
	AimLock Aim;
	Aim.Initialize(&Mem, &Ent, &Offsets);
	AimAssit AimA;
	AimA.Initialize(&Mem, &Ent, &Offsets);
	while (true)
	{
		Sleep(2);
		while (wait)
		{
			if (GetAsyncKeyState(VK_F6))
			{
				wait = false;
				MessageBeep(MB_ICONWARNING);
			}
			Sleep(100);
		}//Ent.UpdateEntitiesFull();
		//Ent.UpdateLocalFull();
		//Mem.Message<char*>("Test", "Test1");
		if (GetAsyncKeyState(VK_F11))
		{
			Mem.SoftInitialize(&Offsets);
			Ent.Initialize(&Mem, &Offsets);
			Glow_.Initialize(&Mem, &Ent, &Offsets);
			Aim.Initialize(&Mem, &Ent, &Offsets);
			AimA.Initialize(&Mem, &Ent, &Offsets);
		}
		if (GetAsyncKeyState(VK_F1))
		{
			Mem.FreeLib();
		}
		/*if (GetAsyncKeyState(0x45))		////E/////
		{
			//Ent.Initialize(&Mem, &Offsets);
			Glow_.Initialize(&Mem, &Ent, &Offsets);
			Glow_.DoGlow();
		}*/
		if (GetAsyncKeyState(VK_F7))
		{
			turnOffGlow = !turnOffGlow;
			MessageBeep(MB_OK);
		}

		if (GetAsyncKeyState(VK_XBUTTON1) && turnOffGlow)
		{
			Glow_.Initialize(&Mem, &Ent, &Offsets);
			Glow_.DoGlow();
		}
		/*if (GetAsyncKeyState(0x52)) /////R/////
		{
			Aim.Initialize(&Mem, &Ent, &Offsets);
			Aim.DoAimLock();
			
		}*/
	
		AimA.Initialize(&Mem, &Ent, &Offsets);
		AimA.DoAimAssit();

	}
	Mem.Message("Should not be here", "Check zeta");
}



