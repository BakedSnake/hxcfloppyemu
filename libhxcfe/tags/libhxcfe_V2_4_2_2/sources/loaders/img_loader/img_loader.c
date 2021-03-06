/*
//
// Copyright (C) 2006 - 2013 Jean-Fran?ois DEL NERO
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
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//-----------H----H--X----X-----CCCCC----22222----0000-----0000------11----------//
//----------H----H----X-X-----C--------------2---0----0---0----0--1--1-----------//
//---------HHHHHH-----X------C----------22222---0----0---0----0-----1------------//
//--------H----H----X--X----C----------2-------0----0---0----0-----1-------------//
//-------H----H---X-----X---CCCCC-----222222----0000-----0000----1111------------//
//-------------------------------------------------------------------------------//
//----------------------------------------------------- http://hxc2001.free.fr --//
///////////////////////////////////////////////////////////////////////////////////
// File : img_loader.c
// Contains: IMG floppy image loader
//
// Written by:	DEL NERO Jean Francois
//
// Change History (most recent first):
///////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "libhxcfe.h"

#include "floppy_loader.h"
#include "floppy_utils.h"

#include "img_loader.h"

#include "pcimgfileformat.h"

#include "libhxcadaptor.h"

int pc_imggetfloppyconfig(unsigned char * img,unsigned int filesize,unsigned short *numberoftrack,unsigned char *numberofside,unsigned short *numberofsectorpertrack,unsigned char *gap3len,unsigned char *interleave,unsigned short *rpm, unsigned int *bitrate,unsigned short * ifmode)
{
	int i;
	unsigned char * uimg;
	int conffound,numberofsector;

	conffound=0;
	uimg=(unsigned char *)img;

	if(uimg[0x18]<24 && uimg[0x18]>7 && (uimg[0x1A]==1 || uimg[0x1A]==2) && !(filesize&0x1FF) )
	{
		*rpm=300;
		*numberofsectorpertrack=uimg[0x18];
		*numberofside=uimg[0x1A];
		if(*numberofsectorpertrack<=10)
		{
			*gap3len=84;
			*interleave=1;
			*bitrate=250000;
			*ifmode=IBMPC_DD_FLOPPYMODE;
		}
		else
		{
			if(*numberofsectorpertrack<=21)
			{
				*bitrate=500000;
				*gap3len=84;
				*interleave=1;
				*ifmode=IBMPC_HD_FLOPPYMODE;

				if(*numberofsectorpertrack>18)
				{
					*gap3len=14;
					*interleave=2;
				}

				if(*numberofsectorpertrack == 15)
				{
					*rpm=360;
				}

			}
			else
			{
				*bitrate=1000000;
				*gap3len=84;
				*interleave=1;
				*ifmode=IBMPC_ED_FLOPPYMODE;
			}
		}
		numberofsector=uimg[0x13]+(uimg[0x14]*256);
		*numberoftrack=(numberofsector/(*numberofsectorpertrack*(*numberofside)));

	//	if((unsigned int)((*numberofsectorpertrack) * (*numberoftrack) * (*numberofside) *512)==filesize)
		{
			conffound=1;
		}

	}

	if(conffound==0)
	{
		i=0;
		do
		{

			if(pcimgfileformats[i].filesize==filesize)
			{
				*numberoftrack=pcimgfileformats[i].numberoftrack;
				*numberofsectorpertrack=pcimgfileformats[i].sectorpertrack;
				*numberofside=pcimgfileformats[i].numberofside;
				*gap3len=pcimgfileformats[i].gap3len;
				*interleave=pcimgfileformats[i].interleave;
				*rpm=pcimgfileformats[i].RPM;
				*bitrate=pcimgfileformats[i].bitrate;
				*ifmode=pcimgfileformats[i].interface_mode;
				conffound=1;
			}
			i++;

		}while(pcimgfileformats[i].filesize!=0 && conffound==0);
	}
	return conffound;
}


int IMG_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int i,conffound;
	int filesize;

	floppycontext->hxc_printf(MSG_DEBUG,"IMG_libIsValidDiskFile");

	if(hxc_checkfileext(imgfile,"img") || hxc_checkfileext(imgfile,"ima"))
	{
		filesize=hxc_getfilesize(imgfile);
		if(filesize<0)
		{
			floppycontext->hxc_printf(MSG_ERROR,"IMG_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		if(filesize&0x1FF)
		{
			floppycontext->hxc_printf(MSG_DEBUG,"IMG_libIsValidDiskFile : non IMG file - bad file size !");
			return HXCFE_BADFILE;
		}

		i=0;
		conffound=0;
		do
		{
			if((int)pcimgfileformats[i].filesize==filesize)
			{
				conffound=1;
			}
			i++;
		}while(pcimgfileformats[i].filesize!=0 && conffound==0);

		if(!conffound)
		{
			floppycontext->hxc_printf(MSG_DEBUG,"IMG_libIsValidDiskFile : non IMG file - bad file size !");
			return HXCFE_BADFILE;
		}

		floppycontext->hxc_printf(MSG_DEBUG,"IMG_libIsValidDiskFile : IMG file !");
		return HXCFE_VALIDFILE;
	}
	else
	{
		floppycontext->hxc_printf(MSG_DEBUG,"IMG_libIsValidDiskFile : non IMG file !");
		return HXCFE_BADFILE;
	}

	return HXCFE_BADPARAMETER;
}



int IMG_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
{

	FILE * f;
	unsigned int filesize;
	unsigned int i,j;
	unsigned int file_offset;
	unsigned char* trackdata;
	unsigned char gap3len,interleave;
	unsigned short rpm;
	unsigned short sectorsize;
	unsigned char trackformat;
	CYLINDER* currentcylinder;

	floppycontext->hxc_printf(MSG_DEBUG,"IMG_libLoad_DiskFile %s",imgfile);

	f=hxc_fopen(imgfile,"rb");
	if(f==NULL)
	{
		floppycontext->hxc_printf(MSG_ERROR,"Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}

	fseek (f , 0 , SEEK_END);
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET);

	if(filesize!=0)
	{
		sectorsize=512; // IMG file support only 512bytes/sector floppies.
		// read the first sector
		trackdata = (unsigned char*)malloc(sectorsize);
		if(trackdata)
		{
			fread(trackdata,sectorsize,1,f);
			if(pc_imggetfloppyconfig(
				trackdata,
				filesize,
				&floppydisk->floppyNumberOfTrack,
				&floppydisk->floppyNumberOfSide,
				&floppydisk->floppySectorPerTrack,
				&gap3len,
				&interleave,
				&rpm,
				&floppydisk->floppyBitRate,
				&floppydisk->floppyiftype)==1
				)
			{

				free(trackdata);
				floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack);

				floppycontext->hxc_printf(MSG_DEBUG,"rpm %d bitrate:%d track:%d side:%d sector:%d",rpm,floppydisk->floppyBitRate,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack);

				trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);

				trackformat=IBMFORMAT_DD;

				for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
				{
					floppydisk->tracks[j]=allocCylinderEntry(rpm,floppydisk->floppyNumberOfSide);
					currentcylinder=floppydisk->tracks[j];

					for(i=0;i<floppydisk->floppyNumberOfSide;i++)
					{
						file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
							(sectorsize*(floppydisk->floppySectorPerTrack)*i);

						fseek (f , file_offset , SEEK_SET);
						fread(trackdata,sectorsize*floppydisk->floppySectorPerTrack,1,f);

						currentcylinder->sides[i]=tg_generateTrack(trackdata,sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500|REVERTED_INDEX,-2500);
					}
				}

				free(trackdata);

				floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");
			}
			hxc_fclose(f);
			return HXCFE_NOERROR;
		}
		else
		{
			hxc_fclose(f);
			return HXCFE_INTERNALERROR;
		}
	}

	floppycontext->hxc_printf(MSG_ERROR,"file size=%d !?",filesize);
	hxc_fclose(f);
	return HXCFE_BADFILE;
}

int IMG_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="RAW_IMG";
	static const char plug_desc[]="IBM PC IMG Loader";
	static const char plug_ext[]="img";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	IMG_libIsValidDiskFile,
		(LOADDISKFILE)		IMG_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	IMG_libGetPluginInfo
	};

	return libGetPluginInfo(
			floppycontext,
			infotype,
			returnvalue,
			plug_id,
			plug_desc,
			&plug_funcs,
			plug_ext
			);
}

