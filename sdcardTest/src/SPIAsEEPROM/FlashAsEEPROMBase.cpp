// /* Speeduino SPIAsEEPROM Library v.2.0.0
//  * Copyright (C) 2020 by Tjeerd Hoogendijk
//  * Created by Tjeerd Hoogendijk - 21/09/2019
//  * Updated by Tjeerd Hoogendijk - 19/04/2020
//  *
//  * This file is part of the Speeduino project. This library is for
//  * Winbond SPI flash memory modules. In its current form it enables reading
//  * and writing individual bytes as if it where an AVR EEPROM. When the begin() 
//  * fuction is called for the first time it will "format" the flash chip. 
//  * !!!!THIS DISTROYS ANY EXISTING DATA ON THE SPI FLASH!!!
//  *  
//  * This Library is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.
//  *
//  * This Library is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License v3.0
//  * along with the Arduino SPIAsEEPROM Library.  If not, see
//  * <http://www.gnu.org/licenses/>.
//  */

// #if defined(USE_SPI_EEPROM)

// #include "FlashAsEEPROMBase.h"

// FLASH_EEPROM_BaseClass::FLASH_EEPROM_BaseClass()
// {
//     //Class indicating if the emulated EEPROM flash is initialized  
//    _EmulatedEEPROMAvailable=false;

//     //Class variable storing number of ones counted in adres translation block
//     _nrOfOnes = 0;

//     //Class variable storing what sector we are working in.
//     _sectorFlash = 0;

//     //Class variable storing what flash address we are working in.
//     _addressFLASH = 0;

//     //Class bool indicating if the flash is initialized and available for use
//     _FlashAvailable=false;
// }

// int8_t FLASH_EEPROM_BaseClass::initialize(bool flashavailable)
// {
//     bool formatted = false;
//     _FlashAvailable = flashavailable;
//     _EmulatedEEPROMAvailable = false;

//     if(_FlashAvailable)
//     {
//       formatted = checkForMagicNumbers(); 

//       //If not formatted format flash. This takes 10 seconds or more!
//       if(!formatted){
//         clear();
//         //check if format succeeded
//         formatted = checkForMagicNumbers();
//       }

//       if(formatted){_EmulatedEEPROMAvailable=true;}
//     }
//     return _EmulatedEEPROMAvailable;
// }

// byte FLASH_EEPROM_BaseClass::read(uint16_t addressEEPROM){
//     //version 0.1 does not check magic number

//     byte EEPROMbyte;

//     //Check if address is outside of the maximum. return zero if address is out of range.
//     if (addressEEPROM > EEPROM_EMULATION_SIZE){addressEEPROM = EEPROM_EMULATION_SIZE - 1; return 0;}  
    
//     //Check at what flash sector the EEPROM byte information resides 
//     _sectorFlash = addressEEPROM/EEPROM_BYTES_PER_SECTOR;

//     //Check at what flash address the EEPROM byte information resides 
//     _addressFLASH = (_sectorFlash*FLASH_SECTOR_SIZE) + ((addressEEPROM % EEPROM_BYTES_PER_SECTOR) + 1) * FLASH_SIZE_PER_EEPROM_BYTE;

//     readFlashBytes(_addressFLASH, _ReadWriteBuffer, FLASH_SIZE_PER_EEPROM_BYTE);

//     //calculate address of the valid data by couting the bits in the Address translation section
//     _nrOfOnes = count(_ReadWriteBuffer, ADDRESS_TRANSLATION_SIZE);

//     //The buffer is already filled with the latest information. Read the valid data at the correct address.
//     EEPROMbyte = _ReadWriteBuffer[_nrOfOnes];

//     return EEPROMbyte;
// }

// int8_t FLASH_EEPROM_BaseClass::write(uint16_t addressEEPROM, byte val){    
//     //Check if address is outside of the maximum. limit to get inside maximum and return an error.
//     if (addressEEPROM > EEPROM_EMULATION_SIZE){addressEEPROM = EEPROM_EMULATION_SIZE - 1; return -1;}  
    
//     //read the current value
//     uint8_t readValue = read(addressEEPROM);

//     //After reading the current byte all global variables containing information about the address are set correctly. 

//     //only write if value is changed. 
//     if (readValue != val){

//       //Check if section is full and an erase must be performed.
//       if (_nrOfOnes < ADDRESS_TRANSLATION_SIZE + 1){

//         //First read all the values in this sector that will get distroyed when erasing
//         byte tempBuf[EEPROM_BYTES_PER_SECTOR];
//         for(uint16_t i = 0; i<EEPROM_BYTES_PER_SECTOR; i++){
//             uint16_t TempEEPROMaddress = (_sectorFlash*EEPROM_BYTES_PER_SECTOR) + i;
//             tempBuf[i] = read(TempEEPROMaddress);
//         }

