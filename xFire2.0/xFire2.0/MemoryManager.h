#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <tchar.h>
struct OffSets
{
	const int MagicNumber = 64;
	static const DWORD m_bSpotted = 0x939;
	DWORD m_iTeamNum = 0xF0;
	DWORD m_iCrossHairID = 0xB2A4;
	DWORD m_dwDormant = 0xE9;
	DWORD m_iHealth = 0xFC;
	static const DWORD m_dwLocalPlayer = 0xAAAAB4;
	static const DWORD m_dwEntityList = 0x4A8574C;
	static const DWORD m_dwGlow = 0x4FB2540;
	DWORD m_dwForceAttack = 0x2ECF4D8;
	static const DWORD dwClientState = 0x57F84C; //Why you no work!!! CHANGE IT ANYWAYS, LOCAL PLAYER USES THIS OFFSET. Check aimLock.cpp for offset 
	DWORD dwClientState_ViewAngles = 0x4D10; //0x4D10 // Notes, you can find view angles via client.dll look to AimAssist for more details
	DWORD m_vecOrigin = 0x134;
	DWORD m_vecViewOffset = 0x104;
	DWORD ClientDllBaseAdress;
	DWORD EngineDllBaseAdress;
};
											//All offsets
struct Vector3
{
	float x;
	float y;
	float z;
};
											//Vector struct, x, y, z

class MemoryManager
{
	private:
		HANDLE pHandle;						// Process Handle to process
		HANDLE mHandle_engine;				// Module Handle to engine.dll
		HANDLE mHandle_client;				// Module Handle to client.dll

		DWORD procId;						// Process Id
		DWORD procAddress;					// Process Base Address

		MODULEENTRY32 mod_Engine;			//Module to engine.dll
		MODULEENTRY32 mod_Client;			//Module to client.dll

		MODULEENTRY32 mods;					// Tempory module entry, to find engine and client dlls
		PROCESSENTRY32 procs;				// Tempory proc entry, to find process handles and pid

		void GetProcessStuffies(const char*);		//Get procId, and handle to a process
		void GetProcessBaseAddress(const char*);	//Get the procId's base address
		void GetModule(const char*);				//Get the modules, 




	public:
		MemoryManager();							//__Init__ of class //// Doesn't do much, all of the real magic happens in function Initialize();
		~MemoryManager();							//Des of class //// Closes handles ect
		HANDLE *GetProcHandle();
		bool Initialize(OffSets *);					//Gets all base address 
		bool SoftInitialize(OffSets *);
		bool CompareOffSets(OffSets *, OffSets *);	//Compare offSets
		bool SetOffSetsEqual(OffSets *, OffSets *);	//Set offsets to be the same
		bool FreeLib();

		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////Templated Functions//////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		template<typename r> //Templated ReadProcess Memory
		r Read(DWORD addressToRead)
		{
			r StoreVar;
			ReadProcessMemory(this->pHandle, (LPVOID)addressToRead, &StoreVar, sizeof(StoreVar), NULL);
			return StoreVar;
		}
		template<class w> //Templated WriteProcess Memory
		BOOL Write(DWORD addressToWriteTo, w ItemToBeWritten)
		{
			if (!WriteProcessMemory(this->pHandle, (LPVOID)addressToWriteTo, &ItemToBeWritten, sizeof(ItemToBeWritten), NULL))
				return false;
			return true;
		}
		template<typename m> //Template MessageBox
		void Message(m out, char *Title)
		{
			TCHAR buff[1000];
			sprintf_s(buff, _T("%d"), out);
			MessageBox(NULL, buff, Title, MB_OK);
		}
		/////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////Other Functions//////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////
		std::string float_to_str(float f)
		{
			std::stringstream stream;
			stream << f;
			return stream.str();
		}

		void MessageF(float out, char* Title)
		{
			std::string outs = float_to_str(out);
			MessageBoxA(NULL, outs.c_str(), Title, MB_OK);
		}

		void MessageI(int out, char *Title)
		{
			int o = out;
			MessageBoxA(NULL, LPCSTR(std::to_string(o).c_str()), Title, MB_OK);
		}

};












