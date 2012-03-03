
#ifndef COMMON_H_
#define COMMON_H_


/* type definition */
#define INT8  unsigned char
#define INT16 unsigned short
#define INT32 unsigned int
#define INT64 unsigned long long

#define SIGNINT8  signed char
#define SIGNINT16 signed short
#define SIGNINT32 signed int
#define SIGNINT64 signed long long



/* const definition */
#define _00_ (0x00)
#define _01_ (0x01)
#define _10_ (0x02)
#define _11_ (0x03)


#define _000_ (0x00)
#define _001_ (0x01)
#define _010_ (0x02)
#define _011_ (0x03)
#define _100_ (0x04)
#define _101_ (0x05)
#define _110_ (0x06)
#define _111_ (0x07)


#define _0000_ (0x00)
#define _0001_ (0x01)
#define _0010_ (0x02)
#define _0011_ (0x03)
#define _0100_ (0x04)
#define _0101_ (0x05)
#define _0110_ (0x06)
#define _0111_ (0x07)
#define _1000_ (0x08)
#define _1001_ (0x09)
#define _1010_ (0x0a)
#define _1011_ (0x0b)
#define _1100_ (0x0c)
#define _1101_ (0x0d)
#define _1110_ (0x0e)
#define _1111_ (0x0f)


#define _00000_ (0x00)
#define _00001_ (0x01)
#define _00010_ (0x02)
#define _00011_ (0x03)
#define _00100_ (0x04)
#define _00101_ (0x05)
#define _00110_ (0x06)
#define _00111_ (0x07)
#define _01000_ (0x08)
#define _01001_ (0x09)
#define _01010_ (0x0a)
#define _01011_ (0x0b)
#define _01100_ (0x0c)
#define _01101_ (0x0d)
#define _01110_ (0x0e)
#define _01111_ (0x0f)
#define _10000_ (0x10)
#define _10001_ (0x11)
#define _10010_ (0x12)
#define _10011_ (0x13)
#define _10100_ (0x14)
#define _10101_ (0x15)
#define _10110_ (0x16)
#define _10111_ (0x17)
#define _11000_ (0x18)
#define _11001_ (0x19)
#define _11010_ (0x1a)
#define _11011_ (0x1b)
#define _11100_ (0x1c)
#define _11101_ (0x1d)
#define _11110_ (0x1e)
#define _11111_ (0x1f)


