/********************************************************************
 * File name: TOMASULO.h
 * This is a class declaration to simulate the Tomasulo algorithm
 * for an out-of-order execution pipeline architecture.
 * Authors:
 * Kan Liu
 * Piyush Ranjan Satapathy
 * Ricky J. Sethi
 * CS203A The University of California, Riverside
 * 11/15/2004
 ****************************************************************/ 

#include <deque>
#include <string>

using namespace std; 

#ifndef __TOMASULO_H__
#define __TOMASULO_H__ 

#define NUM_INTEGER_GPR 32 //define the number of general-purpose integer registers.
#define NUM_FLOAT_GPR 32 //define the number of general-purpose floating-point registers.
#define SIZE_MEM 32 //define the size of memory(words).
#define NUM_FUNCTION 5 //the number of Function Units.
#define CYCLES_INTEGER 1 //the number of cycles to finish an integer instruction.
#define EMPTY -1 //for convenient,define EMPTY to be -1
#define ZERO 0.0 // For memory comparison 

/*Implement 11 instructions where 5 of them are integer instructions, 6 of them are floating-point instructions.*/
enum Instrs {LD,SD,BEQ,BNE,ADD,ADD_D,ADDI,SUB,SUB_D,MULT_D,DIV_D};
enum FUs {READY=-1,ADDER=0,MULTIPLIER,DIVIDER,LOAD,STORE};

//Implement 5 Function Units.Add "READY" for convenience.
enum Res_States {CAL_AD,FIN_AD,NOT_EX,STARTED_EX,COMPLETED_EX};

//To show the current state of a reservation station.
//the data structure to represent the instruction status.
struct Instruction_Status {
    Instrs Op;
    int rd;
    int rs;
    int rt;
    int in_fetch;
    int issue;
    int exe_start;
    int exe_compl;
    int wri_back;
    //int res_id; //the index of the reservation station for this instruction.
};//end Instruction_Status

//the data structure represent the type and fu index and res index of a reservation station.
struct Res_Sta_Id {
    FUs type; //one of ADDER,MULTIPLIER,DIVIDER,LOAD,STORE
    int fu_in;
    int index; //0~
};//end Res_Sta_Id 

//the data structure to represent reservation stations.
struct Reservation_Sta {
    bool Busy;
    Instrs Op;
    float Vj;
    float Vk;
    struct Res_Sta_Id Qj;
    struct Res_Sta_Id Qk;
    int A;
    int des;
    Res_States curr_sta;
    int Exe;
    struct Instruction_Status* curr_ins;
};//end Reservation_Sta 

//the data structure to represent an instruction.
struct Instruction { //please refer enum Instrs {LD,SD,BEQ,BNE,ADD,ADD_D,ADDI,SUB,SUB_D,MULT_D,DIV_D};
    Instrs Op;
    int label;
    int rd;
    int rs;
    int rt;
};//end Instruction

//the data structure to represent the index of a reservation stations
struct Res_Fu_Index{
    int in_fu;//the index of the fus
    int in_res;//the index of res in a fu
};

//class declaration.
class TOMASULO {
 private:
    int In_Gpr[NUM_INTEGER_GPR]; //the array of general-purpose integer registers.
    float Fp_Gpr[NUM_FLOAT_GPR]; //the array of general-purpose floating-point registers.
    float Mem[SIZE_MEM]; //the array to represent the memory.
    int PC; //the PC register.
    int cycle;
    string input_filename;
    int Num_FU[NUM_FUNCTION]; //the array holding the number of the 5 kinds of Function units
    int *Num_Res_Stas[NUM_FUNCTION]; //the array holding the number of reservation stations for each FU.
    int *Exe_Cycs[NUM_FUNCTION]; //the array holding the number of execution cycles for each FU.

    /* we have five kinds of FUs, each has several FUs, each FU has several Res
    stas.*/
    struct Reservation_Sta ** Res_Stas[NUM_FUNCTION]; //the three-dimentional reservation stations.
    //bool * FU_Stas[NUM_FUS]; //indicate the status of fus.
    bool mem_busy;// true means there is a ins using the memory port
    bool *div_busy;//true means there is a div ins using the fu
    struct Res_Sta_Id Register_Stas[NUM_FLOAT_GPR]; //the array for register status.
    struct Instruction_Status * Intr_reg; //the instruction waiting for issuing
    deque <struct Instruction> inst; // RJS: deque of instructions
    deque <struct Instruction_Status> Intrs_Stas; //the queue containing instruction status.
    deque <struct Res_Fu_Index> Load_q; //load queue
    deque <struct Res_Fu_Index> Store_q; //store queue
 public:
    TOMASULO(); //constructor.
    ~TOMASULO(); //destructor.

    int init_in_gpr(); //Initialize general-purpose integer registers.
    int init_fp_gpr(); //Initialize general-purpose floating-point registers.
    int init_mem(); //Initialize memory;
    bool init_from_file(const char *); // RJS: Initialize integer registers, fp registers, and memory from file
    void print_stat(); //RJS: Display res sta stat, inst stat, and reg result stat on every cycle
    void print_final_stats(); // RJS: Print the summary
    bool parse_input_assembly(const char*); // RJS: Parse the input file
    int simu_run(); //simulate the algorithm and run on input assembly instructions.
    void print_stat_local();
    void print_mem(); //Display the content of memory.
    void print_in_gpr(); //Display the content of integer registers.
    void print_fp_gpr(); //Display the content of floating point registers.
    void print_ins(ostream & out); //Display the instructions in the queue "inst".
    void check_regsiter(struct Instruction i); //make sure that the register no. is in valid range.
    bool check_Issue(); //check whether an instruction is waiting for issuing,issue an instruction when it is possible
    void Issue_book(int ty,int fu,int in); //bookkeeping for issue
    void Issue_arith(FUs ty,int fu,int in); //bookkeeping for FP operation issue
    bool fp_arith_issue(FUs ty);
    bool load_issue();
    bool store_issue();
    bool check_start_exe(int ty,int fu); //check whether the reservation station can start execution.
    bool check_compl_exe(int ty,int fu,int in); //check whether the reservation station finishs execution.
    bool write_result(int ty,int fu,int in); //write result
    bool Instr_Fetch(int p); //instruction fetch.
    bool cal_address(int ty,int fu,int in); //calculate address for load and store
    bool memory_Access(); //memory access(load or store),every cycle,at most one ins stared
};//end class TOMASULO

#endif 