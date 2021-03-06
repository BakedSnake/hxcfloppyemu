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
// File : camputerslynxldf_loader.c
// Contains: Camputers Lynx floppy image loader
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

#include "camputerslynxldf_loader.h"

#include "libhxcadaptor.h"

int CAMPUTERSLYNX_libIsValidDiskFile(HXCFLOPPYEMULATOR* floppycontext,char * imgfile)
{
	int filesize;

	floppycontext->hxc_printf(MSG_DEBUG,"CAMPUTERSLYNX_libIsValidDiskFile");

	if( hxc_checkfileext(imgfile,"ldf") )
	{

		filesize=hxc_getfilesize(imgfile);

		if(filesize<0) 
		{
			floppycontext->hxc_printf(MSG_ERROR,"CAMPUTERSLYNX_libIsValidDiskFile : Cannot open %s !",imgfile);
			return HXCFE_ACCESSERROR;
		}

		if(filesize&0x1FF)
		{
			floppycontext->hxc_printf(MSG_DEBUG,"CAMPUTERSLYNX_libIsValidDiskFile : non LDF file - bad file size !");
			return HXCFE_BADFILE;
		}

		floppycontext->hxc_printf(MSG_DEBUG,"CAMPUTERSLYNX_libIsValidDiskFile : LDF file !");
		return HXCFE_VALIDFILE;
	}
	else
	{
		floppycontext->hxc_printf(MSG_DEBUG,"CAMPUTERSLYNX_libIsValidDiskFile : non LDF file !");
		return HXCFE_BADFILE;
	}

	return HXCFE_BADPARAMETER;
}



