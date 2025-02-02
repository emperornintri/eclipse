#include "string.h"

static const unsigned int count_trailing_zeroes_table[64] = 
{
   0,  1,  2, 18,  3, 10, 19, 39, 
   7,  4, 11, 30, 26, 20, 40, 51, 
  16,  8,  5, 24, 14, 12, 31, 45, 
  36, 27, 21, 33, 47, 41, 52, 57, 
  63, 17,  9, 38,  6, 29, 25, 50, 
  15, 23, 13, 44, 35, 32, 46, 56, 
  62, 37, 28, 49, 22, 43, 34, 55, 
  61, 48, 42, 54, 60, 53, 59, 58, 
};

int countTrailingZeros (word_type word)
{
  if (word == 0)
  { 
    return 64;
  }
  return count_trailing_zeroes_table[((word & -word) * 0x0245434CB63AE7BF) >> 58];
}

int countLeadingZeros (word_type word)
{
  if (word == 0)
  {
    return 64;
  }
  word |= (word >> 1);
  word |= (word >> 2);
  word |= (word >> 4);
  word |= (word >> 8);
  word |= (word >> 16);
  word |= (word >> 32); 
  if (word == 0xFFFFFFFFFFFFFFFF){
    return 0;
  }
  word = (word + 1) >> 1;
  return 63 - count_trailing_zeroes_table[((word & -word) * 0x0245434CB63AE7BF) >> 58];
}

static inline word_type repeatBytes (unsigned char byte)
{
  return (((word_type) - 1) / 0xFF) * byte;
}

static inline word_type findLowestZero (word_type word)
{
  word_type least_significant_byte = repeatBytes (0x01);
  word_type most_significant_byte = repeatBytes (0x80);
  return (word - least_significant_byte) & ~ word & most_significant_byte;
}

static inline unsigned int getFirstOne (word_type word)
{
  int result;
  result = countTrailingZeros (word);
  return result / CHAR_BIT;
}

static inline unsigned int getFirstZeroIndex (word_type word)
{
  word = findLowestZero (word);
  return getFirstOne (word);
}

static inline int hasZero (word_type word)
{
  return findLowestZero (word) != 0;
}

static inline word_type findAllZeros (word_type word)
{
  word_type mask = repeatBytes (0x7F);
  return ~ (((word & mask) + mask) | word | mask);
}

static inline word_type shiftWord (word_type word, pointer_as_int string_adress_as_int)
{
  return word >> (CHAR_BIT * (string_adress_as_int % sizeof (word_type)));
}

size_type getStringLength (const char * string)
{
  const pointer_as_int string_adress_as_int = (pointer_as_int) (string);
  const word_type * word_pointer = (const word_type * ) ((string_adress_as_int) & - ((pointer_as_int) (sizeof (word_type))));
  word_type word = * word_pointer;
  word_type mask = shiftWord (findAllZeros (word), string_adress_as_int);
  if (mask != 0)
  {
    return getFirstOne (mask);
  }
  do
  {
    word = * ++ word_pointer;
  }
  while (! hasZero (word));
  return ((const char * ) word_pointer) + getFirstZeroIndex (word) - string;
}