#define _00000000_ (0x00)
#define _00000001_ (0x01)
#define _00000010_ (0x02)
#define _00000011_ (0x03)
#define _00000100_ (0x04)
#define _00000101_ (0x05)
#define _00000110_ (0x06)
#define _00000111_ (0x07)
#define _00001000_ (0x08)
#define _00001001_ (0x09)
#define _00001010_ (0x0a)
#define _00001011_ (0x0b)
#define _00001100_ (0x0c)
#define _00001101_ (0x0d)
#define _00001110_ (0x0e)
#define _00001111_ (0x0f)
#define _00010000_ (0x10)
#define _00010001_ (0x11)
#define _00010010_ (0x12)
#define _00010011_ (0x13)
#define _00010100_ (0x14)
#define _00010101_ (0x15)
#define _00010110_ (0x16)
#define _00010111_ (0x17)
#define _00011000_ (0x18)
#define _00011001_ (0x19)
#define _00011010_ (0x1a)
#define _00011011_ (0x1b)
#define _00011100_ (0x1c)
#define _00011101_ (0x1d)
#define _00011110_ (0x1e)
#define _00011111_ (0x1f)
#define _00100000_ (0x20)
#define _00100001_ (0x21)
#define _00100010_ (0x22)
#define _00100011_ (0x23)
#define _00100100_ (0x24)
#define _00100101_ (0x25)
#define _00100110_ (0x26)
#define _00100111_ (0x27)
#define _00101000_ (0x28)
#define _00101001_ (0x29)
#define _00101010_ (0x2a)
#define _00101011_ (0x2b)
#define _00101100_ (0x2c)
#define _00101101_ (0x2d)
#define _00101110_ (0x2e)
#define _00101111_ (0x2f)
#define _00110000_ (0x30)
#define _00110001_ (0x31)
#define _00110010_ (0x32)
#define _00110011_ (0x33)
#define _00110100_ (0x34)
#define _00110101_ (0x35)
#define _00110110_ (0x36)
#define _00110111_ (0x37)
#define _00111000_ (0x38)
#define _00111001_ (0x39)
#define _00111010_ (0x3a)
#define _00111011_ (0x3b)
#define _00111100_ (0x3c)
#define _00111101_ (0x3d)
#define _00111110_ (0x3e)
#define _00111111_ (0x3f)
#define _01000000_ (0x40)
#define _01000001_ (0x41)
#define _01000010_ (0x42)
#define _01000011_ (0x43)
#define _01000100_ (0x44)
#define _01000101_ (0x45)
#define _01000110_ (0x46)
#define _01000111_ (0x47)
#define _01001000_ (0x48)
#define _01001001_ (0x49)
#define _01001010_ (0x4a)
#define _01001011_ (0x4b)
#define _01001100_ (0x4c)
#define _01001101_ (0x4d)
#define _01001110_ (0x4e)
#define _01001111_ (0x4f)
#define _01010000_ (0x50)
#define _01010001_ (0x51)
#define _01010010_ (0x52)
#define _01010011_ (0x53)
#define _01010100_ (0x54)
#define _01010101_ (0x55)
#define _01010110_ (0x56)
#define _01010111_ (0x57)
#define _01011000_ (0x58)
#define _01011001_ (0x59)
#define _01011010_ (0x5a)
#define _01011011_ (0x5b)
#define _01011100_ (0x5c)
#define _01011101_ (0x5d)
#define _01011110_ (0x5e)
#define _01011111_ (0x5f)
#define _01100000_ (0x60)
#define _01100001_ (0x61)
#define _01100010_ (0x62)
#define _01100011_ (0x63)
#define _01100100_ (0x64)
#define _01100101_ (0x65)
#define _01100110_ (0x66)
#define _01100111_ (0x67)
#define _01101000_ (0x68)
#define _01101001_ (0x69)
#define _01101010_ (0x6a)
#define _01101011_ (0x6b)
#define _01101100_ (0x6c)
#define _01101101_ (0x6d)
#define _01101110_ (0x6e)
#define _01101111_ (0x6f)
#define _01110000_ (0x70)
#define _01110001_ (0x71)
#define _01110010_ (0x72)
#define _01110011_ (0x73)
#define _01110100_ (0x74)
#define _01110101_ (0x75)
#define _01110110_ (0x76)
#define _01110111_ (0x77)
#define _01111000_ (0x78)
#define _01111001_ (0x79)
#define _01111010_ (0x7a)
#define _01111011_ (0x7b)
#define _01111100_ (0x7c)
#define _01111101_ (0x7d)
#define _01111110_ (0x7e)
#define _01111111_ (0x7f)
#define _10000000_ (0x80)
#define _10000001_ (0x81)
#define _10000010_ (0x82)
#define _10000011_ (0x83)
#define _10000100_ (0x84)
#define _10000101_ (0x85)
#define _10000110_ (0x86)
#define _10000111_ (0x87)
#define _10001000_ (0x88)
#define _10001001_ (0x89)
#define _10001010_ (0x8a)
#define _10001011_ (0x8b)
#define _10001100_ (0x8c)
#define _10001101_ (0x8d)
#define _10001110_ (0x8e)
#define _10001111_ (0x8f)
#define _10010000_ (0x90)
#define _10010001_ (0x91)
#define _10010010_ (0x92)
#define _10010011_ (0x93)
#define _10010100_ (0x94)
#define _10010101_ (0x95)
#define _10010110_ (0x96)
#define _10010111_ (0x97)
#define _10011000_ (0x98)
#define _10011001_ (0x99)
#define _10011010_ (0x9a)
#define _10011011_ (0x9b)
#define _10011100_ (0x9c)
#define _10011101_ (0x9d)
#define _10011110_ (0x9e)
#define _10011111_ (0x9f)
#define _10100000_ (0xa0)
#define _10100001_ (0xa1)
#define _10100010_ (0xa2)
#define _10100011_ (0xa3)
#define _10100100_ (0xa4)
#define _10100101_ (0xa5)
#define _10100110_ (0xa6)
#define _10100111_ (0xa7)
#define _10101000_ (0xa8)
#define _10101001_ (0xa9)
#define _10101010_ (0xaa)
#define _10101011_ (0xab)
#define _10101100_ (0xac)
#define _10101101_ (0xad)
#define _10101110_ (0xae)
#define _10101111_ (0xaf)
#define _10110000_ (0xb0)
#define _10110001_ (0xb1)
#define _10110010_ (0xb2)
#define _10110011_ (0xb3)
#define _10110100_ (0xb4)
#define _10110101_ (0xb5)
#define _10110110_ (0xb6)
#define _10110111_ (0xb7)
#define _10111000_ (0xb8)
#define _10111001_ (0xb9)
#define _10111010_ (0xba)
#define _10111011_ (0xbb)
#define _10111100_ (0xbc)
#define _10111101_ (0xbd)
#define _10111110_ (0xbe)
#define _10111111_ (0xbf)
#define _11000000_ (0xc0)
#define _11000001_ (0xc1)
#define _11000010_ (0xc2)
#define _11000011_ (0xc3)
#define _11000100_ (0xc4)
#define _11000101_ (0xc5)
#define _11000110_ (0xc6)
#define _11000111_ (0xc7)
#define _11001000_ (0xc8)
#define _11001001_ (0xc9)
#define _11001010_ (0xca)
#define _11001011_ (0xcb)
#define _11001100_ (0xcc)
#define _11001101_ (0xcd)
#define _11001110_ (0xce)
#define _11001111_ (0xcf)
#define _11010000_ (0xd0)
#define _11010001_ (0xd1)
#define _11010010_ (0xd2)
#define _11010011_ (0xd3)
#define _11010100_ (0xd4)
#define _11010101_ (0xd5)
#define _11010110_ (0xd6)
#define _11010111_ (0xd7)
#define _11011000_ (0xd8)
#define _11011001_ (0xd9)
#define _11011010_ (0xda)
#define _11011011_ (0xdb)
#define _11011100_ (0xdc)
#define _11011101_ (0xdd)
#define _11011110_ (0xde)
#define _11011111_ (0xdf)
#define _11100000_ (0xe0)
#define _11100001_ (0xe1)
#define _11100010_ (0xe2)
#define _11100011_ (0xe3)
#define _11100100_ (0xe4)
#define _11100101_ (0xe5)
#define _11100110_ (0xe6)
#define _11100111_ (0xe7)
#define _11101000_ (0xe8)
#define _11101001_ (0xe9)
#define _11101010_ (0xea)
#define _11101011_ (0xeb)
#define _11101100_ (0xec)
#define _11101101_ (0xed)
#define _11101110_ (0xee)
#define _11101111_ (0xef)
#define _11110000_ (0xf0)
#define _11110001_ (0xf1)
#define _11110010_ (0xf2)
#define _11110011_ (0xf3)
#define _11110100_ (0xf4)
#define _11110101_ (0xf5)
#define _11110110_ (0xf6)
#define _11110111_ (0xf7)
#define _11111000_ (0xf8)
#define _11111001_ (0xf9)
#define _11111010_ (0xfa)
#define _11111011_ (0xfb)
#define _11111100_ (0xfc)
#define _11111101_ (0xfd)
#define _11111110_ (0xfe)
#define _11111111_ (0xff)



