#include <stdio.h>
#include <stdlib.h>
#include "RunXCOSDLL.h"

int main(void)
{
    double output[]={0,0,0,0,0};
    double input[]={100,0,0,0,0};

    init(input,100,output,100);
    for(int i =0;i<30;i++){
        operate();
    }
}