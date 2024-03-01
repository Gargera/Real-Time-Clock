#include "DIO.h"

void seven_seg_vinit(unsigned char portname)
{
	DIO_set_port_direction(portname,0xff);
}

void seven_seg_write(unsigned char portname,unsigned char number,unsigned char com)
{
    unsigned char arr[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x47,0x7f,0x6f};

    if(com == 0) DIO_write_port(portname,arr[number]);
	else DIO_write_port(portname,~arr[number]);
}