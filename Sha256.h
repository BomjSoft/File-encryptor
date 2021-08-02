//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <stdint.h>
#include <string.h>
//---------------------------------------------------------------------------
class CSha256
{
private:
   uint8_t buffer[64];
   uint32_t a;
   uint32_t b;
   uint32_t c;
   uint32_t d;
   uint32_t e;
   uint32_t f;
   uint32_t g;
   uint32_t h;
   uint64_t length;
   uint32_t bufferPos;

   static const uint32_t k[64];

   void CalculateBuffer();
   static uint32_t Rotate(uint32_t Value, uint8_t Delta);

public:
   void Init();
   void Calculate(const void *Data, uint32_t Size);
   void Final();

   void GetHash(uint8_t Result[32]) const;
};
//---------------------------------------------------------------------------
