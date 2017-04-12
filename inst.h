/**********************************************************************
* Author: Cole McKnight, Luke Morrow
* File: inst.h
* Modified On: 4/10/17
* Description: Header for Inst, which represents a single MIPS instruction.
***********************************************************************/
#ifndef INST_H
#define INST_H

#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

class Inst{
public:
    int index; //index of instruction in program
    string fullI; //full instruction(for printing)
    int regs[3]; //registers used 
   	
    Inst(int,string); 
       
    void print();
 
};


#endif


