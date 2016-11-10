/*
 Enc28J60NetworkClass.h
 UIPEthernet network driver for Microchip ENC28J60 Ethernet Interface.

 Copyright (c) 2013 Norbert Truchsess <norbert.truchsess@t-online.de>
 All rights reserved.

 inspired by enc28j60.c file from the AVRlib library by Pascal Stang.
 For AVRlib See http://www.procyonengineering.com/

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Enc28J60Network_H_
#define Enc28J60Network_H_

#include "mempool.h"

#if !defined(ENC28J60_CONTROL_CS)
   #if defined(__AVR__)
      #define ENC28J60_CONTROL_CS     SS // this is pin 10 on Uno and pin 53 on Mega256
   #endif
   #if defined(ARDUINO_ARCH_SAM)
      #define ENC28J60_CONTROL_CS     SS // this is pin 53 on DUE
   #endif
   #if defined(STM32_MCU_SERIES) || defined(__STM32F1__) || defined(__STM32F3__) || defined(__STM32F4__)
      #ifdef ARDUINO_STM32F4_NETDUINO2PLUS
         #define ENC28J60_CONTROL_CS     PC8
      #else
	 #include <SPI.h>
         #define ENC28J60_CONTROL_CS     SPI.nssPin()
         //#define ENC28J60_CONTROL_CS     PA4
      #endif
   #endif
   //https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/using-the-arduino-addon
   #if defined(ESP8266)
//      #define SPI_MISO 12
//      #define SPI_MOSI 13
//      #define SPI_SCK 13
      #define ENC28J60_CONTROL_CS     15 // this is pin 15 on ESP8266
   #endif
#endif

#if defined(ARDUINO_ARCH_AVR)
// AVR-specific code
   #define SPI_MOSI        MOSI
   #define SPI_MISO        MISO
   #define SPI_SCK         SCK
   #define SPI_SS          SS
   #define ENC28J60_USE_SPILIB 0
#elif defined(ARDUINO_ARCH_SAM) || defined(__STM32F1__) || defined(__STM32F3__) || defined(__STM32F4__) || defined(ESP8266)
   #define ENC28J60_USE_SPILIB 1
#endif

#define UIP_RECEIVEBUFFERHANDLE 0xff

//ENC28J60DEBUGDEFINE for search
//#define ENC28J60DEBUG

/*
 * Empfangen von ip-header, arp etc...
 * wenn tcp/udp -> tcp/udp-callback -> assign new packet to connection
 */

class Enc28J60Network : public MemoryPool
{

private:
  static uint16_t nextPacketPtr;
  static uint8_t bank;

  static struct memblock receivePkt;

  static bool broadcast_enabled; //!< True if broadcasts enabled (used to allow temporary disable of broadcast for DHCP or other internal functions)

  static uint8_t readOp(uint8_t op, uint8_t address);
  static void writeOp(uint8_t op, uint8_t address, uint8_t data);
  static uint16_t setReadPtr(memhandle handle, memaddress position, uint16_t len);
  static void setERXRDPT();
  static void readBuffer(uint16_t len, uint8_t* data);
  static void writeBuffer(uint16_t len, uint8_t* data);
  static uint8_t readByte(uint16_t addr);
  static void writeByte(uint16_t addr, uint8_t data);
  static void setBank(uint8_t address);
  static uint8_t readReg(uint8_t address);
  static void writeReg(uint8_t address, uint8_t data);
  static void writeRegPair(uint8_t address, uint16_t data);
  static void phyWrite(uint8_t address, uint16_t data);
  static uint16_t phyRead(uint8_t address);
  static void clkout(uint8_t clk);

  static void enableBroadcast (bool temporary);
  static void disableBroadcast (bool temporary);
  static void enableMulticast ();
  static void disableMulticast ();

  static uint8_t readRegByte (uint8_t address);
  static void writeRegByte (uint8_t address, uint8_t data);

  friend void enc28J60_mempool_block_move_callback(memaddress,memaddress,memaddress);

public:

  uint8_t getrev(void);
  void powerOn();
  void powerOff();
  bool linkStatus();

  static void init(uint8_t* macaddr);
  static memhandle receivePacket();
  static void freePacket();
  static memaddress blockSize(memhandle handle);
  static void sendPacket(memhandle handle);
  static uint16_t readPacket(memhandle handle, memaddress position, uint8_t* buffer, uint16_t len);
  static uint16_t writePacket(memhandle handle, memaddress position, uint8_t* buffer, uint16_t len);
  static void copyPacket(memhandle dest, memaddress dest_pos, memhandle src, memaddress src_pos, uint16_t len);
  static uint16_t chksum(uint16_t sum, memhandle handle, memaddress pos, uint16_t len);
};

extern Enc28J60Network Enc28J60;
#endif /* Enc28J60NetworkClass_H_ */
