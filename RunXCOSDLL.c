
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <windows.h>
#include "machine.h" 
#include "scicos.h" 
#include "scicos_malloc.h" 
#include "scicos_free.h" 
#include "scicos_block4.h"
#include "RunXCOSDLL.h"

typedef int  (*dllfunc)(scicos_block *block, int flag);
static dllfunc TestBlock;
static scicos_block parent_block_TestBlock;

static boolean init_flg = FALSE;

int init(double *input_data,int input_size,double *output_data,int output_size)
{ 
    #define def_arraysize 100

    static double  z[100];
    
    static scicos_block children_block_TestBlock[100];

    // memalocでやりたいがめんどくさいので適当に
    static double   array_temp_output[def_arraysize];
    static double   rpar[100];
    static int      ipar[100];

    static double   *array_pinput[def_arraysize];
    static double   *array_poutput[def_arraysize];
    static double   *array_temp_poutput[def_arraysize];
    static double   *p_rpar = &rpar[0];
    static int      *p_ipar = &ipar[0];

    if(readconstdata("TestBlock_c.sci",&rpar[0],&ipar[0]) != 0){
        return -1;
    }
    
    memset(&z[0],0x00,sizeof(z));
    
    for(int i=0;i<input_size;i++){
        array_pinput[i]  = &input_data[i];
    }

    for(int i=0;i<output_size;i++){
        array_poutput[i] = &output_data[i];
    }
    
    for(int i=0;i<def_arraysize;i++){
        array_temp_output[i] = 0;
        array_temp_poutput[i]   = &array_temp_output[i];
    }

    parent_block_TestBlock.z        = &z[0];
    parent_block_TestBlock.inptr    = array_pinput;
    parent_block_TestBlock.outptr   = array_poutput;
    parent_block_TestBlock.ozptr    = array_temp_poutput;
    parent_block_TestBlock.rpar     = p_rpar;
    parent_block_TestBlock.ipar     = p_ipar;
    parent_block_TestBlock.work     = &children_block_TestBlock[0];

    parent_block_TestBlock.nevprt   = 1;

    // DLL読み込み
    HMODULE dll = LoadLibrary("libTestBlock.dll");
	if (dll == NULL)
	{
		printf("Failer:Load dll.\n");
		return 0;
	}

    // DLLから関数を読み込み
	TestBlock =(dllfunc)GetProcAddress(dll, "TestBlock");
	if (TestBlock == NULL)
	{
		printf("Failer:Load function.\n");
		return 0;
	}
    init_flg = FALSE;
}

void operate(void)
{
    if(init_flg == FALSE){
        init_flg = TRUE;
        TestBlock(&parent_block_TestBlock,4);
    }else{
        for(int j = 1;j<10;j++){
            parent_block_TestBlock.nevprt = j;
            TestBlock(&parent_block_TestBlock,2);
            TestBlock(&parent_block_TestBlock,1);
        }
    }
}

// 適当に作った読み込み処理
int readconstdata(char fname[],double *rpar,int *ipar) {
    FILE *fp; // FILE型構造体
    char tmp[1000];
    memset(rpar,0,sizeof(rpar));
    memset(ipar,0,sizeof(ipar));
    int cnt = 0;
    char *str_rpar = "    rpar = ";
    char *str_ipar = "    ipar = ";
    errno_t err; // errno_t型(int型)

    char output[5001]={0};

    fp = fopen(fname, "r"); // ファイルを開く。失敗するとエラーコードを返す。
    if(fp != NULL) {
        while(fgets(tmp,sizeof(tmp),fp)!=NULL){
            if(strncmp(tmp,str_rpar,sizeof(str_rpar))==0){
                char *start = "[";
                char *end   = "]";
                char *s;
                char *e;
                char *searchS, *searchE;
                if (( s = strstr(tmp,start) ) != NULL) {
                    searchS = (s+strlen(start));

                    //見つかったら終了タグで検索
                    if ((e = strstr(tmp, end)) != NULL) {

                    searchE = e;

                    //文字列の抜き出し
                    if ( searchE > searchS )
                        memcpy(output, searchS, searchE-searchS);
                        char *ptr = strtok(output, ";");
                        cnt = 0;
                        rpar[cnt++] = atof(ptr);
                        while(ptr != NULL) {
                            // strtok関数により変更されたNULLのポインタが先頭
                            ptr = strtok(NULL, ";");
                            
                            // ptrがNULLの場合エラーが発生するので対処
                            if(ptr != NULL) {
                                rpar[cnt++] = atof(ptr);
                            }
                        }
                    }
                }
            }

            if(strncmp(tmp,str_ipar,sizeof(str_ipar))==0){
                char *start = "[";
                char *end   = "]";
                char *s;
                char *e;
                char *searchS, *searchE;
                if (( s = strstr(tmp,start) ) != NULL) {
                    searchS = (s+strlen(start));

                    //見つかったら終了タグで検索
                    if ((e = strstr(tmp, end)) != NULL) {

                    searchE = e;

                    //文字列の抜き出し
                    if ( searchE > searchS )
                        memcpy(output, searchS, searchE-searchS);
                        char *ptr = strtok(output, ";");
                        cnt = 0;
                        ipar[cnt++] = atof(ptr);
                        while(ptr != NULL) {
                            // strtok関数により変更されたNULLのポインタが先頭
                            ptr = strtok(NULL, ";");
                            
                            // ptrがNULLの場合エラーが発生するので対処
                            if(ptr != NULL) {
                                ipar[cnt++] = atoi(ptr);
                            }
                        }
                    }
                }
            }
        }
    }else{
        return -1;
    }
    
    fclose(fp); // ファイルを閉じる
    
    return 0;
}

