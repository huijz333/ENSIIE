#ifndef FILE_USB_H
#define FILE_USB_H

// ISP registers
#define ISPreg_Wr_DcEndpointConfiguration      0x0020  // (113) 20=OudCtl 21=InCtl 22=EP1 23=EP2 ...
#define ISPreg_Rd_DcEndpointConfiguration      0x0030  // (113) 30=OudCtl 31=InCtl 32=EP1 33=EP2 ...
#define ISPreg_Wr_DcAddress                    0x00B6  // (114)
#define ISPreg_Rd_DcAddress                    0x00B7  // (114)
#define ISPreg_Wr_DcMode                       0x00B8  // (114)
#define ISPreg_Rd_DcMode                       0x00B9  // (114)
#define ISPreg_Wr_DcHardwareConfiguration      0x00BA  // (115)
#define ISPreg_Rd_DcHardwareConfiguration      0x00BB  // (115)
#define ISPreg_Wr_DcInterruptEnable            0x00C2  // (116)
#define ISPreg_Rd_DcInterruptEnable            0x00C3  // (116)
#define ISPreg_Wr_DcDMAConfiguration           0x00F0
#define ISPreg_Rd_DcDMAConfiguration           0x00F1
#define ISPreg_Wr_DcDMACounter                 0x00F2
#define ISPreg_Rd_DcDMACounter                 0x00F3
#define ISPreg_Wr_Reset                        0x00F6
#define ISPreg_Wr_Buffer                       0x0000 // (119) 00=OudCtl 01=InCtl 02=EP1 03=EP2 ...
#define ISPreg_Rd_Buffer                       0x0010 // (119) 10=OudCtl 11=InCtl 12=EP1 13=EP2 ...
#define ISPreg_EPSTS_STALL                     0x0040 // (121) 40=OudCtl 41=InCtl 42=EP1 43=EP2 ...
#define ISPreg_Rd_EndPointStatus               0x0050 // (120) 50=OudCtl 51=InCtl 52=EP1 53=EP2 ...
#define ISPreg_DcValidate                      0x0060 // (122) 60=OudCtl 61=InCtl 62=EP1 63=EP2 ...
#define ISPreg_DcClearBuffer                   0x0070 // (122) 70=OudCtl 71=InCtl 72=EP1 73=EP2 ..
#define ISPreg_EPSTS_UNSTALL                   0x0080 // (121) 80=OudCtl 81=InCtl 82=EP1 83=EP2 ...
#define ISPreg_Rd_DcEndpointStatusImage        0x00D0 // (122) D0=OudCtl D1=InCtl D2=EP1 D3=EP2 ...
#define ISPreg_DcAcknowledgeSetup              0x00F4 // (123) 
#define ISPreg_Rd_ErrorCode                    0x00A0 // (123) A0=OudCtl A1=InCtl A2=EP1 A3=EP2 ... 
#define ISPreg_UnlockDevice                    0x00B0 // (124)
#define ISPreg_Wr_DcScratchRegister            0x00B2
#define ISPreg_Rd_DcScratchRegister            0x00B3
#define ISPreg_Rd_DcFrameNumber                0x00B4
#define ISPreg_Rd_DcChipID                     0x00B5
#define ISPreg_Rd_DcInterrupt                  0x00C0 // (127)

#if 0
--From hal.h
constant EPINDEX4EP0_CONTROL_OUT : std_logic_vector(3 downto 0) := x"0";
constant EPINDEX4EP0_CONTROL_IN : std_logic_vector(3 downto 0) := x"1";
constant EPINDEX4EP01 : std_logic_vector(3 downto 0) := x"2";
constant EPINDEX4EP02 : std_logic_vector(3 downto 0) := x"3";
constant EPINDEX4EP03 : std_logic_vector(3 downto 0) := x"4";
constant EPINDEX4EP04 : std_logic_vector(3 downto 0) := x"5";
constant EPINDEX4EP05 : std_logic_vector(3 downto 0) := x"6";
constant EPINDEX4EP06 : std_logic_vector(3 downto 0) := x"7";
constant EPINDEX4EP07 : std_logic_vector(3 downto 0) := x"8";
constant EPINDEX4EP08 : std_logic_vector(3 downto 0) := x"9";
constant EPINDEX4EP09 : std_logic_vector(3 downto 0) := x"A";
constant EPINDEX4EP0A : std_logic_vector(3 downto 0) := x"B";
constant EPINDEX4EP0B : std_logic_vector(3 downto 0) := x"C";
constant EPINDEX4EP0C : std_logic_vector(3 downto 0) := x"D";
constant EPINDEX4EP0D : std_logic_vector(3 downto 0) := x"E";
constant EPINDEX4EP0E : std_logic_vector(3 downto 0) := x"F";


