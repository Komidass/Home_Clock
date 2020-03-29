#ifndef TYPED_H
#define TYPED_H
typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef unsigned long int u64;
typedef signed char s8;
typedef signed int s32;
typedef unsigned short int s16;
typedef signed long int s64;
typedef float f32;
typedef double f64;
typedef _Bool u1;
typedef union
{
	struct
	{
		u8 Bit_0 : 1;
		u8 Bit_1 : 1;
		u8 Bit_2 : 1;
		u8 Bit_3 : 1;
		u8 Bit_4 : 1;
		u8 Bit_5 : 1;
		u8 Bit_6 : 1;
		u8 Bit_7 : 1;
	}Bit_Access;
	u8 ByteAccess;
}Register_8Bit;

#endif
