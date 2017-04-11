/***********************************************
Name: Cole McKnight
File: dfa.cpp
Description: Mini-DFA that produces data flow analysis for MIPS instructions.
Date: 4/10/17
***********************************************/
#include <iostream> 
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include "inst.h"
using namespace std;


int main(int argc, char* argv[])
{
    
    int ld = 1; //load delay
    bool rr = false; //register renaming 

/***********************************************
INPUT
***********************************************/
    ifstream infile; //creates ifstream

    if(argc == 1) //no input
    {
        printf("No input file.\n"); //handles no-arg error
        return -1;   
    } 
    else if(argc == 2) //only input file
    {
        infile.open(argv[1]); //creates ifstream
    }
    else if(argc == 3) //one option
    {
        string argv1(argv[1]);
        if(argv1 == "-r")     //checks for register renaming
        {
            rr = true;
            printf("Register renaming.\n");
        }else if(argv1[1] == 'l') //checks for load delay
        {
            ld = ((int)argv1.at(2) - 48);
        }else{printf("Argument(s) are invalid.\n");return -1;}
        infile.open(argv[2]); //creates ifstream
    }
    else if(argc == 4) //two options
    {
        string argv1(argv[1]);
        string argv2(argv[2]);
        if(argv1 == "-r")  //checks for register renaming
        {
            rr = true;
            printf("Register renaming.\n");
        }else{printf("Argument(s) are invalid.\n");return -1;}
        if(argv2[1] == 'l') //checks for load delay
        {
            ld = ((int)argv2.at(2) - 48);
        }else{printf("Argument(s) are invalid.\n");return -1;}
        infile.open(argv[3]); //creates ifstream
    }
    
/***********************************************
FILE ANALYSIS
***********************************************/

    if(!infile) //validates file
    {
        cout << "File is invalid" << endl;  
        return -1;
    }else{
        string line;
        vector<Inst> instr; //holds instructions
        int index = 0;
        //instruction vector population
        while(getline(infile,line))  //adds instructions to vector
        {
            Inst in(index,line);
            instr.push_back(in);
            index++;
        }

        //register array population
        for(int i = 0;i < instr.size();i++)
        {
            cout << endl << instr[i].fullI;
            int rIndex = 0;
        }

/***********************************************
REGISTER RENAMING
***********************************************/

        if(rr)
        {
            int renameIndex = 10;
            for(int i = 0;i < instr.size();i++) 
            {
                int reg = instr[i].regs[0];
                for(int j = i;j < instr.size();j++) 
                {
                    for(int k = 0;k < 3;k++)
                    {
                        if(instr[j].regs[k] == reg)
                        {
                            instr[j].regs[k] = renameIndex;
                        }
                    }
                }
                renameIndex++;
            }
        }

/***********************************************
DATA FLOW ANALYSIS 
***********************************************/
        for(int i = 0;i < instr.size();i++) // prints initial instructions
        {
            cout << endl << instr[i].fullI;
        }
        cout << endl;

        //populate levels
        vector< vector<Inst> > levels; //holds instructions for each level
        while(instr.size() > 0) //levels
        {
            vector<Inst> temp;
            levels.push_back(temp);
            cout << endl << "-------------------" << endl;
            for(int i = instr.size()-1;i > -1;i--) //instructions
            {
                bool isDep = false;
                for(int j = i-1;j > -1;j--) //regs
                {
                    Inst a = instr[i];
                    Inst b = instr[j];
                    if(a.index == b.index)
                        isDep = true;
                    if(a.regs[0] == b.regs[0]) //checks for WAW 
                    {
                        isDep = true;
                        cout << "inst " << a.index << " WAW dependent on inst " << b.index << " through reg " << a.regs[0] << endl;
                    }
                    if(a.regs[0] == b.regs[1] || a.regs[0] == b.regs[2]) //checks for WAR
                    {
                        isDep = true;
                        cout << "inst " << a.index << " WAR dependent on inst " << b.index << " through reg " << a.regs[0] << endl;
                    }
                    if(a.regs[1] == b.regs[0] || a.regs[2] == b.regs[0]) //checks for RAW 
                    {
                        isDep = true;
                        cout << "inst " << a.index << " RAW dependent on inst " << b.index << " through reg " << b.regs[0] << endl;
                    }
                }
                if(!isDep) //adds instruction to level and removes from instr vector
                {
                    levels.at(levels.size()-1).push_back(instr[i]);
                    instr.erase(instr.begin() + i);
                }
            }
        }

/***********************************************
OUTPUT 
***********************************************/

        printf("Load delay set to %x.\n",ld);
        int delay = 0;
        int i;
        for(i = 0;i < levels.size();i++) //goes through each level
        {
            bool delayBool = false;
            if(i == 0)
            {
                cout << "level 0 instructions: " << endl;
            }else{cout << "level " << ((i-1)+delay) << " instructions: " << endl;}
            for(int j = levels.at(i).size()-1;j > -1;j--) //prints each instruction in level
            {
                cout << "   " << levels.at(i).at(j).index << " " << levels.at(i).at(j).fullI;
                if(rr) //checks for renaming
                {
                    cout << " with renaming d/s1/s2 regs to " << levels.at(i).at(j).regs[0] << " " << levels.at(i).at(j).regs[1] << " " << levels.at(i).at(j).regs[2];
                }
                cout << endl;   
                if(levels.at(i).at(j).fullI.substr(0,2) == "lw") //checks for load delay
                    delayBool = true;
            }
            if(delayBool) //sets delay
                delay = ld;
        }
        cout << "The data flow can be executed in " << ((i-1) + ld) << " cycles" << endl;
    }    
   
    return 0;
}


