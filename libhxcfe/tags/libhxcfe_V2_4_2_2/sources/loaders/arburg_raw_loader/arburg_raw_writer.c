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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libhxcfe.h"

#include "arburg_raw_loader.h"

#include "tracks/sector_extractor.h"

#include "libhxcadaptor.h"

int ARBURG_RAW_libWrite_DiskFile(HXCFLOPPYEMULATOR* floppycontext,FLOPPY * floppy,char * filename)
{
	int i,j,k,l,nbsector;
	FILE * rawfile;
	char * log_str;
	char   tmp_str[256];
	unsigned char blankblock[0xF02];
	int sectorsize,track_type_id;
	int systblockfound;

	SECTORSEARCH* ss;
	SECTORCONFIG** sca;

	floppycontext->hxc_printf(MSG_INFO_1,"Write Arburg RAW file %s...",filename);

	track_type_id=0;
	log_str=0;

	systblockfound = 0;

	memset(blankblock,0x00,0xF00);

	if((floppy->floppyNumberOfTrack < 80) || (floppy->floppyNumberOfSide != 2) )
	{
		return HXCFE_BADPARAMETER;
	}

	rawfile=hxc_fopen(filename,"wb");
	if(rawfile)
	{
		ss=hxcfe_initSectorSearch(floppycontext,floppy);
		if(ss)
		{
			for(i=0;i<2;i++)
			{
				for(j=0;j<80;j++)
				{
					sprintf(tmp_str,"track:%.2d:%d file offset:0x%.6x, sectors: ",j,i,(unsigned int)ftell(rawfile));

					log_str=0;
					log_str=realloc(log_str,strlen(tmp_str)+1);
					memset(log_str,0,strlen(tmp_str)+1);
					strcat(log_str,tmp_str);

					k=0;
					do
					{
						switch(track_type_id)
						{
							case 0:
								sca = hxcfe_getAllTrackSectors(ss,j,i,ARBURGDAT_ENCODING,&nbsector);
							break;
							case 1:
								sca = hxcfe_getAllTrackSectors(ss,j,i,ARBURGSYS_ENCODING,&nbsector);
								if(nbsector)
									systblockfound = 0xFF;
							break;
						}

						if(!nbsector)
							track_type_id=(track_type_id+1)%2;

						k++;

					}while(!nbsector && k<2);

					if(nbsector)
					{
						sectorsize=sca[0]->sectorsize;
						for(l=0;l<256;l++)
						{

							k=0;
							do
							{
								if(sca[k]->sector==l)
								{
									if(sca[k]->sectorsize!=(unsigned int)sectorsize)
									{
										sectorsize=-1;
									}
									if(sca[k]->input_data)
										fwrite(sca[k]->input_data,sca[k]->sectorsize,1,rawfile);

									sprintf(tmp_str,"%d ",sca[k]->sector);
									log_str=realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
									strcat(log_str,tmp_str);
									break;
								}

								k++;
							}while(k<nbsector);
						}

						k=0;
						do
						{
							if(sca[k])
							{
								if(sca[k]->input_data)
									free(sca[k]->input_data);
								free(sca[k]);
							}
							k++;
						}while(k<nbsector);


						if(sectorsize!=-1)
						{
							sprintf(tmp_str,",%dB/s",sectorsize);
						}
						else
						{
							sprintf(tmp_str,"");
						}

						log_str=realloc(log_str,strlen(log_str)+strlen(tmp_str)+1);
						strcat(log_str,tmp_str);

					}
					else
					{
						floppycontext->hxc_printf(MSG_WARNING,"No Arburg block found !?!...");
						if( ((j<10) && (i==0) ) || !systblockfound)
						{
							// Block Not found ?!?
							// Put a blank data sector instead with a bad checksum...
							memset(blankblock,0x00,0xA00);
							for(k=0;k<0xA0;k++)
								strcat((char*)blankblock,">MISSING BLOCK<!");
							fwrite(blankblock,0xA00,1,rawfile);
						}
						else
						{
							memset(blankblock,0x00,0xF00);
							for(k=0;k<0xF0;k++)
								strcat((char*)blankblock,">MISSING BLOCK<!");
							fwrite(blankblock,0xF00,1,rawfile);
						}

					}

					floppycontext->hxc_printf(MSG_INFO_1,log_str);
					free(log_str);

				}
			}

			hxcfe_deinitSectorSearch(ss);
		}

		hxc_fclose(rawfile);
	}

	return 0;
}
