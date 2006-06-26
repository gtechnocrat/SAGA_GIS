
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    api_memory.cpp                     //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, version 2.1 of the License.      //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, write to the Free Software Foundation, Inc.,     //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "malloc.h"

#include "api_core.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

#ifndef _WINDOWS_

//---------------------------------------------------------
void *		API_Malloc(size_t size)
{
	return( malloc(size) );
}

//---------------------------------------------------------
void *		API_Calloc(size_t num, size_t size)
{
	return( calloc(num, size) );
}

//---------------------------------------------------------
void *		API_Realloc(void *memblock, size_t new_size)
{
	return( realloc(memblock, new_size) );
}

//---------------------------------------------------------
void		API_Free(void *memblock)
{
	if( memblock )
	{
		free(memblock);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// Due to a bad 'feature' in the realloc routine of MS's
// MSVCRT (C-Runtime-Library), we recommend to use our own
// memory allocation routines...

#else	// ifndef _WINDOWS_

void *		API_Malloc(size_t size)
{
	return( HeapAlloc(GetProcessHeap(), 0, size) );
}

void *		API_Calloc(size_t num, size_t size)
{
	return( HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, num * size) );
}

void *		API_Realloc(void *memblock, size_t new_size)
{
	if( new_size > 0 )
	{
		if( memblock )
		{
			return( HeapReAlloc(GetProcessHeap(), 0, memblock, new_size) );
		}
		else
		{
			return( HeapAlloc(GetProcessHeap(), 0, new_size) );
		}
	}
	else
	{
		API_Free(memblock);

		return( NULL );
	}
}

void		API_Free(void *memblock)
{
	if( memblock )
	{
		HeapFree(GetProcessHeap(), 0, memblock);
	}
}

#endif	// ifndef _WINDOWS_


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void			API_Swap_Bytes(void *Value, int nValueBytes)
{
	char	Byte, *pA, *pB;

	pA	= (char *)Value;
	pB	= pA + nValueBytes - 1;

	while( pA < pB )
	{
		Byte	= *pA;
		*(pA++)	= *pB;
		*(pB--)	= Byte;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
