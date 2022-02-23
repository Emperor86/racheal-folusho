/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Dec 22 12:48:37 2003
 */
/* Compiler settings for D:\NetSET 5.1\AnimAlg\AnimAlg.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IDijkstra = {0x5DDF46FF,0x095C,0x4A62,{0x9B,0xBF,0xB2,0x0A,0x29,0x66,0xDC,0x8A}};


const IID LIBID_ANIMALGLib = {0xE5266849,0xC5D9,0x4D8C,{0xAA,0x1C,0xB9,0xC6,0x5D,0x7B,0xE1,0xA6}};


const IID DIID__IDijkstraEvents = {0xD5A6541C,0xCEC7,0x4D54,{0x8C,0xF6,0xCD,0xAE,0x91,0x18,0x30,0xD5}};


const CLSID CLSID_Dijkstra = {0x6F02AF52,0xEC0B,0x4188,{0xA4,0x52,0x3F,0x1E,0x39,0xF1,0xB3,0x47}};


#ifdef __cplusplus
}
#endif

