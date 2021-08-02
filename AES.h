//---------------------------------------------------------------------------
#pragma once
//---------------------------------------------------------------------------
#include <stdint.h>
#include <string.h>
//---------------------------------------------------------------------------
class CAES
{
private:
   uint32_t key[60];

   static const uint8_t SBox[256];
   static const uint8_t InvSBox[256];

   static uint32_t RotWord(uint32_t Value);
   static uint32_t SubWord(uint32_t Value);
   static uint8_t Gmult(uint8_t A, uint8_t B);
   static uint32_t CoefMult(uint32_t A, uint32_t B);
   static uint32_t RCon(uint8_t Value);
   static void Subuint8_ts(uint8_t *State);
   static void ShiftRows(uint8_t *State);
   static void MixColumns(uint8_t *State);
   static void InvSubuint8_ts(uint8_t *State);
   static void InvShiftRows(uint8_t *State);
   static void InvMixColumns(uint8_t *State);
   void AddRoundKey(uint8_t *State, int Round);

public:
   void SetKey(const uint8_t Key[32]);
   void Encrypt(const uint8_t Input[16], uint8_t Output[16]);
   void Decrypt(const uint8_t Input[16], uint8_t Output[16]);
};
//---------------------------------------------------------------------------
