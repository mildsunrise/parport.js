/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * parallel-port
 * Copyright (C) 2010 Matheus Neder <matheusneder@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @brief parallel-port /parallel-port/src/ParallelPortWin32.cpp
 * @file ParallelPortWin32.cpp
 * @date 11/09/2010
 * @author Matheus Neder <matheusneder@gmail.com>
 */

#ifdef _WIN32

#include <cstdlib>
#include <string>
#include <sstream>
#include <windows.h>
#include <ParallelPort.h>

typedef short (_stdcall *Win32InpOut32DLLInput)(short portaddr);
typedef void (_stdcall *Win32InpOut32DLLOutput)(short portaddr, short data);

static HINSTANCE win32HandleInstance;
static Win32InpOut32DLLInput win32Input;
static Win32InpOut32DLLOutput win32Ouput;

using namespace std;

static const short BASE_PORT_ADDR = 0x378;
static size_t portsOpened = 0;

ParallelPort::ParallelPort() :
	_isOpen(false)
{

}

void ParallelPort::open(short port) throw (std::runtime_error)
{
	dataPortAddr = BASE_PORT_ADDR + (port * 3);
	statusPortAddr = dataPortAddr + 1;
	controlPortAddr = dataPortAddr + 2;

	/* Load the library */
	win32HandleInstance = LoadLibrary("inpout32.dll");

	if (win32HandleInstance == NULL)
	{
		throw runtime_error("Unable to LoadLibrary inpout32.dll");
	}

	/* get the address of the Inp32/Out32 functions */
	win32Input = (Win32InpOut32DLLInput) GetProcAddress(win32HandleInstance,
			"Inp32");

	if (win32Input == NULL)
	{
		throw runtime_error("Unable to GetProcAddress for Inp32");
	}

	win32Ouput = (Win32InpOut32DLLOutput) GetProcAddress(win32HandleInstance,
			"Out32");

	if (win32Ouput == NULL)
	{
		throw runtime_error("Unable to GetProcAddress for Out32");
	}
	_isOpen = true;
	portsOpened++;
}

bool ParallelPort::isOpen()
{
	return _isOpen;
}

void ParallelPort::close()
{
	if (_isOpen)
	{
		_isOpen = false;
		portsOpened--;
		if (portsOpened == 0)
		{
			FreeLibrary(win32HandleInstance);
		}
	}
}
char ParallelPort::readData() throw (std::runtime_error)
{
	return (char) (win32Input)(dataPortAddr);
}
char ParallelPort::readControl() throw (std::runtime_error)
{
	return (char) (win32Input)(controlPortAddr);
}
char ParallelPort::readStatus() throw (std::runtime_error)
{
	return (char) win32Input(statusPortAddr);
}
void ParallelPort::writeData(char value) throw (std::runtime_error)
{
	(win32Ouput)(dataPortAddr, (short) value);
}
void ParallelPort::writeControl(char value) throw (std::runtime_error)
{
	(win32Ouput)(controlPortAddr, (short) value);
}

#endif
