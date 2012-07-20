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
 * @brief parallel-port /parallel-port/include/ParallelPort.h
 * @file ParallelPort.h
 * @date 11/09/2010
 * @author Matheus Neder <matheusneder@gmail.com>
 */
/**
 * @mainpage
 * Parallel Port is a easy way to get raw access to computer parallel port
 * in the same way on both Windows and Linux operation system.
 * @date 11/09/2010
 * @author Matheus Neder <matheusneder@gmail.com>
 */
#ifndef PARALLELPORT_H_
#define PARALLELPORT_H_

#include <stdexcept>
#include <string>

/**
 * @brief ParallelPort class
 * @date 11/09/2010
 * @author Matheus Neder <matheusneder@gmail.com>
 */
class ParallelPort
{
#if defined(__linux) //linux specific attributes
	int fd;
	std::string parportPath;
#elif defined(_WIN32) //win32 specific attributes
	short dataPortAddr, controlPortAddr, statusPortAddr;
#endif
	bool _isOpen;
public:
	ParallelPort();
	/**
	 * @brief Open port
	 * @param port Port number, usually port number 0 opens lpt1
	 * 	(/dev/parport0 on linux with 0x378 base address)
	 */
	void open(short port = 0) throw (std::runtime_error);

	/**
	 * @brief Verify if port for this instance is open
	 */
	bool isOpen();

	/**
	 * @brief Close port
	 */
	void close();

	/**
	 * @brief Read a byte from data pins (usually 0x378)
	 */
	char readData() throw (std::runtime_error);

	/**
	 * @brief Read a byte from control pins (usually 0x37a)
	 *
	 * Control pins is unidirectional (you can't use as input) and
	 * this method just return the data that you put to device with
	 * writeControl method or the data that was there before you perform
	 * a writeControl
	 */
	char readControl() throw (std::runtime_error);

	/**
	 * @brief Read a byte from status pins (usually 0x379)
	 */
	char readStatus() throw (std::runtime_error);

	/**
	 * @brief Write data to data pins (usually 0x378)
	 */
	void writeData(char value) throw (std::runtime_error);

	/**
	 * @brief Write data to status pins (usually 0x379)
	 */
	void writeControl(char value) throw (std::runtime_error);
};

#endif /* PARALLELPORT_H_ */
