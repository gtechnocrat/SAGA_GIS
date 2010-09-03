
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                   Projection_Proj4                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    crs_transform.h                    //
//                                                       //
//                 Copyright (C) 2010 by                 //
//                      Olaf Conrad                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__crs_transform_H
#define HEADER_INCLUDED__crs_transform_H

//---------------------------------------------------------
#include "MLB_Interface.h"

#include <projects.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class pj_proj4_EXPORT CCRS_Transform : public CSG_Module
{
public:
	CCRS_Transform(void);

	virtual bool			do_Sync_Projections		(void)	{	return( false  );	}


protected:

	virtual int				On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool			On_Execute				(void);
	virtual bool			On_Execute_Conversion	(void)	= 0;

	bool					Set_Source				(const CSG_Projection &Projection);
	bool					Set_Inverse				(bool bOn = true);

	bool					Get_Converted			(double &x, double &y);
	bool					Get_Converted			(TSG_Point &Point)		{	return( Get_Converted(Point.x, Point.y) );	}


private:

	bool					m_bInverse;

	PJ						*m_pPrjSrc, *m_pPrjDst;


	bool					_Set_Projection			(const CSG_Projection &Projection, PJ **ppPrj, bool bInverse);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class pj_proj4_EXPORT CCRS_Transform_Shapes : public CCRS_Transform
{
public:
	CCRS_Transform_Shapes(void);

	virtual const SG_Char *	Get_MenuPath			(void)	{	return( _TL("R:Shapes") );	}


protected:

	virtual bool			On_Execute_Conversion	(void);


private:

	bool					_Get_Conversion			(CSG_Shapes *pSource, CSG_Shapes *pTarget);


};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__crs_transform_H