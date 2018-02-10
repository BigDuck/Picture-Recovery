#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef unsigned char  BYTE;



int main(int argc, char* argv[]){
    BYTE cur[512];
    int number = 0;
    FILE* fout = fopen("buff.temp", "w");
    FILE* fin;
    
    if(fout == NULL){
        fprintf(stderr, "Could not open file\n");
        return 2;
    }
    if(argc!=2){
        printf("Usage: ./recover file\n");
        return 1;
    }
    fin = fopen(argv[1], "r");
    if(fin==NULL){
        fprintf(stderr, "Could not open file\n");
        return 2;
    }

    while(fread(cur, sizeof(BYTE), 512, fin)!= EOF){
        //If demarcates start of jpeg(0xff, 0xd8, 0xff, 0xe?)
        //close the current fout. open another fout, check 
        //if null, and write into it.
        if(cur[0]==0xff && cur[1]==0xd8 && cur[2]==0xff &&
            (cur[3] & 0xe0)==0xe0){
            char fileName[20];
            sprintf(fileName, "Image%02d.jpg", number);
            fclose(fout);
            fout = fopen(fileName, "w");
            if(fout == NULL){
                fprintf(stderr, "Point to null error\n");
                return 2;
            }
            number++;
        }
        fwrite(cur, sizeof(BYTE), 512, fout);
    }
    
    remove("buff.temp");
    
    return 0;
}