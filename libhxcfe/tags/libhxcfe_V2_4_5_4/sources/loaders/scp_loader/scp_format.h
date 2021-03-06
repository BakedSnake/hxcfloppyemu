/*
//
// Copyright (C) 2006-2014 Jean-Fran?ois DEL NERO
//
// This file is part of the HxCFloppyEmulator library
//
// HxCFloppyEmulator may be used and distributed without restriction provided
// that this copyright statement is not removed from the file and that any
// derivative work contains the original copyright notice and the associated
// disclaimer.
//
// HxCFloppyEmulator is free software; you can redistribute it
// and/or modify  it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// HxCFloppyEmulator is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HxCFloppyEmulator; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
*/

/*
This information is copyright (C) 2012-2014 By Jim Drew.  Permissions is granted
for inclusion with any source code when keeping this copyright notice.

The SuperCard Pro image file format will handle flux level images for any type of disk,
be it 5.25" or 3.5", GCR, MFM, etc.

All longwords are in little endian format.  The words used for the flux length in the
actual flux data are in big endian format... sorry, that's just how it worked out for
the SuperCard Pro hardware.

The header contains the ASCII of "SCP" as the first 3 bytes. If this is not found, then
the file is not ours.

BYTE 0x03 is the version/revision as a byte.  This is encoded as (Version<<4|Revision),
so that 0x39= version 3.9 of the format.  This is the version number of the SCP imaging
software that created this image.

BYTE 0x04 is the disk type and represents the type of disk for the image (see disk types
in the defines).

BYTE 0x05 is the number of revolutions, which is how many revolutions for each track is
contained in the image.

BYTES 0x06 and 0x07 are the start track and end track bytes.  Tracks are numbered 0-165,
which is a maximum of 166 tracks (82 tracks with top/bottom).

BYTE 0x08 is the FLAGS byte.  This byte contains information about how the image was
produced.  The bits are defined as follows:

Bit 0 - INDEX, cleared if the image did not use the index mark for queuing tracks
               set is the image used the index mark to queue tracks

Bit 1 - TPI, cleared if the drive is a 48TPI drive
             set if the drive is a 96TPI drive

Bit 2 - RPM, cleared if the drive is a 300 RPM drive
             set if the drive is a 360 RPM drive

Bit 3 - TYPE, cleared if the image is a read-only type
              set if the image is read/write capable

FLAGS bit 0 is used to determine when the reading of flux data started.  If this bit is
set then the flux data was read immediately after the index pulse was detected.  If
this bit is clear then the flux data was read starting at some random location on the
track.

FLAGS bit 1 is used for determining the type of 5.25" drive was used.  Does not apply
to any 3.5" drives.

FLAGS bit 2 is used to determine the approximate RPM of the drive.  When FLAGS bit 0 is
clear, the index pulse is simulated using either a 166.6667ms (360 RPM) or 200ms (300 RPM)
fake pulse.

FLAGS bit 3 is used to determine if the image is read-only or read/write capable.  Most
images will be read-only (write protected) for emulation usage.  The read/write capable
images contain padded space to allow the track to change size within the image.  Only a
single revolution is needed when the TYPE bit is set.

BYTE 0x09 is the width of the bit cell time.  Normally this is always 16 bits, but if the
value is non-zero, then it represents the number of bits for each bit cell entry.  For
example, if this byte was set to 8, then each bit cell entry would be 8 bits wide, not
the normal 16 bits.  This is for future expansion, and may never be actually used.

BYTES 0x0A and 0x0B are reserved for future use.

BYTES 0x0C-0x0F are the 32 bit checksum of data starting from offset 0x10 through the
end of the image file.  Checksum is standard addition, with a wrap beyond 32 bits
rolling over.

BYTES 0x10-0x13 are the 32 bit offset (from the start of the file) to the 1st track's
data header.  There is an offset entry for every track, with up to 164 tracks supported.
This means that disk images of up to 82 tracks with sides 0/1 are possible.  If no
flux data for a track is present, then the entry will contain a longword of zeros
(0x00000000).  The 1st track header will start at offset 0x000002A8, but could
technically start anywhere in the file.  Since offsets are used, track data does not
have to be in any order, so you could append track data to the file and change the
header to point to the appended data.  For simplicity it is recommended that you follow
the normal image format shown below, with all tracks in sequential order.


TRACK DATA HEADER (TDH) INFO:
----------------------------

As stated, each offset entry points to a track data header.  That header contains
information about the flux data for that track.  To help recover corrupt files,
each entry has its own header starting with "TRK" as the first 3 bytes of the header.

BYTES 0x00-0x02 ASCII 'TRK'

BYTE 0x03 contains the track number (0-165).  For single sided disks, tracks 0-41 (48TPI)
or 0-83 (96TPI) are used.  For double-sided disks, the actual track number is this value
divided by 2.  The remainder (0 or 1) is the head.  0 represents the bottom head and
1 represents the top head.  For example, 0x0B would be 11/2 = track 5, head 1 (top).
Likewise, 0x50 would be 80/2 = track 40, head 0 (bottom).

Starting at BYTE 0x04 are sets of 3 longwords, one set for each revolution that is
stored.  Typically a maximum of 5 sets of 3 entries are stored using the SuperCard
Pro's imaging program, but this can vary from image to image.  BYTE 0x05 of the
main file header to determines the number of revolutions that are stored for each track.
The 3 longwords are described below:

BYTES 0x04-0x07 contain the duration of the 1st revolution.  This is the time from index
pulse to index pulse.  This will be either ~360RPMs or ~300 RPMs depending the drive.
It is important to have this exact time as it is necessary when writing an image back to a
real disk.  The index time is a value in nanoseconds/25ns for one revolution.  Multiply
the value by 25 to get the exact value.

BYTES 0x08-0x0B contain the length of the track in bit cells (flux transitions).  If a
16 bit width is used (selected above), then this value *2 will be the total number of
bytes of data that is used for that track (16 bit = 2 bytes per word).

BYTES 0x0C-0x0F contains the offset from the start of the Track Data Header (BYTE 0x00,
which is the "T" in "TRK) to the flux data.

If there were more revolutions, there would be 3 more longwords stored for each
additional revolution.. example:

BYTES 0x10-0x1B would be the 2nd entry (duration2, length2, offset2).
BYTES 0x1C-0x27 would be the 3rd entry (duration3, length3, offset3).
BYTES 0x28-0x33 would be the 4th entry (duration4, length4, offset4).
BYTES 0x34-0x3F would be the 5th entry (duration5, length5, offset5).

Note that image types with FLAGS bit set for READ/WRITE capability only uses a single
revolution, but the space allocated for the flux data will be the maximum necessary to
store the worst case requirement.  The actual flux data will be inside of this space
and the length will be accurate.  This allows the track to expand and contract inside of
the space without overlapping and corrupting the file.

Flux data for each revolution is always stored sequentially (like the disk is spinning
multiple times) with SuperCard Pro's imager..  No break or skew occurs.  However, it is
possible that flux data could be located anywhere in the file because everything is
offset based.  For simplicity, please try to keep the revolutions sequential.
 

After the last byte of flux data there will be a timestamp.  This timestamp is an
ASCII string, ie: 1/05/2014 5:15:21 PM



; ------------------------------------------------------------------
; SCP IMAGE FILE FORMAT
; ------------------------------------------------------------------
;
; 0000              'SCP' (ASCII CHARS)
; 0003              VERSION (nibbles major/minor)
; 0004              DISK TYPE
;                   UPPER 4 BITS ARE USED TO DEFINE A DISK CLASS (MANUFACTURER)
;                   LOWER 4 BITS ARE USED TO DEFINE A DISK SUB-CLASS (MACHINE)
;
;                   MANUFACTURER BIT DEFINITIONS:
;                   0000 = COMMODORE
;                   0001 = ATARI
;                   0010 = APPLE
;                   0011 = PC
;                   0100 = TANDY
;                   0101 = TEXAS INSTRUMENTS
;                   0110 = ROLAND
;
;					SEE DISK TYPE BIT DEFINITIONS BELOW
;
; 0005              NUMBER OF REVOLUTIONS (1-5)
; 0006              START TRACK (0-165)
; 0007              END TRACK (0-165)
; 0008              FLAGS BITS (0=INDEX, 1=TPI, 2=RPM, 3=TYPE)
; 0009              BIT CELL ENCODING (0=16 BITS, >0=NUMBER OF BITS USED)
; 000A-B            RESERVED (2 BYTES)
; 000C-F            32 BIT CHECKSUM OF DATA FROM 0x10-EOF
; 0010              OFFSET TO 1st TRACK DATA HEADER (4 bytes of 0 if track is skipped)
; 0014              OFFSET TO 2nd TRACK DATA HEADER (4 bytes of 0 if track is skipped)
; 0018              OFFSET TO 3rd TRACK DATA HEADER (4 bytes of 0 if track is skipped)
; ....
; 02A4              OFFSET TO 165th TRACK DATA HEADER (4 bytes of 0 if track is skipped)
; 02A8              START OF 1st TRACK DATA HEADER
;
; ....              END OF TRACK DATA
; ????              TIMESTAMP (AS ASCII STRING - ie. 7/17/2013 12:45:49 PM)

; ## FILE FORMAT DEFINES ##

IFF_ID = 0x00                      ; "SCP" (ASCII CHARS)
IFF_VER = 0x03                     ; version (nibbles major/minor)
IFF_DISKTYPE = 0x04                ; disk type (0=CBM, 1=AMIGA, 2=APPLE II, 3=ATARI ST, 4=ATARI 800, 5=MAC 800, 6=360K/720K, 7=1.44MB)
IFF_NUMREVS = 0x05                 ; number of revolutions (2=default)
IFF_START = 0x06                   ; start track (0-165)
IFF_END = 0x07                     ; end track (0-165)
IFF_FLAGS = 0x08                   ; FLAGS bits (0=INDEX, 1=TPI, 2=RPM, 3=TYPE - see defines below)
IFF_ENCODING = 0x09                ; BIT CELL ENCODING (0=16 BITS, >0=NUMBER OF BITS USED)
IFF_RSRVED = 0x0A                  ; 2 bytes of reserved space
IFF_CHECKSUM = 0x0C                ; 32 bit checksum of data added together starting at 0x0010 through EOF
IFF_THDOFFSET = 0x10               ; first track data header offset
IFF_THDSTART = 0x2A8               ; start of first Track Data Header

; FLAGS BIT DEFINES (BIT NUMBER)

FB_INDEX = 0x00                    ; clear = no index reference, set = flux data starts at index
FB_TPI = 0x01                      ; clear = drive is 48TPI, set = drive is 96TPI (only applies to 5.25" drives!)
FB_RPM = 0x02                      ; clear = drive is 300 RPM drive, set = drive is 360 RPM drive
FB_TYPE = 0x03                     ; clear = image is read-only, set = image is read/write capable

; MANUFACTURERS                      7654 3210
man_CBM = 0x00                     ; 0000 xxxx
man_Atari = 0x10                   ; 0001 xxxx
man_Apple = 0x20                   ; 0010 xxxx
man_PC = 0x40                      ; 0011 xxxx
man_Tandy = 0x21                   ; 0100 xxxx
man_TI = 0x40                      ; 0101 xxxx
man_Roland = 0x60                  ; 0110 xxxx

; DISK TYPE BIT DEFINITIONS
;
; CBM DISK TYPES
disk_C64 = 0x00                    ; xxxx 0000
disk_Amiga = 0x04                  ; xxxx 0100

; ATARI DISK TYPES
disk_AtariFMSS = 0x00              ; xxxx 0000
disk_AtariFMDS = 0x01              ; xxxx 0001
disk_AtariFMEx = 0x02              ; xxxx 0010
disk_AtariSTSS = 0x04              ; xxxx 0100
disk_AtariSTDS = 0x05              ; xxxx 0101

; APPLE DISK TYPES
disk_AppleII = 0x00                ; xxxx 0000
disk_AppleIIPro = 0x01             ; xxxx 0001
disk_Apple400K = 0x04              ; xxxx 0100
disk_Apple800K = 0x05              ; xxxx 0101
disk_Apple144 = 0x06               ; xxxx 0110

; PC DISK TYPES
disk_PC360K = 0x00                 ; xxxx 0000
disk_PC720K = 0x01                 ; xxxx 0001
disk_PC12M = 0x02                  ; xxxx 0010
disk_PC144M = 0x03                 ; xxxx 0011

; TANDY DISK TYPES
disk_TRS80SS = 0x00                ; xxxx 0000
disk_TRS80DS = 0x01                ; xxxx 0001

; TI DISK TYPES
disk_TI994A = 0x00                 ; xxxx 0000

; ROLAND DISK TYPES
disk_D20 = 0x00                    ; xxxx 0000


; ------------------------------------------------------------------
; TRACK DATA HEADER FORMAT
; ------------------------------------------------------------------
;
; 0000              'TRK' (ASCII CHARS)             - 3 chars
; 0003              TRACK NUMBER                    - 1 byte
; ....              START OF TABLE OF ENTRIES FOR EACH REVOLUTION
; 0004              INDEX TIME (1st REVOLUTION)     - 4 bytes
; 0008              TRACK LENGTH (1st REVOLUTION)   - 4 bytes
; 000C              DATA OFFSET (1st REVOLUTION)    - 4 bytes (offset is from start of Track Data Header)
; ....              ADDITIONAL ENTRIES FOR EACH REVOLUTION (IF AVAILABLE)...
; 0010              INDEX TIME (2nd REVOLUTION)     - 4 bytes
; 0014              TRACK LENGTH (2nd REVOLUTION)   - 4 bytes
; 0018              DATA OFFSET (2nd REVOLUTION)    - 4 bytes
; 001C              INDEX TIME (3rd REVOLUTION)     - 4 bytes
; 0020              TRACK LENGTH (3rd REVOLUTION)   - 4 bytes
; 0024              DATA OFFSET (3rd REVOLUTION)    - 4 bytes
; 0028              INDEX TIME (4th REVOLUTION)     - 4 bytes
; 002C              TRACK LENGTH (4th REVOLUTION)   - 4 bytes
; 0030              DATA OFFSET (4th REVOLUTION)    - 4 bytes
; 0034              INDEX TIME (5th REVOLUTION)     - 4 bytes
; 0038              TRACK LENGTH (5th REVOLUTION)   - 4 bytes
; 003C              DATA OFFSET (5th REVOLUTION)    - 4 bytes
; .... etc. etc.
;
;
; INDEX TIME = 32 BIT VALUE, TIME IN NANOSECONDS/25ns FOR ONE REVOLUTION
;
; i.e. 0x7A1200 = 8000000, 8000000*25 = 200000000 = 200.00000ms
;
; TRACK DATA = 16 BIT VALUE, TIME IN NANOSECONDS/25ns FOR ONE BIT CELL TIME
;
; i.e. 0x00DA = 218, 218*25 = 5450ns = 5.450us

; ## TRACK DATA HEADER DEFINES ##

TDH_ID = 0x00                      ; "TRK" (ASCII CHARS)
TDH_TRACKNUM = 0x03                ; track number
TDH_TABLESTART = 0x04              ; table of entries (3 longwords per revolution stored)
TDH_DURATION = 0x4                 ; duration of track, from index pulse to index pulse (1st revolution)
TDH_LENGTH = 0x08                  ; length of track (1st revolution)
TDH_OFFSET = 0x0C                  ; offset to flux data from start of TDH (1st revolution)
*/

// Header Flags
#define INDEXMARK     0x01
#define DISK_96TPI    0x02
#define DISK_360RPM   0x04
#define DISK_RWENABLE 0x08

#pragma pack(1)

typedef struct scp_header_
{
	unsigned char sign[3];				// "SCP"
	unsigned char version;				// Version<<4|Revision
	unsigned char disk_type;
	unsigned char number_of_revolution;
	unsigned char start_track;
	unsigned char end_track;
	unsigned char flags;
	unsigned char RFU_0;
	unsigned char RFU_1;
	unsigned char RFU_2;
	unsigned long file_data_checksum;
}scp_header;

typedef struct scp_index_pos_
{
	unsigned long index_time;
	unsigned long track_lenght;
	unsigned long track_offset;
}scp_index_pos;

typedef struct scp_track_header_
{
	unsigned char trk_sign[3];				// "TRK"
	unsigned char track_number;
	scp_index_pos index_position[5];
	unsigned long track_data_checksum;
}scp_track_header;

#pragma pack()
