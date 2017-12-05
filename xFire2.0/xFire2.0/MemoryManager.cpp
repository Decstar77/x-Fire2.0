#include "MemoryManager.h"


MemoryManager::MemoryManager()
{
	this->pHandle = NULL;
	this->mHandle_engine = NULL;
	this->mHandle_client = NULL;
	this->procs.dwSize = sizeof(PROCESSENTRY32);
	this->mods.dwSize = sizeof(MODULEENTRY32);
	this->mod_Client.dwSize = sizeof(MODULEENTRY32);
	this->mod_Engine.dwSize = sizeof(MODULEENTRY32);
}
MemoryManager::~MemoryManager()
{
	CloseHandle(pHandle);
	CloseHandle(mHandle_client);
	CloseHandle(mHandle_engine);
}
bool MemoryManager::SetOffSetsEqual(OffSets *ToBeSet, OffSets *ToBeUsedToSet)
{
	//ToBeSet->ClientDllBaseAdress = OffSet
	return true;
}
void MemoryManager::GetProcessStuffies(const char* name)
{
	try
	{
		this->pHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (this->pHandle == NULL) { throw 1; }
		if (!Process32First(this->pHandle, &procs)) { throw 2; }
		do
		{
			if (strcmp(procs.szExeFile, name) == 0)
			{		
				this->procId = procs.th32ProcessID;
				this->pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, this->procId);
				return;
			}
		} while (Process32Next(this->pHandle, &procs));
		this->pHandle = NULL;
		throw 3;
	}
	catch (int error)
	{
		switch (error)
		{
		case 1: MessageBox(NULL, "GetProcessId, m_handle is NULL", "Error", MB_OK); return;
		case 2: MessageBox(NULL, "GetProcessId, proc32First", "Error", MB_OK); return;
		case 3: MessageBox(NULL, "GetProcessId, cannot find process", "Error", MB_OK); return;
		default: throw 'E';
		}
	}
	catch (...)
	{
		MessageBox(NULL, "GetProcessId, unknown error", "Error", MB_OK);
	}
}
void MemoryManager::GetProcessBaseAddress(const char* name)
{
	/*
	try
	{
		this->m_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
		if (this->m_handle == NULL) { throw 1; }
		if (!Module32First(m_handle, &modEntry)) { throw 2; }
		do
		{
			if (strcmp(modEntry.szModule, name) == 0)
			{
				this->m_handle = NULL;
				return (DWORD)modEntry.modBaseAddr;
			}
		} while (Module32Next(m_handle, &modEntry));
		this->m_handle = NULL;
		throw 3;
	}
	catch (int error)
	{
		switch (error)
		{
		case 0: MessageBox(NULL, "GetProcessAddress, procId is NULL", "Error", MB_OK); return NULL;
		case 1: MessageBox(NULL, "GetProcessAddress, m_handle is NULL", "Error", MB_OK); return NULL;
		case 2: MessageBox(NULL, "GetProcessAddress, Cannot get first entry", "Error", MB_OK); return NULL;
		case 3: MessageBox(NULL, "GetProcessAddress, Cannot find module", "Error", MB_OK); return NULL;
		default: throw 'E';
		}
	}
	catch (...)
	{
		MessageBox(NULL, "GetProcessAddress, Unknown error", "Error", MB_OK);
	}
	*/
}
void MemoryManager::GetModule(const char* name)
{
	HANDLE m_handle;
	try
	{
		m_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, NULL);
		if (m_handle == NULL) { throw 1; }
		if (!Module32First(m_handle, &mods)) { throw 2; }
		do
		{
			if (strcmp(mods.szModule, name) == 0)
			{
				if (strcmp(name, "client.dll") == 0)
				{
					this->mHandle_client = m_handle;
					this->mod_Client = mods;
					return;
				}
				else if (strcmp(name, "engine.dll") == 0)
				{
					this->mHandle_engine = m_handle;
					this->mod_Engine = mods;
					return;
				}
				else
				{
					throw 4;
				}
			}
		} while (Module32Next(m_handle, &mods));
		m_handle = NULL;
		throw 3;
	}
	catch (int error)
	{
		switch (error)
		{
		case 0:  MessageBox(NULL, "GetModule, procId is NULL", "Error", MB_OK); return;
		case 1:  MessageBox(NULL, "GetModule, m_handle is NULL", "Error", MB_OK); return;
		case 2:  MessageBox(NULL, "GetModule, cannot get first entry ", "Error", MB_OK); return;
		case 3:  MessageBox(NULL, "GetModule, cannot find module", "Error", MB_OK); return;
		case 4:  MessageBox(NULL, "GetModule, names ?", "Error", MB_OK); return;
		default: throw 'E';
		}
	}
	catch (...)
	{
		MessageBox(NULL, "GetModule, Unknown error", "Error", MB_OK);
	}
}
bool MemoryManager::Initialize(OffSets *offset)
{
	GetProcessStuffies("csgo.exe");
	GetModule("client.dll");
	GetModule("engine.dll");
	offset->ClientDllBaseAdress = (DWORD)this->mod_Client.modBaseAddr;
	offset->EngineDllBaseAdress = (DWORD)this->mod_Engine.modBaseAddr;
	return true;
}