/* debug */
void INFOLOG(const char * format, ...);
#define PROGRAM_ERROR { printf("Program Error at File:%s Line:%d!\n", __FILE__, __LINE__); }
#define MARKPOINT { printf("(Info) In %s: line %d, function %s: ", __FILE__, __LINE__, __FUNCTION__); }




/* utilities */
extern INT32 andMask[32];

/* Logical shift left(8 bit) */
#define LSL8(i, n)  ( ( (i) << (n) ) & 0xff )
/* Logical shift left(16 bit) */
#define LSL16(i, n) ( ( (i) << (n) ) & 0xffff )
/* Logical shift left(32 bit) */
#define LSL32(i, n) ( ( (i) << (n) ) & 0xffffffff )

/* Logical shift right(zero extend, 8 bit) */
#define LSR8(i, n)  ( ( (i) >> (n) ) & ( andMask[ 7 - (n) ] ) )
/* Logical shift right(zero extend, 16 bit) */
#define LSR16(i, n) ( ( (i) >> (n) ) & ( andMask[ 15 - (n) ] ) )
/* Logical shift right(zero extend, 32 bit) */
#define LSR32(i, n) ( ( (i) >> (n) ) & ( andMask[ 31 - (n) ] ) )

/* Arithmetic shift left(8 bit) */
#define ASL8(i, n)  ( ( (i) << (n) ) & 0xff )
/* Arithmetic shift left(16 bit) */
#define ASL16(i, n) ( ( (i) << (n) ) & 0xffff )
/* Arithmetic shift left(32 bit) */
#define ASL32(i, n) ( ( (i) << (n) ) & 0xffffffff )