//         //Now erase the sector
//         eraseFlashSector(_sectorFlash*FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);

//         //Write the magic numbers 
//         writeMagicNumbers(_sectorFlash);

//         //write all the values back
//         for(uint8_t i=0; i<EEPROM_BYTES_PER_SECTOR; i++){
//             write((_sectorFlash*EEPROM_BYTES_PER_SECTOR) + i, tempBuf[i]);
//         }

//         //Do not forget to write the new value!
//         write(addressEEPROM, val);

//         //Return we have writen a whole sector. 
//         return EEPROM_BYTES_PER_SECTOR;
        
//       }

//       //determine the adress of the byte in the address translation section where one bit must be reset when writing new values 
//       uint8_t AdressInAddressTranslation = (_nrOfOnes - 1)/8;

//       //determine value of the infoblock byte after writing one more time.
//       uint8_t ValueInAddressTranslation = 0xFF << (8 - (_nrOfOnes - 1 - ((AdressInAddressTranslation) * 8)));  
      
//       //write the new adress translation value at the new location in buffer
//       _ReadWriteBuffer[AdressInAddressTranslation] = ValueInAddressTranslation;

//       //Write the new EEPROM value at the new location in the buffer.
//       _ReadWriteBuffer[_nrOfOnes-1] = val;

//       //Write the buffer to the undelying flash storage. 
//       // writeFlashBytes(_addressFLASH, _ReadWriteBuffer, FLASH_SIZE_PER_EEPROM_BYTE);
//       byte tempBuffer[1] = {0};
//       tempBuffer[0] = _ReadWriteBuffer[_nrOfOnes-1];
//       writeFlashBytes(_addressFLASH +_nrOfOnes-1, tempBuffer, 1);

//       tempBuffer[0] = _ReadWriteBuffer[AdressInAddressTranslation];
//       writeFlashBytes(_addressFLASH+AdressInAddressTranslation, tempBuffer, 1);
//       return 1;
//     }  
//   return 0;
// }

// int8_t FLASH_EEPROM_BaseClass::update(uint16_t addressEEPROM, uint8_t val){
//   return write(addressEEPROM, val);
// }

// int16_t FLASH_EEPROM_BaseClass::clear(){
//       uint32_t i;
//       for(i=0; i<FLASH_SECTORS_USED; i++ ){
//           eraseFlashSector(i*FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
//           writeMagicNumbers(i);
//       }
//       return i;
// }

// uint16_t FLASH_EEPROM_BaseClass::length(){ return EEPROM_EMULATION_SIZE; }


// bool FLASH_EEPROM_BaseClass::checkForMagicNumbers(){
//       bool magicnumbers = true;
//       for(uint32_t i=0; i<FLASH_SECTORS_USED; i++ ){
//           readFlashBytes(i*FLASH_SECTOR_SIZE, _ReadWriteBuffer, FLASH_SIZE_PER_EEPROM_BYTE);
//           if((_ReadWriteBuffer[0] != MAGICNUMBER1) | (_ReadWriteBuffer[1] != MAGICNUMBER2) | (_ReadWriteBuffer[2] != MAGICNUMBER3) | (_ReadWriteBuffer[3] != EEPROM_VERSION)){magicnumbers=false;}
//       }
//       return magicnumbers;
// }

// int8_t FLASH_EEPROM_BaseClass::writeMagicNumbers(uint32_t sector){ 
//     _ReadWriteBuffer[0] = MAGICNUMBER1;
//     _ReadWriteBuffer[1] = MAGICNUMBER2;
//     _ReadWriteBuffer[2] = MAGICNUMBER3;
//     _ReadWriteBuffer[3] = EEPROM_VERSION;
//     writeFlashBytes(sector*FLASH_SECTOR_SIZE, _ReadWriteBuffer, MAGICNUMBER_OFFSET); 
//   return true;
// }

// uint16_t FLASH_EEPROM_BaseClass::count(byte* buffer, uint16_t length){
//     byte tempBuffer[length];
//     memcpy(&tempBuffer, buffer, length);
//     uint16_t count=0;
//     for(uint8_t j=0; j < length; j++)
//       for(uint8_t i=0; i<8; i++){
//         if((tempBuffer[j] & 1) == 1){ //if current bit 1
//           count++;//increase count
//         }
//         tempBuffer[j]=tempBuffer[j]>>1;//right shift
//       }
//     return count;
// }

