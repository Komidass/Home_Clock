#ifndef BIT_CALC_H
#define BIT_CALC_H

#define Set_Bit(Var,Bit_No)    (Var) |=  (1 << (Bit_No))

#define Clear_Bit(Var,Bit_No)  (Var) &= ~(1 << (Bit_No))

#define Toggle_Bit(Var,Bit_No) (Var) ^=  (1 << (Bit_No))

#define Get_Bit(Var,Bit_No)    ( ((Var) >> (Bit_No)) & (1) )

#define Get_4_Bits(Var,Bit_No)    (((Var) >> (Bit_No*4)) & (0x0F))

#define Assign_Bit(Var,Bit_No,Value) do{if(Value == 1) Set_Bit(Var,Bit_No); \
										else Clear_Bit(Var,Bit_No);}while(0)

#define ASSIGN_2BITS(Var,Bit_No,Value) do{if(Value == 0b00) {Clear_Bit(Var,Bit_No*2);\
                                                             Clear_Bit(Var,Bit_No*2+1);}\
                                        else if(Value == 0b11) {Set_Bit(Var,Bit_No*2);\
                                                                Set_Bit(Var,Bit_No*2+1);}\
                                        else if(Value == 0b10) {Clear_Bit(Var,Bit_No*2);\
                                                                Set_Bit(Var,Bit_No*2+1);}\
                                        else if(Value == 0b01) {Set_Bit(Var,Bit_No*2);\
                                                                Clear_Bit(Var,Bit_No*2+1);}}while(0)

#endif