constant DEVADDR_MASK : std_logic_vector(15 downto 0) := x"007F";
constant DEVADDR_EN : std_logic_vector(15 downto 0) := x"0080";

constant EP_DISABLE : std_logic_vector(15 downto 0) := "--------0-------";
constant EPCNFG_FIFO_EN : std_logic_vector(15 downto 0) := x"0080";
constant EPCNFG_IN_EN : std_logic_vector(15 downto 0) := x"0040";
constant EPCNFG_DBLBUF_EN : std_logic_vector(15 downto 0) := x"0020";
constant EPCNFG_ISO_EN : std_logic_vector(15 downto 0) := x"0010";
constant EPCNFG_ISOSZ_MASK : std_logic_vector(15 downto 0) := x"000F";
constant EPCNFG_NONISOSZ_MASK : std_logic_vector(15 downto 0) := x"0007";
constant EPCNFG_RFB_EN : std_logic_vector(15 downto 0) := x"0008";
constant EPCNFG_NONISOSZ_8 : std_logic_vector(15 downto 0) := x"0000";
constant EPCNFG_NONISOSZ_16 : std_logic_vector(15 downto 0) := x"0001";
constant EPCNFG_NONISOSZ_32 : std_logic_vector(15 downto 0) := x"0002";
constant EPCNFG_NONISOSZ_64 : std_logic_vector(15 downto 0) := x"0003";
constant EPCNFG_ISOSZ_16 : std_logic_vector(15 downto 0) := x"0000";
constant EPCNFG_ISOSZ_32 : std_logic_vector(15 downto 0) := x"0001";
constant EPCNFG_ISOSZ_48 : std_logic_vector(15 downto 0) := x"0002";
constant EPCNFG_ISOSZ_64 : std_logic_vector(15 downto 0) := x"0003";
constant EPCNFG_ISOSZ_96 : std_logic_vector(15 downto 0) := x"0004";
constant EPCNFG_ISOSZ_128 : std_logic_vector(15 downto 0) := x"0005";
constant EPCNFG_ISOSZ_160 : std_logic_vector(15 downto 0) := x"0006";
constant EPCNFG_ISOSZ_192 : std_logic_vector(15 downto 0) := x"0007";
constant EPCNFG_ISOSZ_256 : std_logic_vector(15 downto 0) := x"0008";
constant EPCNFG_ISOSZ_320 : std_logic_vector(15 downto 0) := x"0009";
constant EPCNFG_ISOSZ_384 : std_logic_vector(15 downto 0) := x"000A";
constant EPCNFG_ISOSZ_512 : std_logic_vector(15 downto 0) := x"000B";
constant EPCNFG_ISOSZ_640 : std_logic_vector(15 downto 0) := x"000C";
constant EPCNFG_ISOSZ_768 : std_logic_vector(15 downto 0) := x"000D";
constant EPCNFG_ISOSZ_896 : std_logic_vector(15 downto 0) := x"000E";
constant EPCNFG_ISOSZ_1023 : std_logic_vector(15 downto 0) := x"000F";

--constant EPSTS_STALL : std_logic_vector(15 downto 0) := x"0080";
constant EPSTS_DBF1 : std_logic_vector(15 downto 0) := x"0040";
constant EPSTS_DBF0 : std_logic_vector(15 downto 0) := x"0020";
constant EPSTS_OVWR : std_logic_vector(15 downto 0) := x"0008";
constant EPSTS_SETUP : std_logic_vector(15 downto 0) := x"0004";
constant EPSTS_DBFIDX : std_logic_vector(15 downto 0) := x"0002";

