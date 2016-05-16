#ifndef _DATA_PROCESS_H__
#define _DATA_PROCESS_H__

#define PackHead1 0xb4
#define PackHead2 0x09


int pack_data(unsigned char data_after[] , char data1[] , char data2[] , char data3[] , int num_data);
int unpack_data(char data_before[] , char data_split[] , int num_split);

#endif
