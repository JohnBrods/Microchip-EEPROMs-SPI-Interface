                                                    /*Anyone is free to copy, modify, publish, use, compile or
                                                    distribute this software, either in source code form or as a compiled
                                                    binary, for non-commercial use only. (i.e. YOU MAY NOT SELL IT)
                                                    John B 22/06/2021

                                                    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
                                                    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
                                                    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
                                                    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
                                                    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
                                                    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
                                                    OTHER DEALINGS IN THE SOFTWARE.*/



void Write_Protect_All(){

   LATC1_bit = 0;
   SPI1_Write(0b00000001);
   SPI1_Write(0b10001100);  //Lock Everything.
   LATC1_bit = 1;
}


void Remove_Write_Protect(){

   LATC1_bit = 0;
   SPI1_Write(0b00000001);
   SPI1_Write(0b10000000);  //Unlock Everything.
   LATC1_bit = 1;
}



  unsigned char buffer;
  unsigned char status1 = 0;
Read_EEprom_Status_Register(){

    LATC1_bit = 0;
    SPI1_Write(0b00000101);          //Read Ststus Register
    status1 = SPI1_Read(buffer);
    LATC1_bit = 1;
    Write_Number(status1,0,36,Lime);
    return status1;
}

void Enable_Write_EEprom(){

   LATC1_bit = 0;
   SPI1_Write(0b00000110); // Set The Write Enable Latch (Enable Write Operations)
   LATC1_bit = 1;
}

void Disable_Write_EEprom(){

   LATC1_bit = 0;
   SPI1_Write(0b00000100); // Reset The Write Enable Latch (Disable Write Operations)
   LATC1_bit = 1;          // WRITE DISABLE SEQUENCE (WRDI)
}

 //------------------------------------------------------------------------------------------------------------------------------------------------
     //To Write More Than One Page To The EEPROM   MICROCHIP 25LC1024

      page = 0;
      address = 98352;

      for (i = 0; i<128; i++){

      Enable_Write_EEprom();

      LATC1_bit = 0;
      SPI1_Write(0b00000010); //Write data to memory array beginning at selected address

      SPI1_Write(address>>16);
      SPI1_Write(address>>8);
      SPI1_Write(address);

      for(j = page; j < page+256; j++){
      SPI1_Write(Lakeboat128128_bmp[j]);        //<<<<<<<<put your own picture here
      address++;
      }

      LATC1_bit = 1;
      Read_EEprom_Status_Register();

     if (status&0x01){
      Write_Number(status1,0,70,Yellow);
      Read_EEprom_Status_Register();
      }

      page = page + 256;
     }
     
  //-----------------------------------------------------------------------------------------------------------------------------------
     
      // To Read 32768 Bytes And Show It On A Screen
      
       LATC1_bit = 0;
       SPI1_Write(0b00000011);  //Read data from memory array beginning at selected address
       SPI1_Write(address>>16);
       SPI1_Write(address>>8);
       SPI1_Write(address);

       for (i = 0; i <32768; i++){

       EEprom_Data = SPI1_Read(buffer);

       TFT_Write_Data_ST7735S(EEprom_Data);
       }
       LATC1_bit = 1;