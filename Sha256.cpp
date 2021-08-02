//---------------------------------------------------------------------------
#include "Sha256.h"
//---------------------------------------------------------------------------
const uint32_t CSha256::k[64] = {
   0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
   0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
   0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
   0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
   0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
   0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
   0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
   0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};
//---------------------------------------------------------------------------

void CSha256::Init()
{
   a = 0x6A09E667;
   b = 0xBB67AE85;
   c = 0x3C6EF372;
   d = 0xA54FF53A;
   e = 0x510E527F;
   f = 0x9B05688C;
   g = 0x1F83D9AB;
   h = 0x5BE0CD19;
   length = 0;
   bufferPos = 0;
}
//---------------------------------------------------------------------------

void CSha256::Calculate(const void *Data, uint32_t Size)
{
   uint8_t *data = (uint8_t*)Data;
   length += Size * 8;
   while (Size > 0)
   {
      uint32_t delta = (Size < 64 - bufferPos)?Size:64 - bufferPos;
      memcpy(&buffer[bufferPos], data, delta);
      Size -= delta;
      bufferPos += delta;
      data += delta;
      if (bufferPos >= 64)
      {
         bufferPos = 0;
         CalculateBuffer();
      }
   }
}
//---------------------------------------------------------------------------

void CSha256::Final()
{
   buffer[bufferPos] = 0x80;
   bufferPos++;
   if (bufferPos <= 56)
   {
      memset(&buffer[bufferPos], 0, 56 - bufferPos);
   }
   else
   {
      memset(&buffer[bufferPos], 0, 64 - bufferPos);
      CalculateBuffer();
      memset(buffer, 0, 56);
   }
   uint8_t *temp = (uint8_t*)&length;
   buffer[56] = temp[7];
   buffer[57] = temp[6];
   buffer[58] = temp[5];
   buffer[59] = temp[4];
   buffer[60] = temp[3];
   buffer[61] = temp[2];
   buffer[62] = temp[1];
   buffer[63] = temp[0];
   CalculateBuffer();
}
//---------------------------------------------------------------------------

uint32_t CSha256::Rotate(uint32_t Value, uint8_t Delta)
{
   return (((Value) >> (Delta)) | ((Value) << (32 - (Delta))));
}
//---------------------------------------------------------------------------

void CSha256::CalculateBuffer()
{
   uint32_t aa = a;
   uint32_t bb = b;
   uint32_t cc = c;
   uint32_t dd = d;
   uint32_t ee = e;
   uint32_t ff = f;
   uint32_t gg = g;
   uint32_t hh = h;

   uint32_t w[80];
   for (int i = 0; i < 16; i++)
   {
      w[i] = uint32_t(buffer[i * 4]) << 24;
      w[i] |= uint32_t(buffer[i * 4 + 1]) << 16;
      w[i] |= uint32_t(buffer[i * 4 + 2]) << 8;
      w[i] |= uint32_t(buffer[i * 4 + 3]);
   }
   for (int i = 16; i < 64; i++)
   {
      uint32_t s0 = Rotate(w[i - 15], 7) ^ Rotate(w[i - 15], 18) ^ (w[i - 15] >> 3);
      uint32_t s1 = Rotate(w[i - 2], 17) ^ Rotate(w[i - 2], 19) ^ (w[i - 2] >> 10);
      w[i] = w[i - 16] + s0 + w[i - 7] + s1;
   }

   for(int i = 0; i < 64; i++)
   {
      uint32_t t0 = (Rotate(a, 2) ^ Rotate(a, 13) ^ Rotate(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
      uint32_t t1 = h + (Rotate(e, 6) ^ Rotate(e, 11) ^ Rotate(e, 25)) + ((e & f) ^ ((~e) & g)) + k[i] + w[i];

      h = g;
      g = f;
      f = e;
      e = d + t1;
      d = c;
      c = b;
      b = a;
      a = t0 + t1;
   }

   a += aa;
   b += bb;
   c += cc;
   d += dd;
   e += ee;
   f += ff;
   g += gg;
   h += hh;
}
//---------------------------------------------------------------------------

void CSha256::GetHash(uint8_t Result[32]) const
{
   uint32_t *result = (uint32_t*)Result;
   result[0] = (a >> 24) | (a << 24) | ((a >> 8) & 0xFF00) | ((a & 0xFF00) << 8);
   result[1] = (b >> 24) | (b << 24) | ((b >> 8) & 0xFF00) | ((b & 0xFF00) << 8);
   result[2] = (c >> 24) | (c << 24) | ((c >> 8) & 0xFF00) | ((c & 0xFF00) << 8);
   result[3] = (d >> 24) | (d << 24) | ((d >> 8) & 0xFF00) | ((d & 0xFF00) << 8);
   result[4] = (e >> 24) | (e << 24) | ((e >> 8) & 0xFF00) | ((e & 0xFF00) << 8);
   result[5] = (f >> 24) | (f << 24) | ((f >> 8) & 0xFF00) | ((f & 0xFF00) << 8);
   result[6] = (g >> 24) | (g << 24) | ((g >> 8) & 0xFF00) | ((g & 0xFF00) << 8);
   result[7] = (h >> 24) | (h << 24) | ((h >> 8) & 0xFF00) | ((h & 0xFF00) << 8);
}
//---------------------------------------------------------------------------
