//
//  main.cpp
//  574project1
//
//  Created by tianqing on 11/21/15.
//  Copyright (c) 2015 mdong3. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int sim_resol(unsigned char*iv, unsigned char *key, int byte_index);

unsigned char out;

int main(int argc,char* argv[])
{
    unsigned char key[5];
    memset(key, 0, 5);
    unsigned char counts[256];
    FILE *fd;
    unsigned char tmp_line[143];
    unsigned char plain = 'C';
    size_t read;

    
    for(int byte_index=0; byte_index<5; byte_index++)
    {
        fd = fopen(argv[1],"r");
        if(fd == NULL)
        {
            printf ("Error, the file doesn't exist!\n");
            return 0;
        }
        memset(counts, 0, 256);
//        memset(tmp_line, 0, 142);
        read = fread (tmp_line, sizeof(unsigned char), 143, fd);
//        int packet = 1;
        while(read !=0)
        {
            if((tmp_line[24] == byte_index+3) && (tmp_line[25] == 255))
            {
                int tmp;
                out = plain^tmp_line[27];
                tmp = sim_resol(&tmp_line[24], key, byte_index);
                if((tmp >= 0) && (tmp <=255))
                    counts[tmp]++;
            }
//            memset(tmp_line, 0, 142);
            read = fread(tmp_line, sizeof(unsigned char), 143, fd);
//            packet ++;
        }
//        cout << packet << endl;
        int max=0;
        int max_index=0;
        for(int i=0; i<256; i++)
        {
            if(counts[i] > max)
            {
                max_index=i;
                max = counts[i];
            }
        }
        key[byte_index]=(unsigned char)max_index;
    }
    for (int i = 0; i < 5; i ++) {
        cout << key[i];
    }
    fclose (fd);
    return 0;
}
int sim_resol(unsigned char *iv, unsigned char *key, int byte_index)
{
    unsigned char s[256];
    unsigned char a, s_1, k;
    unsigned char tmp_key[8];
    
    memcpy(tmp_key, iv, 3);
    memcpy(&tmp_key[3], key, 5);
    for (int i = 0; i < 256; i ++)
        s[i] = i;
    int j;
    j= k = 0;
    
    for(int i = 0; i < byte_index + 3; i ++)
    {
        j = (unsigned char)(j+s[i]+tmp_key[k]);
        a = s[i];
        s[i] = s[j];
        s[j] = a;
        if (++k >= 8)
            k = 0;
    }
    for (int i=0; i<256; i++)
        if(s[i] == out)
            s_1 = i;
    if((s[0] != byte_index+ 3) || (s[1] != 0))
        return -1;
    else
        return (s_1-j-s[byte_index+3]);
}