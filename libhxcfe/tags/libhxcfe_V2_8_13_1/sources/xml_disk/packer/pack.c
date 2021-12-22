/*
//
// Copyright (C) 2006-2018 Jean-Fran�ois DEL NERO
//
// This file is part of HxCFloppyEmulator.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "types.h"

#include "lzw.h"
#include "rle.h"

unsigned char * data_unpack(unsigned char * bufferin, uint32_t sizein,unsigned char * bufferout, uint32_t sizeout)
{
	unsigned char* buffer;

	buffer=(unsigned char*)malloc(sizeout+100);

	if(bufferin[0]&0x01)
	{
		lzw_expand(bufferin+1,buffer, sizeout );
	}
	else
	{
		memcpy(buffer,bufferin+1,sizeout);
	}

	return  buffer;
};
