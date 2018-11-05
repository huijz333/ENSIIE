/*#########################################################################
# $Software: busiac
# $section : rs232-mux (driver)
#       $Id: usb-ctl.c 330 2015-06-05 09:27:16Z ia $ 
#  $HeadURL: svn://lunix120.ensiie.fr/ia/cours/archi/projet/busiac/c/usb-ctl.c $
# $Author  : Ivan Auge (Email: auge@ensiie.fr)
# #########################################################################
# 
# This file is part of the BUSIAC software: Copyright (C) 2010 by I. Auge.
#
# This program is free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License  as published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
#
# BUSIAC software is distributed in the hope that it will be  useful, but
# WITHOUT  ANY  WARRANTY ;  without  even  the  implied  warranty  of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# #######################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "driver.h"
#include "usb.h"

#define  HISPTORY_FILE "usb-test"
#include "pretty_input.h"

#define ADDR 0xF0

#define ISPWRAPPER_CMD_WrCmd      0
#define ISPWRAPPER_CMD_WrDat16    1
#define ISPWRAPPER_CMD_RdDat16    2
#define ISPWRAPPER_CMD_Write16    3
#define ISPWRAPPER_CMD_Read16     4
#define ISPWRAPPER_CMD_Ready      5
#define ISPWRAPPER_CMD_EnableInt  6

#define ISPWRAPPER_ADDR_CtlUsb    8
#define ISPWRAPPER_ADDR_DtEpUsb   9

// --- ISP Wrapper
static void wtest();
static void rtest();

static Tmdm_param isp_handler __automatic_start = {
    .name          = "isp wrapper",
    .addrprocessor = ADDR,
    .samewindow    = 0,
    .rdwr          = rtest,
    .wronly        = wtest,
    .wronly_geom   = "100x10+0+0",
    .rdwr_geom     = "100x15+0+160",
};

// --- USB Contoler
static void usbctl();

static Tmdm_param usbctl_handler __automatic_start = {
    .name          = "usb controler",
    .addrprocessor = ADDR + ISPWRAPPER_ADDR_CtlUsb,
    .samewindow    = 0,
    .rdwr          = usbctl,
    .wronly        = 0,
    .rdwr_geom     = "100x30+570+0",
};

// ------------------------------------------------
#define BIT(n) (1<<(n))

int ADDR_SRC  = ADDR;
int ADDR_DEST = ADDR;

static print_time(const char* mess)
{
    struct timespec tm;
    long long date;
    
    clock_gettime(CLOCK_REALTIME,&tm);
    date = tm.tv_sec*1000000LL + tm.tv_nsec/1000LL;

    printf("%16Ld us : %s\n",date,mess);
}

static void write_data00_to_reg(int reg)
{
    Tmessage mess;
    mess.ctl  = 0;
    mess.asrc = ADDR_SRC;
    mess.ades = ADDR_DEST + ISPWRAPPER_CMD_WrCmd;
    mess.uval = (reg<<16)&0xFF0000;
    mess_send(&mess);
}

static void write_data16(unsigned int value)
{
    Tmessage mess;
    mess.ctl  = 0;
    mess.asrc = ADDR_SRC;
    mess.ades = ADDR_DEST + ISPWRAPPER_CMD_WrDat16;
    mess.uval = value&0xFFFF ;
    mess_send(&mess);
}

static void write_data16_to_reg(int reg, unsigned int value)
{
    Tmessage mess;
    mess.ctl  = 0;
    mess.asrc = ADDR_SRC;
    mess.ades = ADDR_DEST + ISPWRAPPER_CMD_Write16;
    mess.uval = ((reg<<16)&0xFF0000) | (value&0xFFFF) ;
    mess_send(&mess);
}

static void write_data32_to_reg(int reg, unsigned int value)
{
    write_data16_to_reg(reg,value);
    write_data16(value>>16);
}


static void read_data16()
{
    Tmessage mess;
    mess.ctl  = 0;
    mess.asrc = ADDR_SRC;
    mess.ades = ADDR_DEST + ISPWRAPPER_CMD_RdDat16;
    mess.uval = 0;
    mess_send(&mess);
}

static int nbbytes_alreadysent=0;
static void reset()
{
    int i;
    nbbytes_alreadysent=0;
    // IntEnable + SoftConnect
    write_data16_to_reg(ISPreg_Wr_DcMode, BIT(3)+BIT(0) );
    // NoLazyClock + ClkRun + 48MH
    write_data16_to_reg(ISPreg_Wr_DcHardwareConfiguration, BIT(13) ); // +BIT(12) );
    //  CtlOut: FIFOEN=1 EPDIR=0=Out DBLBUF=0 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
    write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 0, BIT(7)+3 );
    //  CtlIn: FIFOEN=1 EPDIR=1=In   DBLBUF=0 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
    write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 1, BIT(7)+BIT(6)+3 );
    //  Ep1Out: FIFOEN=1 EPDIR=0 DBLBUF=1 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
    write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 2, BIT(7)+BIT(5)+3 );
    //  Ep2In:  FIFOEN=1 EPDIR=1 DBLBUF=1 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
    write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 3, BIT(7)+BIT(6)+BIT(5)+3 );
    for (i=4 ; i<=15 ; i++) {
      //  EpN:  disabled
      write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + i, 0 );
    }
    // enable interupt for CtlOut, Ep1 and busreset
    write_data32_to_reg(ISPreg_Wr_DcInterruptEnable, 0x00000501 );
}

static void wtest_read(int*adest, int*reg, int*value)
{
    char* cmd;
    char rw[100]; int arg1,arg2;
    char prompt[100];
    sprintf(prompt,"entrer r|w|? [reg] [val] > ");

    while (1) {
        cmd=rl_gets(prompt);
        if ( cmd==0 ) {
            printf("\n");
            printf("EOF \n");
            while (1) pause();
        }
        int status=sscanf(cmd,"%s%x%x",rw,&arg1,&arg2);
fprintf(stderr,"st=%d ;%s;%x;%x;\n",status,rw,arg1,arg2);
        switch ( status ) {
          case 1:
            if ( rw[0]=='w' ) {
                fprintf(stderr,"write require a reg or a data\n");
            } else if ( rw[0]=='i' ) {
                reset();
                continue;
            } else if ( rw[0]=='e' ) {
                *adest  = ADDR+ISPWRAPPER_CMD_EnableInt;
                *reg   = 0;
                *value = 0;
                return;
            } else if ( rw[0]=='d' && rw[1]=='a' ) {
                write_data00_to_reg(ISPreg_DcAcknowledgeSetup);
                write_data00_to_reg(ISPreg_DcClearBuffer | 0);
            } else if ( rw[0]=='d' && rw[1]=='r' ) {
                write_data00_to_reg(ISPreg_Rd_EndPointStatus + 2);
                write_data00_to_reg(ISPreg_Rd_Buffer + 2);
                read_data16();
                read_data16();
                read_data16();
                write_data00_to_reg(ISPreg_DcClearBuffer + 2);
            } else if ( rw[0]=='d' && rw[1]=='w' && rw[2]=='3') {
                write_data00_to_reg(ISPreg_Wr_Buffer + 3);
                write_data16(4);
                write_data16(0x3132);
                write_data16(0x3334);
                write_data00_to_reg(ISPreg_DcValidate + 3);
            } else if ( rw[0]=='d' && rw[1]=='w' && rw[2]=='4') {
                write_data00_to_reg(ISPreg_Wr_Buffer + 4);
                write_data16(4);
                write_data16(0x4142);
                write_data16(0x4344);
                write_data00_to_reg(ISPreg_DcValidate + 4);
                //write_data00_to_reg(ISPreg_DcClearBuffer + 2);
            } else if ( rw[0]=='d' && rw[1]=='w' && rw[2]=='i') {
                write_data00_to_reg(ISPreg_Wr_Buffer + 3);
                write_data16(4);
                write_data16(0x3132);
                //write_data00_to_reg(ISPreg_Wr_Buffer + 3);
                write_data16(0x3334);
                write_data00_to_reg(ISPreg_DcValidate + 3);
                write_data00_to_reg(ISPreg_Wr_Buffer + 4);
                write_data16(4);
                write_data16(0x4142);
                write_data16(0x4344);
                write_data00_to_reg(ISPreg_DcValidate + 4);
            } else if ( rw[0]=='d' && rw[1]=='s' ) {
                write_data00_to_reg(ISPreg_Rd_DcEndpointStatusImage + 0); read_data16();
                write_data00_to_reg(ISPreg_Rd_DcEndpointStatusImage + 1); read_data16();
                write_data00_to_reg(ISPreg_Rd_DcEndpointStatusImage + 2); read_data16();
                write_data00_to_reg(ISPreg_Rd_DcEndpointStatusImage + 3); read_data16();
            } else if ( rw[0]=='r' && rw[1]==0) {
                *adest  = ADDR+ISPWRAPPER_CMD_RdDat16;
                *reg   = 0;
                *value = 0;
                return;
            } else if ( rw[0]=='r' &&  rw[1]=='e' &&  rw[2]=='a') {
                *adest  = ADDR+ISPWRAPPER_CMD_Ready;
                *reg   = 0;
                *value = 0;
                return;
            } else
                fprintf(stderr,"invalid command\n");
            break;
          case 2:
            if ( rw[0]=='w' ) {
                *adest  = ADDR+ISPWRAPPER_CMD_WrDat16;
                *reg   = 0;
                *value = arg1;
                return;
            } else if ( rw[0]=='c' ) {
                *adest  = ADDR+ISPWRAPPER_CMD_WrCmd;
                *reg   = arg1;
                *value = 0;
                return;
            } else if ( rw[0]=='r' ) {
                *adest   = ADDR+ISPWRAPPER_CMD_Read16;
                *reg   = arg1;
                *value = 0;
                return;
            } else
                fprintf(stderr,"invalid command\n");
            break;
          case 3:
            if ( rw[0]=='w' ) {
                *adest  = ADDR+ISPWRAPPER_CMD_Write16;
                *reg   = arg1;
                *value = arg2;
                return;
            } else if ( rw[0]=='r' ) {
                fprintf(stderr,"to many arguments for read (0 or 1)\n");
            } else
                fprintf(stderr,"invalid command\n");
            break;
        }
    }
}

static void wtest()
{
    //reset();
int cnt=0;
    Tmessage mess;
    int adest,reg,value;
    while (1) {
        wtest_read(&adest,&reg,&value);
        mess.ctl  = 0;
        mess.asrc = ADDR;
        mess.ades = adest;
        mess.uval = (reg<<16) | (value&0xFFFF) ;
        mess_send(&mess);
    }
}

static void rtest()
{
    Tmessage mess;
    while (1) {
        mess = mess_receive();
        printf("usb: from=%3d ; value=0x%06x\n",mess.asrc,mess.uval);
    }
}

// -----------------------------------------------------------
// --- USB Device controler

typedef struct _Tusb_request {
    unsigned char  bmRequestType;
    unsigned char  bRequest;
    unsigned short wValue;
    unsigned short wIndex;
    unsigned short wLenght;
    unsigned char  data[100];
} Tusb_request;

typedef struct _Tusb_devDescr {
    unsigned char  bLenght;
    unsigned char  bDescriptorType;
    unsigned short bcdUSB;
    unsigned char  bDeviceClass;
    unsigned char  bDeviceSubClass;
    unsigned char  bDeviceProtocol;
    unsigned char  bMaxPacketSize;
    unsigned short idVendor;
    unsigned short idProduct;
    unsigned short bcdProduct;
    unsigned char  iManufacturer;
    unsigned char  iProduct;
    unsigned char  iSerialNumber;
    unsigned char  bNumConfiguration;
} Tusb_devDescr;

  
typedef struct _Tusb_confDescr {
    unsigned char  bLength;            // Size of this descriptor in bytes
    unsigned char  bDescriptorType;    // constant = 2
    unsigned short wTotalLength;       // Total number of bytes in this descriptor
                                       // and all the following descriptors.
    unsigned char bNumInterfaces;      // Number of interfaces of this configuration
    unsigned char bConfigurationValue; // Value used by Set Configuration to select
                                       // this configuration
    unsigned char iConfiguration;      // Index of string descriptor describing
                                       // configuration - set to 0 if no string
    unsigned char bmAttributes;        // D7: Must be set to 1
                                       // D6: Self-powered
                                       // D5: Remote Wakeup
                                       // D4...D0: Set to 0
    unsigned char bMaxPower;           // Maximum current drawn by device in
                                       // this configuration.
                                       // In units of 2mA. So 50 means 100 mA.
} Tusb_confDescr;

typedef struct _Tusb_intfDescr {
    unsigned char  bLength;           // Size of this descriptor in bytes
    unsigned char  bDescriptorType;   // constant = 4
    unsigned char  bInterfaceNumber;  // Number identifying this interface.
                                      // Zero-based value.
    unsigned char  bAlternateSetting; // Value used to select this alternate
                                      // setting for this interface.
    unsigned char  bNumEndpoints;     // Number of endpoints used by this
                                      // interface.
                                      // Doesn't include control endpoint 0.
        
    unsigned char bInterfaceClass;    // Class code assigned by USB-IF
                                      //   00h is a reserved value
                                      //   FFh means vendor-defined class
                                      //   Any other value must be a class code
    unsigned char bInterfaceSubClass; // SubClass Code assigned by USB-IF
    unsigned char bInterfaceProtocol; // Protocol Code assigned by USB-IF
    unsigned char iInterface;         // Index of string descriptor,
                                      // set to 0 if no string    
} Tusb_intfDescr;

typedef struct _Tusb_endpDescr {
    unsigned char  bLength;          // Size of this descriptor in bytes
    unsigned char  bDescriptorType;  // Constant = 5
    unsigned char  bEndpointAddress; // The address of this endpoint within
                                     // the device.
                                     // D7: Direction, 0 = OUT, 1 = IN
                                     // D6-D4: Set to 0
                                     // D3-D0: Endpoint number

    unsigned char  bmAttributes;     // D1:0 Transfer Type
                                     //   00 = Control
                                     //   01 = Isochronous
                                     //   10 = Bulk
                                     //   11 = Interrupt
                                     // The following only apply to isochronous
                                     // endpoints. Else set to 0.
                                     // D3:2 Synchronisation Type
                                     //   00 = No Synchronisation
                                     //   01 = Asynchronous
                                     //   10 = Adaptive
                                     //   11 = Synchronous
                                     // D5:4 Usage Type
                                     //   00 = Data endpoint
                                     //   01 = Feedback endpoint
                                     //   10 = Implicit feedback Data endpoint
                                     //   11 = Reserved
                                     // D7:6 Reserved set to 0
    unsigned short wMaxPacketSize;   // Maximum packet size this endpoint can
                                     // send or receive when this configuration
                                     // is selected
    unsigned char  bInterval;        // Interval for polling endpoint for data
                                     // transfers. Expressed in frames (ms) for
                                     // low/full speed or microframes (125us)
                                     // for high speed. 
} Tusb_endpDescr;

unsigned char usb_address;

static int swap_u16(void* addr)
{
    unsigned short* s = (unsigned short*)addr;
    *s = (((*s)>>8)&0x00FF) | (((*s)<<8)&0xFF00);
}

static int usb_DeviceDesc(unsigned char**d)
{
    static Tusb_devDescr des;
    des.bLenght           = 18; 
    des.bDescriptorType   = 1;
    des.bcdUSB            = 0x0200;
    des.bDeviceClass      = 0;
    des.bDeviceSubClass   = 0;
    des.bDeviceProtocol   = 0;
    des.bMaxPacketSize    = 64; 
    des.idVendor          = 0x04cc;
    des.idProduct         = 0x1a62;
    des.bcdProduct        = 0x0100;
    des.iManufacturer     = 0;
    des.iProduct          = 0;
    des.iSerialNumber     = 0;
    des.bNumConfiguration = 0x01;

    //swap_u16(&des.bcdUSB);
    //swap_u16(&des.idVendor);
    //swap_u16(&des.idProduct);
    //swap_u16(&des.bcdProduct);

    *d=(unsigned char*)&des;
    return des.bLenght;
}

static unsigned char *confBuffer=0;
static int usb_ConfigurationDesc(unsigned char**d)
{

    Tusb_confDescr conf;
    Tusb_intfDescr intf;
    Tusb_endpDescr ep2,ep3,ep4;

    conf.bLength             = 9;
    conf.bDescriptorType     = 2;
    conf.wTotalLength        = 9+9+3*7; // IVG 9+9+2*7;
    conf.bNumInterfaces      = 1;
    conf.bConfigurationValue = 1;
    conf.iConfiguration      = 0;
    conf.bmAttributes        = 0xC0; // Self-powered
    conf.bMaxPower           = 0;

    intf.bLength             = 9;
    intf.bDescriptorType     = 4;
    intf.bInterfaceNumber    = 0;
    intf.bAlternateSetting   = 0;
    intf.bNumEndpoints       = 3; // IVG 2;
    intf.bInterfaceClass     = 0xFF;
    intf.bInterfaceSubClass  = 0xFF;
    intf.bInterfaceProtocol  = 0xFF;
    intf.iInterface          = 0;
                            
    ep2.bLength              = 7;
    ep2.bDescriptorType      = 5;
    ep2.bEndpointAddress     = 0x01; // IN ep1
    ep2.bmAttributes         = 2;    // bulk
    ep2.wMaxPacketSize       = 64;
    ep2.bInterval            = 0;

    ep3.bLength              = 7;
    ep3.bDescriptorType      = 5;
    ep3.bEndpointAddress     = 0x82; // OUT ep2
    ep3.bmAttributes         = 2;    // bulk
    ep3.wMaxPacketSize       = 64;
    ep3.bInterval            = 0;

    ep4.bLength              = 7;
    ep4.bDescriptorType      = 5;
    ep4.bEndpointAddress     = 0x83; // OUT ep3
    ep4.bmAttributes         = 2;    // bulk
    ep4.wMaxPacketSize       = 64;
    ep4.bInterval            = 0;

//printf("AAAAAAAAAAAAAAAAAAAAAAa:0:%d:%p\n",conf.wTotalLength*2,confBuffer); sleep(1);
    confBuffer= realloc(confBuffer,conf.wTotalLength*2);
//printf("AAAAAAAAAAAAAAAAAAAAAAa:1:%d:%p\n",conf.wTotalLength*2,confBuffer); sleep(1);
    *d = confBuffer;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:2\n"); sleep(1);

    int len=0;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:3:%d/%d len=%d\n",conf.bLength,conf.wTotalLength*2,len); sleep(1);
    memcpy(confBuffer+len ,&conf,  conf.bLength); len += conf.bLength;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:4:%d/%d len=%d\n",intf.bLength,conf.wTotalLength*2,len); sleep(1);
    memcpy(confBuffer+len ,&intf,  intf.bLength); len += intf.bLength;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:5:%d/%d len=%d\n",ep2.bLength,conf.wTotalLength*2,len); sleep(1);
    memcpy(confBuffer+len ,&ep2,   ep2.bLength ); len += ep2.bLength;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:6:%d/%d len=%d\n",ep3.bLength,conf.wTotalLength*2,len); sleep(1);
    memcpy(confBuffer+len ,&ep3,   ep3.bLength ); len += ep3.bLength;
//printf("AAAAAAAAAAAAAAAAAAAAAAa:7: len=%d\n",len); sleep(1);
    memcpy(confBuffer+len ,&ep4,   ep4.bLength ); len += ep4.bLength;

    return len;
}

static Tusb_request* usb_char2request(unsigned char*d)
{
    unsigned char tmp;
    //tmp=d[0]; d[0]=d[1]; d[1]=tmp;
    //swap_u16(d);
    return (Tusb_request*)d;
}

static Tusb_request* usb_putDesc(unsigned char*d, int des_size, int max_size)
{
    int i;
    int size = des_size >= max_size ? max_size : des_size;
    unsigned short* s=(unsigned short*)d;
    write_data00_to_reg(ISPreg_Wr_Buffer + 1);
    write_data16(size);
    for (i=0 ; i<size ; i+=2) {
        write_data16(*s++);
    }
    write_data00_to_reg(ISPreg_DcValidate + 1);
    // write_data00_to_reg(ISPreg_DcAcknowledgeSetup + 1);
}

static int usbctl_getmess(unsigned char *d)
{
    int i,size;
    Tmessage mess;

    mess = mess_receive();
    //print_time("usb_getmess_begin");
    if ( mess.uval==0x800000 )
        return -1; // reset
    size = (mess.uval>>8)&0x7FF;
    for (i=0 ; i<size ; i+=2) {
        mess = mess_receive();
        // little endian: LL LH HL HH
        // little endian:  0  1  2  3
        // data:          00  L  H 00 
        // -- d[i+2] = (mess.uval>> 0) & 0xFF; // -- iggnored
        d[i+0] = (mess.uval>> 8) & 0xFF;
        d[i+1] = (mess.uval>>16) & 0xFF;
        // -- printf("receive:%2d/%2d: %02x %02x (%02x) (%06x)\n",
        // --     i+2,size, d[i],d[i+1],d[i+2],mess.uval&0xFFFFFF);
    }
    //print_time("usb_getmess_end");
    return size;
}

void usbctl()
{
    int i,messcnt;
    unsigned char d[0x800];
    Tusb_request* request;
    unsigned char* packet;
    int len;

    usb_ConfigurationDesc(&packet);
    usb_ConfigurationDesc(&packet);

    for ( messcnt=1 ; 1 ; messcnt+=1 ) {
        int status= usbctl_getmess(d);
        printf("mess:%d:",messcnt);
        if (status==-1) {
            //reset();
            //print_time("usb_ctl_reset_sent");
            printf(" reset\n");
            continue;
        }
        if ( status==0 ) {
            printf(" empty\n");
            continue;
        }
        for (i=0 ; i<status ; i++)
            printf(" %02x",d[i]);
        printf("\n");
#if 0
static int cnt=0;
if (cnt++==0) {
ADDR_SRC=100;
ADDR_DEST=100;
usb_putDesc(d,8);
ADDR_SRC=ADDR;
ADDR_DEST=ADDR;
}
#endif
        request=(Tusb_request*)d;
        // printf("mess:%d: t=%02x r=%02x v=%04x i=%04x l=%04x\n",
        //     messcnt, request->bmRequestType, request->bRequest,
        //     request->wValue,request->wIndex, request->wLenght);
        request = usb_char2request(d);
        printf("mess:%d: t=%02x r=%02x v=%04x i=%04x l=%04x",
            messcnt, request->bmRequestType, request->bRequest,
            request->wValue,request->wIndex, request->wLenght);
#if 0
        if ( request->wLenght>0 ) {
            printf(" data=");
            for (i=0 ; i<10 && i<request->wLenght ; i++)
                printf(" %02x",request->data[i]);
            if ( i!=request->wLenght )
                printf(" ...");
        }
#endif
        printf("\n");

        if ( request->bmRequestType==0x80 && request->bRequest==0x00 ) {
            printf("mess:%d: GET_STATUS device (done)\n",messcnt);
            unsigned short status = 0x0001; // self powered
            usb_putDesc((void*)&status,2,2);
        } else if ( request->bmRequestType==0x00 && request->bRequest==0x01 ) {
            printf("mess:%d: CLEAR_FEATURE\n",messcnt);
        } else if ( request->bmRequestType==0x00 && request->bRequest==0x03 ) {
            printf("mess:%d: SET_FEATURE\n",messcnt);
        } else if ( request->bmRequestType==0x00 && request->bRequest==0x05 ) {
            printf("mess:%d: SET_ADDRESS\n",messcnt);
            usb_address= 0x80 | (request->wValue&0x7F);
            write_data16_to_reg(ISPreg_Wr_DcAddress,usb_address);
            usb_putDesc(packet,0,0);
            //print_time("usb_ctl_set_addr");
            printf("mess:%d: %d address is set\n",messcnt,usb_address&0x7F);
        } else if ( request->bmRequestType==0x80 && request->bRequest==0x06 ) {
            int type = (request->wValue>>8)&0xff;
            printf("mess:%d: GET_DESCRIPTOR type=%d",messcnt,type);
            if (type==1) {
#if 0
                len = usb_DeviceDesc(&packet);
                int nbbytes_tosend=len-nbbytes_alreadysent;
                if (nbbytes_tosend > 0) {
                    if ( nbbytes_tosend > request->wLenght )
                        nbbytes_tosend = request->wLenght;
                }
                usb_putDesc(packet+nbbytes_alreadysent,nbbytes_tosend);
                nbbytes_alreadysent += nbbytes_tosend;
                if ( nbbytes_alreadysent == len )
                    nbbytes_alreadysent =0;
#endif
                printf(" = device (done)\n");
           } else if (type==2) {
//printf("\nBBBBBBBBBBBBBBBBBBBBBBb:0\n");
                len = usb_ConfigurationDesc(&packet);
//printf("BBBBBBBBBBBBBBBBBBBBBBb:0\n");
#if 1
                usb_putDesc(packet,len,request->wLenght);
#else
                int nbbytes_tosend=len-nbbytes_alreadysent;
                if (nbbytes_tosend > 0) {
                    if ( nbbytes_tosend > request->wLenght )
                        nbbytes_tosend = request->wLenght;
                }
                usb_putDesc(packet+nbbytes_alreadysent,nbbytes_tosend);
                nbbytes_alreadysent += nbbytes_tosend;
                if ( nbbytes_alreadysent == len )
                    nbbytes_alreadysent =0;
//printf("BBBBBBBBBBBBBBBBBBBBBBb:0\n");
#endif
                printf(" = configuration (done)\n");
            } else if (type==6) {
                write_data00_to_reg(0x41); // stall Conrol In EP
                printf(" = Qualifier Descriptor ? (stalled: no other config)\n");
            } else if (type==10) {
                write_data00_to_reg(0x41); // stall Conrol In EP
                printf(" = Qualifier Descriptor  (stalled: no other config)\n");
            } else {
                printf(" (ignored)\n");
            }
            //print_time("usb_ctl_get_devDesc");
        } else if ( request->bmRequestType==0x00 && request->bRequest==0x07 ) {
            printf("mess:%d: SET_DESCRIPTOR\n",messcnt);
        } else if ( request->bmRequestType==0x80 && request->bRequest==0x08 ) {
            printf("mess:%d: GET_CONFIGURARION\n",messcnt);
        } else if ( request->bmRequestType==0x00 && request->bRequest==0x09 ) {
            //  CtlOut: FIFOEN=1 EPDIR=0=Out DBLBUF=0 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
            write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 0, BIT(7)+3 );
            //  CtlIn: FIFOEN=1 EPDIR=1=In   DBLBUF=0 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
            write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 1, BIT(7)+BIT(6)+3 );
            //  Ep1Out: FIFOEN=1 EPDIR=0 DBLBUF=1 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
            write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 2, BIT(7)+BIT(5)+3 );
            //  Ep2In:  FIFOEN=1 EPDIR=1 DBLBUF=1 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
            write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 3, BIT(7)+BIT(6)+BIT(5)+3 );
            //  Ep3In:  FIFOEN=1 EPDIR=1 DBLBUF=1 FFOISO=0 (bulk)  FFOSZ=3 (64 bytes)
            write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + 4, BIT(7)+BIT(6)+BIT(5)+3 );
            for (i=5 ; i<=15 ; i++) {
               //  EpN:  disabled
               write_data16_to_reg(ISPreg_Wr_DcEndpointConfiguration + i, 0 );
            }
            usb_putDesc(packet,0,0);
            printf("mess:%d: SET_CONFIGURARION config %d accepte par l'host\n",messcnt,request->wValue);
        } else {
            printf("mess:%d: UNKNOWN REQUEST\n",messcnt);
        }
    }

}
