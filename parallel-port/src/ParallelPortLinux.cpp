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
 * @brief parallel-port /parallel-port/src/ParallelPortLinux.cpp
 * @file ParallelPortLinux.cpp
 * @date 11/09/2010
 * @author Matheus Neder <matheusneder@gmail.com>
 */

#ifdef _LINUX

extern "C"
{
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>
#include <fcntl.h>
#include <unistd.h>
}
#include <ParallelPort.h>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

ParallelPort::ParallelPort() :
	_isOpen(false)
{
}

void ParallelPort::open(short port) throw (std::runtime_error)
{
	stringstream aux;
	aux << "/dev/parport" << port;
	parportPath = aux.str();
	fd = ::open(parportPath.c_str(), O_RDWR);
	if (fd == -1)
	{
		throw runtime_error(string("Unable to open ") + parportPath);
	}
	if (ioctl(fd, PPCLAIM) != 0)
	{
		::close(fd);
		throw runtime_error(string("Could not claim the ") + parportPath
				+ " to start using it");
	}
	int mode = IEEE1284_MODE_COMPAT;
	if (ioctl(fd, PPNEGOT, &mode) != 0)
	{
		::close(fd);
		throw runtime_error(string(
				"Unable to negotiate IEEE1284_MODE_COMPAT mode on ")
				+ parportPath);
	}
	_isOpen = true;
}

bool ParallelPort::isOpen()
{
	return _isOpen;
}

void ParallelPort::close()
{
	_isOpen = false;
	ioctl(fd, PPRELEASE);
	::close(fd);
}

char ParallelPort::readData() throw (std::runtime_error)
{
	char ret;
	if (ioctl(fd, PPRDATA, &ret) != 0)
	{
		throw runtime_error(string("Unable to read DATA on ") + parportPath);
	}
	return ret;
}

char ParallelPort::readControl() throw (std::runtime_error)
{
	char ret;
	if (ioctl(fd, PPRCONTROL, &ret) != 0)
	{
		throw runtime_error(string("Unable to read CONTROL on ") + parportPath);
	}
	return ret;
}

char ParallelPort::readStatus() throw (std::runtime_error)
{
	char ret;
	if (ioctl(fd, PPRSTATUS, &ret) != 0)
	{
		throw runtime_error(string("Unable to read STATUS on ") + parportPath);
	}
	return ret;
}

void ParallelPort::writeData(char value) throw (std::runtime_error)
{
	if (ioctl(fd, PPWDATA, &value) != 0)
	{
		throw runtime_error(string("Unable to write DATA on ") + parportPath);
	}
}

void ParallelPort::writeControl(char value) throw (std::runtime_error)
{
	if (ioctl(fd, PPWCONTROL, &value) != 0)
	{
		throw runtime_error(string("Unable to write CONTROL on ") + parportPath);
	}
}

#endif