/* Arithmetic shift right(sign extend, 8 bit) */
#define ASR8(i, n)  ( ( (i) & 0x80 ) ? ( ( (i) >> (n) ) | ( 0xff << ( 8 - (n) ) ) ) : ( LSR8(i, n) ) )
/* Arithmetic shift right(sign extend, 16 bit) */
#define ASR16(i, n) ( ( (i) & 0x8000 ) ? ( ( (i) >> (n) ) | ( 0xffff << ( 16 - (n) ) ) ) : ( LSR16(i, n) ) )
/* Arithmetic shift right(sign extend, 32 bit) */
#define ASR32(i, n) ( ( (i) & 0x80000000 ) ? ( ( (i) >> (n) ) | ( 0xffffffff << ( 32 - (n) ) ) ) : ( LSR32(i, n) ) )

/* rotate right(8 bit) */
#define ROR8(n, b)  ( LSR8(n, b)  | ( (n) << ( 8  - (b) ) ) )
/* rotate right(16 bit) */
#define ROR16(n, b) ( LSR16(n, b) | ( (n) << ( 16 - (b) ) ) )
/* rotate right(32 bit) */
#define ROR32(n, b) ( LSR32(n, b) | ( (n) << ( 32 - (b) ) ) )


/* only for read */
#define GETBIT(n, b)          ( ( (n) >> (b) ) & 1)
#define GETBITS(n, high, low) ( ( (n) >> (low) ) & andMask[high - low] )



/* only for write */
#define SETBIT(n, b)            ( (n) | ( 1 << (b) ) )
#define CLEARBIT(n, b)          ( (n) & ( ~( 1 << (b) ) ) )
#define ASSIGNBIT(n, b, value)  ( (value) ? SETBIT(n, b) : CLEARBIT(n, b) )
#define ASSIGNBITS(n, high, low, value) ( ( (n) & ( ~( andMask[ (high) - (low) ] << (low) ) ) ) | ( ( (value) & andMask[ (high) - (low) ] ) << (low) ) )


/*s1 and s2 MUST be INT64! */
INT64 multiplyUnsigned32(INT64 s1, INT64 s2);

/*s1 and s2 MUST be SIGNINT64! */
SIGNINT64 multiplySigned32(SIGNINT64 s1, SIGNINT64 s2);

/* n(from bit(0)-bit(b) ) will extend sign to 32bits */
INT32 SignExtend(INT32 n, int b);



/* global data */
typedef struct
{
  int argc;
  char ** argv;
}ARMGO;


extern ARMGO armgo;


#endif
