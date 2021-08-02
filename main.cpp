//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
//---------------------------------------------------------------------------
#include "Sha256.h"
#include "AES.h"
//---------------------------------------------------------------------------
#define BUFFER_SIZE  4096
//---------------------------------------------------------------------------

bool Encrypt(CAES &Crypto, FILE *Input, FILE *Output)
{
   int length;
   uint8_t buffer[BUFFER_SIZE];
   while (true)
   {
      length = fread(buffer, 1, BUFFER_SIZE, Input);
      if (length < 0)
      {
         perror("Read error!\n");
         return false;
      }
      if (length < BUFFER_SIZE)
      {
         if (length == 0)
         {
            return true;
         }
         memset(&buffer[length], 0, BUFFER_SIZE - length);
      }
      for (int i = 0; i < length; i += 16)
      {
         Crypto.Encrypt(&buffer[i], &buffer[i]);
      }
      fwrite(buffer, 1, (length & 15)?(length | 15) + 1:length, Output);
   }
}
//---------------------------------------------------------------------------

bool Decrypt(CAES &Crypto, FILE *Input, FILE *Output)
{
   int length;
   uint8_t buffer[BUFFER_SIZE];
   while (true)
   {
      length = fread(buffer, 1, BUFFER_SIZE, Input);
      if (length < 0)
      {
         perror("Read error!\n");
         return false;
      }
      if (length < BUFFER_SIZE)
      {
         if (length == 0)
         {
            return true;
         }
         memset(&buffer[length], 0, BUFFER_SIZE - length);
      }
      for (int i = 0; i < length; i += 16)
      {
         Crypto.Decrypt(&buffer[i], &buffer[i]);
      }
      fwrite(buffer, 1, (length & 15)?(length | 15) + 1:length, Output);
   }
}
//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
   if (argc < 5)
   {
      return -1;
   }

   CSha256 hash;
   CAES crypto;
   bool encrypt;
   uint8_t key[32];

   if (!strcmp(argv[1], "-e"))
   {
      encrypt = true;
   }
   else if (!strcmp(argv[1], "-d"))
   {
      encrypt = false;
   }
   else
   {
      perror("Unsupported command!\n");
      return -1;
   }

   hash.Init();
   hash.Calculate(argv[2], strlen(argv[2]));
   hash.Final();
   hash.GetHash(key);

   crypto.SetKey(key);

   FILE *input = fopen(argv[3], "rb");
   if (input == nullptr)
   {
      return -1;
   }
   FILE *output = fopen(argv[4], "wb");
   if (output == nullptr)
   {
      fclose(input);
      return -1;
   }

   bool result;
   if (encrypt)
   {
      result = Encrypt(crypto, input, output);
   }
   else
   {
      result = Decrypt(crypto, input, output);
   }

   fclose(input);
   fclose(output);
   return result?0:-1;
}
//---------------------------------------------------------------------------
