/**********************************************************************
* Author: Cole McKnight, Luke Morrow
* File: inst.cpp
* Modified On: 4/10/17
* Description: Contains Inst methods
***********************************************************************/

#include <math.h>
#include <stdio.h>
#include <string>
#include <iostream> 
#include "inst.h"
using namespace std;

Inst::Inst(int i,string line)
{
    index = i;
    fullI = line;
    int rIndex = 0;
    //populates register array
    for(int i = 0;i < line.size();i++) 
    {
        if(line.at(i) == '$') //finds register tags
        {
            regs[rIndex] = ((int)line.at(i+1) - 48); //stores registers
            rIndex++;
            if(regs[rIndex] > 7 || regs[rIndex] < 1) //sets blank spaces in array to 0
                regs[rIndex] = 0;
        }
    } 
}

void Inst::print()
{
    cout << index << " " << fullI << endl;
}