// int8_t FLASH_EEPROM_BaseClass::readFlashBytes(uint32_t address , byte* buffer, uint16_t length){return -1;}
// int8_t FLASH_EEPROM_BaseClass::writeFlashBytes(uint32_t address, byte* buffer, uint16_t length){return -1;}
// int8_t FLASH_EEPROM_BaseClass::eraseFlashSector(uint32_t address, uint16_t length){return -1;}


// // SPI_EEPROM_Class::SPI_EEPROM_Class()
// // {

// // }

// // byte SPI_EEPROM_Class::read(uint16_t addressEEPROM){
// //     //Check if emulated EEPROM is available if not yet start it first.
// //     if(!_EmulatedEEPROMAvailable){
// //       SPI.begin();
// //       SPI.setBitOrder(MSBFIRST);
// //       SPI.setDataMode(SPI_MODE0);
// //       SPI.setClockDivider(SPI_CLOCK_DIV2);
// //       begin(SPI, USE_SPI_EEPROM);
// //     }
    
// //     return FLASH_EEPROM_BaseClass::read(addressEEPROM);
// // }

// // int8_t SPI_EEPROM_Class::begin(SPIClass &_spi, uint8_t pinSPIFlash_CS=6){
// //     pinMode(pinSPIFlash_CS, OUTPUT);
// //     bool flashavailable;
// //     flashavailable = winbondSPIFlash.begin(_W25Q16,_spi, pinSPIFlash_CS);
// //     return FLASH_EEPROM_BaseClass::initialize(flashavailable);
// // }    

// // int8_t SPI_EEPROM_Class::readFlashBytes(uint32_t address, byte *buf, uint16_t length){
// //   while(winbondSPIFlash.busy());
// //   return winbondSPIFlash.read(address+EEPROM_FLASH_BASEADRESS, buf, length);
// // }

// // int8_t SPI_EEPROM_Class::writeFlashBytes(uint32_t address, byte *buf, uint16_t length){
// //   winbondSPIFlash.setWriteEnable(true);
// //   winbondSPIFlash.writePage(address+EEPROM_FLASH_BASEADRESS, buf, length);
// //   while(winbondSPIFlash.busy());
// //   return 0;
// // }

// // int8_t SPI_EEPROM_Class::eraseFlashSector(uint32_t address, uint16_t length){
// //   winbondSPIFlash.setWriteEnable(true);
// //   winbondSPIFlash.eraseSector(address+EEPROM_FLASH_BASEADRESS);
// //   while(winbondSPIFlash.busy());
// //   return 0;
// // }

// // SPI_EEPROM_Class EEPROM;

// // Internal_EEPROM_Class::Internal_EEPROM_Class()
// // {

// // }

// // byte Internal_EEPROM_Class::read(uint16_t addressEEPROM){
// //     //Check if emulated EEPROM is available if not yet start it first.
// //     // if(!_EmulatedEEPROMAvailable){
// //     //   SPI.begin();
// //     //   SPI.setBitOrder(MSBFIRST);
// //     //   SPI.setDataMode(SPI_MODE0);
// //     //   SPI.setClockDivider(SPI_CLOCK_DIV2);
// //     //   begin(SPI, USE_SPI_EEPROM);
// //     // }
    
// //     return FLASH_EEPROM_BaseClass::read(addressEEPROM);
// // }

// // int8_t Internal_EEPROM_Class::begin(SPIClass &_spi, uint8_t pinSPIFlash_CS=6){
// //     // pinMode(pinSPIFlash_CS, OUTPUT);
// //     // bool flashavailable;
// //     // flashavailable = winbondSPIFlash.begin(_W25Q16,_spi, pinSPIFlash_CS);
// //     // return FLASH_EEPROM_BaseClass::initialize(flashavailable);
// //     return -1;
// // }    

// // int8_t Internal_EEPROM_Class::readFlashBytes(uint32_t address, byte *buf, uint16_t length){
// //   // while(winbondSPIFlash.busy());
// //   // return winbondSPIFlash.read(address+EEPROM_FLASH_BASEADRESS, buf, length);
// //   return -1;
// // }

// // int8_t Internal_EEPROM_Class::writeFlashBytes(uint32_t address, byte *buf, uint16_t length){
// //   // winbondSPIFlash.setWriteEnable(true);
// //   // winbondSPIFlash.writePage(address+EEPROM_FLASH_BASEADRESS, buf, length);
// //   // while(winbondSPIFlash.busy());
// //   return 0;
// // }

// // int8_t Internal_EEPROM_Class::eraseFlashSector(uint32_t address, uint16_t length){
// //   // winbondSPIFlash.setWriteEnable(true);
// //   // winbondSPIFlash.eraseSector(address+EEPROM_FLASH_BASEADRESS);
// //   // while(winbondSPIFlash.busy());
// //   return 0;
// // }

// #endif