int CAMPUTERSLYNX_libLoad_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppydisk,char * imgfile,void * parameters)
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
	
	floppycontext->hxc_printf(MSG_DEBUG,"CAMPUTERSLYNX_libLoad_DiskFile %s",imgfile);
	
	f=hxc_fopen(imgfile,"rb");
	if(f==NULL) 
	{
		floppycontext->hxc_printf(MSG_ERROR,"CAMPUTERSLYNX_libLoad_DiskFile : Cannot open %s !",imgfile);
		return HXCFE_ACCESSERROR;
	}
	
	fseek (f , 0 , SEEK_END); 
	filesize=ftell(f);
	fseek (f , 0 , SEEK_SET); 
	
	if(filesize!=0)
	{		
		sectorsize=512; 
		rpm=300;

		// read the first sector
		switch(filesize)
		{
			case 40*1*10*512:
				floppydisk->floppyNumberOfTrack=40;
				floppydisk->floppyNumberOfSide=1;
				floppydisk->floppySectorPerTrack=10;
				floppydisk->floppyBitRate=250000;
				rpm=300;
				break;

			case 80*1*10*512:
				floppydisk->floppyNumberOfTrack=80;
				floppydisk->floppyNumberOfSide=1;
				floppydisk->floppySectorPerTrack=10;
				floppydisk->floppyBitRate=250000;
				break;

			case 80*2*10*512:
				floppydisk->floppyNumberOfTrack=80;
				floppydisk->floppyNumberOfSide=2;
				floppydisk->floppySectorPerTrack=10;
				floppydisk->floppyBitRate=250000;
				break;

			default:
				floppydisk->floppyNumberOfTrack=40;
				floppydisk->floppyNumberOfSide=1;
				floppydisk->floppySectorPerTrack=10;
				floppydisk->floppyBitRate=250000;
				break;
		}

		gap3len=255;
		interleave=2;
					
		trackformat=IBMFORMAT_DD;
		floppydisk->floppyiftype=GENERIC_SHUGART_DD_FLOPPYMODE;
		floppydisk->tracks=(CYLINDER**)malloc(sizeof(CYLINDER*)*floppydisk->floppyNumberOfTrack+1);
		floppycontext->hxc_printf(MSG_DEBUG,"rpm %d bitrate:%d track:%d side:%d sector:%d",rpm,floppydisk->floppyBitRate,floppydisk->floppyNumberOfTrack,floppydisk->floppyNumberOfSide,floppydisk->floppySectorPerTrack);
		trackdata=(unsigned char*)malloc(sectorsize*floppydisk->floppySectorPerTrack);
			
		for(j=0;j<floppydisk->floppyNumberOfTrack;j++)
		{
			floppydisk->tracks[j]=allocCylinderEntry(rpm,2);
			currentcylinder=floppydisk->tracks[j];
				
			for(i=0;i<floppydisk->floppyNumberOfSide;i++)
			{
				file_offset=(sectorsize*(j*floppydisk->floppySectorPerTrack*floppydisk->floppyNumberOfSide))+
					(sectorsize*(floppydisk->floppySectorPerTrack)*i);
				
				fseek (f , file_offset , SEEK_SET);
				fread(trackdata,sectorsize*floppydisk->floppySectorPerTrack,1,f);		
				
				switch(floppydisk->floppyNumberOfSide)
				{
					case 1:
						// dummy/empty track side 0. 
						currentcylinder->sides[0]=tg_generateTrack(trackdata,sectorsize,0 ,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,ISOFORMAT_DD,gap3len,0,2500| NO_SECTOR_UNDER_INDEX,-2500);
						// first track to side 1
						currentcylinder->sides[1]=tg_generateTrack(trackdata,sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500| NO_SECTOR_UNDER_INDEX,-2500);
					break;

					case 2:
						currentcylinder->sides[i]=tg_generateTrack(trackdata,sectorsize,floppydisk->floppySectorPerTrack,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,trackformat,gap3len,0,2500| NO_SECTOR_UNDER_INDEX,-2500);
					break;
				}
			}
		}
		
		// add a dummy track...
		floppydisk->tracks[floppydisk->floppyNumberOfTrack]=allocCylinderEntry(rpm,2);
		currentcylinder=floppydisk->tracks[j];
		currentcylinder->sides[0]=tg_generateTrack(trackdata,sectorsize,0 ,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,ISOFORMAT_DD,gap3len,0,2500| NO_SECTOR_UNDER_INDEX,-2500);
		currentcylinder->sides[1]=tg_generateTrack(trackdata,sectorsize,0 ,(unsigned char)j,(unsigned char)i,1,interleave,(unsigned char)(0),floppydisk->floppyBitRate,currentcylinder->floppyRPM,ISOFORMAT_DD,gap3len,0,2500| NO_SECTOR_UNDER_INDEX,-2500);
		floppydisk->floppyNumberOfTrack++;

		// 2 sides floppy mode forced.
		floppydisk->floppyNumberOfSide=2;

		free(trackdata);

		floppycontext->hxc_printf(MSG_INFO_1,"track file successfully loaded and encoded!");

		hxc_fclose(f);

		hxcfe_sanityCheck(floppycontext,floppydisk);

		return HXCFE_NOERROR;

	}
	
	floppycontext->hxc_printf(MSG_ERROR,"file size=%d !?",filesize);
	hxc_fclose(f);
	return HXCFE_BADFILE;
}

int CAMPUTERSLYNX_libGetPluginInfo(HXCFLOPPYEMULATOR* floppycontext,unsigned long infotype,void * returnvalue)
{

	static const char plug_id[]="CAMPUTERSLYNX";
	static const char plug_desc[]="CAMPUTERSLYNX Loader";
	static const char plug_ext[]="ldf";

	plugins_ptr plug_funcs=
	{
		(ISVALIDDISKFILE)	CAMPUTERSLYNX_libIsValidDiskFile,
		(LOADDISKFILE)		CAMPUTERSLYNX_libLoad_DiskFile,
		(WRITEDISKFILE)		0,
		(GETPLUGININFOS)	CAMPUTERSLYNX_libGetPluginInfo
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


