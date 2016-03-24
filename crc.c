#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/crc32.h>
#include "rxe.h"
#include "rxe_loc.h"

int init_module(void)
{
	u32 crc;
        unsigned int length;
        unsigned int grh_offset;
        unsigned int bth_offset;
        u8 tmp[RXE_GRH_BYTES + RXE_BTH_BYTES];
	u8 hdr[RXE_GRH_BYTES + RXE_BTH_BYTES + RXE_RETH_BYTES];
	u8 pl[12];
	int i;

	//LRH
        crc = 0xdebb20e3;

        length = RXE_BTH_BYTES;
        grh_offset = 0;
        bth_offset = 0;

	//GRH
	*(hdr +  0) = 0x60;
	*(hdr +  1) = 0x00;
	*(hdr +  2) = 0x00;
	*(hdr +  3) = 0x00;
	*(hdr +  4) = 0x00;
	*(hdr +  5) = 0x1c;
	*(hdr +  6) = 0x1b;
	*(hdr +  7) = 0x10;
	*(hdr +  8) = 0xfe;
	*(hdr +  9) = 0x80;
	*(hdr + 10) = 0x00;
	*(hdr + 11) = 0x00;
	*(hdr + 12) = 0x00;
	*(hdr + 13) = 0x00;
	*(hdr + 14) = 0x00;
	*(hdr + 15) = 0x00;
	*(hdr + 16) = 0x02;
	*(hdr + 17) = 0x90;
	*(hdr + 18) = 0xfa;
	*(hdr + 19) = 0xff;
	*(hdr + 20) = 0xfe;
	*(hdr + 21) = 0xb8;
	*(hdr + 22) = 0xf5;
	*(hdr + 23) = 0x30;
	*(hdr + 24) = 0xfe;
	*(hdr + 25) = 0x80;
	*(hdr + 26) = 0x00;
	*(hdr + 27) = 0x00;
	*(hdr + 28) = 0x00;
	*(hdr + 29) = 0x00;
	*(hdr + 30) = 0x00;
	*(hdr + 31) = 0x00;
	*(hdr + 32) = 0xf6;
	*(hdr + 33) = 0x52;
	*(hdr + 34) = 0x14;
	*(hdr + 35) = 0xff;
	*(hdr + 36) = 0xfe;
	*(hdr + 37) = 0x94;
	*(hdr + 38) = 0x99;
	*(hdr + 39) = 0x60;
	//BTH
	*(hdr + 40) = 0x04;
	*(hdr + 41) = 0x20;
	*(hdr + 42) = 0xff;
	*(hdr + 43) = 0xff;
	*(hdr + 44) = 0x00;
	*(hdr + 45) = 0x04;
	*(hdr + 46) = 0x03;
	*(hdr + 47) = 0xb4;
	*(hdr + 48) = 0x80;
	*(hdr + 49) = 0x85;
	*(hdr + 50) = 0x3f;
	*(hdr + 51) = 0x5a;
	//RETH
	/**
	*(hdr + 52) = 0x01;
	*(hdr + 53) = 0x71;
	*(hdr + 54) = 0x0A;
	*(hdr + 55) = 0x1C;
	*(hdr + 56) = 0x01;
	*(hdr + 57) = 0x5D;
	*(hdr + 58) = 0x40;
	*(hdr + 59) = 0x02;
	*(hdr + 60) = 0x38;
	*(hdr + 61) = 0xF2;
	*(hdr + 62) = 0x7A;
	*(hdr + 63) = 0x05;
	*(hdr + 64) = 0x00;
	*(hdr + 65) = 0x00;
	*(hdr + 66) = 0x00;
	*(hdr + 67) = 0x0E;
	**/
	length += RXE_GRH_BYTES;
      	bth_offset += RXE_GRH_BYTES;
                              
	memcpy(tmp, hdr, length);

	printk("GRH\n");
	for(i = 0; i < 40; i++)
	{
		printk("%02x ", *(tmp + i));
	}
	printk("\n");
	printk("BTH\n");
	for(i = 40; i < 52; i++)
	{
		printk("%02x ", *(tmp + i));
	}
	printk("\n");

	tmp[grh_offset + 0] |= 0x0f;
	tmp[grh_offset + 1] = 0xff;
	tmp[grh_offset + 2] = 0xff;
	tmp[grh_offset + 3] = 0xff;
	tmp[grh_offset + 7] = 0xff;

	tmp[bth_offset + 4] = 0xff;
	
	printk("GRH masked\n");
        for(i = 0; i < 40; i++)
        {
                printk("%02x ", *(tmp + i));
        }
        printk("\n");
	printk("BTH masked\n");
	for(i = 40; i < 52; i++)
	{
		printk("%02x ", *(tmp + i));
	}
	printk("\n");

	crc = crc32_le(crc, tmp + grh_offset, length);
	printk("crc #1 %x\n", crc);
	/**
	printk("RETH\n");
	for(i = length; i < 68; i++)
	{
		printk("%02x ",*(hdr + i));
	}
	printk("\n");

	crc = crc32_le(crc, hdr + length, 16);
	printk("crc #2 %x\n",crc);
	**/
	*(pl +  0) = 0x7b;
	*(pl +  1) = 0x7b;
	*(pl +  2) = 0x7b;
	*(pl +  3) = 0x7b;
	*(pl +  4) = 0x7b;
	*(pl +  5) = 0x7b;
	*(pl +  6) = 0x7b;
	*(pl +  7) = 0x7b;
	*(pl +  8) = 0x7b;
	*(pl +  9) = 0x7b;
	*(pl + 10) = 0x00;
	*(pl + 11) = 0x00;

	crc = crc32_le(crc, pl, 12);
	crc = ~crc;
	printk("crc cpu order %#x\n", crc);
	printk("crc network order%#x\n", htonl(crc));

	return 0;
}

void cleanup_module(void)
{
	
}