constant ERRCODE_RXTX : std_logic_vector(15 downto 0) := x"0001";
constant ERRCODE_DPID : std_logic_vector(15 downto 0) := x"0040";
constant ERRCODE_OVRLP : std_logic_vector(15 downto 0) := x"0080";
constant ERRCODE_MASK : std_logic_vector(15 downto 0) := x"001E";
constant ERRCODE_OK : std_logic_vector(15 downto 0) := x"0000";
constant ERRCODE_PIDERR : std_logic_vector(15 downto 0) := x"0002";
constant ERRCODE_PIDNEW : std_logic_vector(15 downto 0) := x"0004";
constant ERRCODE_PKTTYP : std_logic_vector(15 downto 0) := x"0006";
constant ERRCODE_TKCRC : std_logic_vector(15 downto 0) := x"0008";
constant ERRCODE_DATACRC : std_logic_vector(15 downto 0) := x"000A";
constant ERRCODE_TIMEOUT : std_logic_vector(15 downto 0) := x"000C";
constant ERRCODE_RSRV0 : std_logic_vector(15 downto 0) := x"000E";
constant ERRCODE_EOP : std_logic_vector(15 downto 0) := x"0010";
constant ERRCODE_NAK : std_logic_vector(15 downto 0) := x"0012";
constant ERRCODE_STALL : std_logic_vector(15 downto 0) := x"0014";
constant ERRCODE_OVRFL : std_logic_vector(15 downto 0) := x"0016";
constant ERRCODE_RSRV1 : std_logic_vector(15 downto 0) := x"0018";
constant ERRCODE_BITSTUFF : std_logic_vector(15 downto 0) := x"001A";
constant ERRCODE_RSRV2 : std_logic_vector(15 downto 0) := x"001C";
constant ERRCODE_DATAPID : std_logic_vector(15 downto 0) := x"001E";

constant LOCK_IOEN : std_logic_vector(15 downto 0) := x"0001";
constant MODE_SOFTCONNECT : std_logic_vector(15 downto 0) := x"0001";
constant MODE_DBG : std_logic_vector(15 downto 0) := x"0004";
constant MODE_INT_EN : std_logic_vector(15 downto 0) := x"0008";
constant MODE_SUSPND : std_logic_vector(15 downto 0) := x"0020";

constant DEVCNFG_INTPOL : std_logic_vector(15 downto 0) := x"0001";
constant DEVCNFG_INTEDGE : std_logic_vector(15 downto 0) := x"0002";
constant DEVCNFG_PWROFF : std_logic_vector(15 downto 0) := x"0004";
constant DEVCNFG_WAKEUPBY_CS : std_logic_vector(15 downto 0) := x"0008";
constant DEVCNFG_EOTPOL : std_logic_vector(15 downto 0) := x"0010";
constant DEVCNFG_DMAACKPOL : std_logic_vector(15 downto 0) := x"0020";
constant DEVCNFG_DMARQPOL : std_logic_vector(15 downto 0) := x"0040";
constant DEVCNFG_DMAACKONLY : std_logic_vector(15 downto 0) := x"0080";
constant DEVCNFG_CLOCKDIV_MASK : std_logic_vector(15 downto 0) := x"0F00";
constant DEVCNFG_CLOCKRUNNING : std_logic_vector(15 downto 0) := x"1000";
constant DEVCNFG_NOLAZYCLOCK : std_logic_vector(15 downto 0) := x"2000";
constant DEVCNFG_EXPULLUP : std_logic_vector(15 downto 0) := x"4000";


constant INTSRC_BUSRESET : std_logic_vector(15 downto 0) := x"0001";
constant INTSRC_RESUME : std_logic_vector(15 downto 0) := x"0002";
constant INTSRC_SUSPEND : std_logic_vector(15 downto 0) := x"0004";
constant INTSRC_EOT : std_logic_vector(15 downto 0) := x"0008";
constant INTSRC_SOF : std_logic_vector(15 downto 0) := x"0010";
constant INTSRC_PSEUDO_SOF : std_logic_vector(15 downto 0) := x"0020";
constant INTSRC_SHORT_PACKET : std_logic_vector(15 downto 0) := x"0040";
constant INTSRC_EP0OUT : std_logic_vector(15 downto 0) := x"0100";
constant INTSRC_EP0IN : std_logic_vector(15 downto 0) := x"0200";
constant INTSRC_EP01 : std_logic_vector(15 downto 0) := x"0400";
constant INTSRC_EP02 : std_logic_vector(15 downto 0) := x"0800";
constant INTSRC_EP03 : std_logic_vector(15 downto 0) := x"1000";
constant INTSRC_EP04 : std_logic_vector(15 downto 0) := x"2000";
constant INTSRC_EP05 : std_logic_vector(15 downto 0) := x"4000";
constant INTSRC_EP06 : std_logic_vector(15 downto 0) := x"8000";
#endif

#endif /* FILE_USB_H */
