#include <stdio.h>  
#include <stdlib.h>






using Byte = unsigned char; // unsigned 8bit #.
using Word = unsigned short;// unsigned 16bit #.

using u32 = unsigned int; //unsigned 32 bit #.


struct MEM{
    static constexpr u32 MAX_MEM = 1024 * 64;// how much mem the emulator will sim.
    Byte data[MAX_MEM];// acts as a memory bank.

    void Initialise(){// when initialising it sets every byte to 0.
        for ( u32 i=0; i < MAX_MEM; i++)
        {
            data[i] = 0;
        }
    }

    Byte operator[]( u32 Address ) const// reads 1 byte from memory.
    {

        return data[Address];
    }

    Byte& operator[]( u32 Address ) // write 1 byte from memory.
    {

        return data[Address];
    }
};



struct CPU{
    Word pc; // program counter is 16 bits.
    Byte sp; // stack pointer points to the next avalible location on the stack.

    Byte a, x, y; //registers x input y input and accumulator.

    Byte c : 1; // all the flags
    Byte z : 1;
    Byte i : 1;
    Byte n : 1;
    Byte v : 1;
    Byte b : 1;
    Byte d : 1;

    void Reset( MEM& memory)// on button
    {

        pc = 0xFFFC;
        sp = 0x0100;
        d = 0;
        a = x = y = 0; //initilizing a,x,y to 0
        c = z = i = n = v = b = d = 0; //initilizing all flags
        memory.Initialise();

    }

    Byte FetchByte ( u32& Ticks, MEM& memory )//fetches the next instruction
    {
        Byte data = memory[pc];
        pc++;
        Ticks --;
        return data;
    }

    Byte ReadByte( u32& Ticks, Byte address, MEM& memory )
    {
        Byte data = memory[address];
        Ticks --;
        return data;
    }

    // operation codes
    static constexpr Byte
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5;

    void LDAStatus()
    {
        z = (a == 0);
        n = (a & 0b10000000) > 0;
    }

    void Execute( u32 Ticks, MEM& memory)
    {
        while ( Ticks > 0)
        {   
            Byte Ins = FetchByte( Ticks, memory); // gets the instruction from memory
            switch (Ins)
            {
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte( Ticks, memory);
                    a = Value;
                    
                    LDAStatus();
                }break;
                
                case INS_LDA_ZP:
                {
                    Byte ZeroPageAdress = FetchByte(Ticks, memory);
                    a = ReadByte(Ticks, ZeroPageAdress, memory);
                    LDAStatus();
                }break;
                default:
                {
                    printf("instruction not allowed");
                }break;
            }
        }
    
    }
};





int main(){

    MEM mem;
    CPU cpu;
    cpu.Reset( mem );
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    cpu.Execute( 3, mem );
    printf("The instruction is, %02X. The A register is set to: %02X\n ", cpu.INS_LDA_IM ,cpu.a);
    return 0;


}