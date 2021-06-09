
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "machine.h" 
#include "scicos.h" 
#include "scicos_malloc.h" 
#include "scicos_free.h" 
#include "scicos_block4.h"

#define def_arraysize 10

int  TestBlock_sim(double tf,double dt,double h, int solver);
int  TestBlock(scicos_block *block, int flag);

extern scicos_block block_TestBlock[1];

scicos_block parent_block_TestBlock;
scicos_block children_block_TestBlock[100];


int __real_main(int argc, char *argv[]);

int __wrap_main(int argc, char *argv[])
{

    TestBlock_sim(0,0.1,0.001,3);
    
    double z[100];
    for(int i=0;i<100;i++){
        z[i]=0;
    }
    double array_input[def_arraysize];
    double array_output[def_arraysize];
    double array_temp_output[def_arraysize];

    double  *array_pinput[def_arraysize];
    double  *array_poutput[def_arraysize];
    double  *array_temp_poutput[def_arraysize];
    double  *p_rpar = NULL;
    int     *p_ipar = NULL;

    for(int i=0;i<def_arraysize;i++){
        array_input[i] = 1;
        array_output[i] = 0;
        array_temp_output[i] = 0;

        array_pinput[i]         = &array_input[i];
        array_poutput[i]        = &array_output[i];
        array_temp_poutput[i]   = &array_temp_output[i];
    }

    for(int i=0;i<100;i++){
        if(block_TestBlock[i].z == NULL){
            break;
        }
        if(p_rpar == NULL){
            if(block_TestBlock[i].rpar != NULL){
                p_rpar = block_TestBlock[i].rpar;
            }
        }
        if(p_ipar == NULL){
            if(block_TestBlock[i].ipar != NULL){
                p_ipar = block_TestBlock[i].ipar;
            }
        }
    }

    parent_block_TestBlock.z        = &z[0];
    parent_block_TestBlock.inptr    = array_pinput;
    parent_block_TestBlock.outptr   = array_poutput;
    parent_block_TestBlock.ozptr    = array_temp_poutput;
    parent_block_TestBlock.rpar     = p_rpar;
    parent_block_TestBlock.ipar     = p_ipar;
    parent_block_TestBlock.work     = &children_block_TestBlock[0];

    parent_block_TestBlock.nevprt   = 1;

    TestBlock(&parent_block_TestBlock,4);
    for(int i = 0;i<10;i++){
        for(int j = 1;j<10;j++){
            parent_block_TestBlock.nevprt = j;
            TestBlock(&parent_block_TestBlock,1);
            TestBlock(&parent_block_TestBlock,2);
        }
    }
}
