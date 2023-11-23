/********************************************************************
 * Filename: main.cpp
 * Main test driver
 * Syntax: tomasulo.exe <input.asm> <init.txt>
 * Authors:
 * Kan Liu
 * Piyush Ranjan Satapathy
 * Ricky J. Sethi
 * Project for CS203A The University of California,Riverside
 * Date: 11/15/2004
 ********************************************************************/

#include "TOMASULO.h"
#include <string>
#include <iostream>

using namespace std;

//====================================================================
// Some Constant and Global Variables
//====================================================================
string inputfile = "input.txt";
string initfile = "init.txt"; 

//====================================================================
// Main:
//====================================================================
int main(int argc, char ** argv) {
    if(argc == 2) {
        inputfile = argv[1];
    } else if(argc == 3) {
        inputfile = argv[1];
        initfile = argv[2];
    }
    
    cout << "Using the assembly filename: " << inputfile << " and initialization file: " << initfile << endl;
    
    TOMASULO * simulator = new TOMASULO();
    
    // Setup:
    simulator->parse_input_assembly(inputfile.c_str());
    simulator->init_from_file(initfile.c_str());
    simulator->print_ins(cout);
    
    // Run:
    simulator->simu_run();
    simulator->print_final_stats();
    
    // Debugging:
    //simulator->print_mem();
    //simulator->print_fp_gpr();
    //simulator->print_stat_local();
    return 0;
} 
