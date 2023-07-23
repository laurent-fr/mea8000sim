#include <stdio.h>
#include <stdlib.h>

int read_bin(char *filename,unsigned char *buffer,int max_size) {

    FILE *fp;

    fp=fopen(filename,"rb");
    if (fp==NULL) {
        perror("Cannot open file");
        exit(-1);
    }

    // file length
    fseek(fp,0,SEEK_END);
    int length = ftell(fp);
    fseek(fp,0,SEEK_SET);

   if (length>max_size) {
        fprintf(stderr,"file size too big (%d>%d).",length,max_size);
   }

    int n = fread(buffer,1,length,fp);
    fclose(fp);

    return n;
}

void find_sample(unsigned char *buffer,int sample_num,int *start,int *length) {

    int sample_offset = buffer[sample_num*2]*256 + buffer[sample_num*2+1];

    printf("sample_offset %d\n",sample_offset);

    *length = buffer[sample_offset]*256 + buffer[sample_offset+1] - 3;

    *start =  sample_offset + 3;
}