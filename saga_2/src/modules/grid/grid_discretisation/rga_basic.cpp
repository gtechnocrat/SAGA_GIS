
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                  grid_discretisation                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    rga_basic.cpp                      //
//                                                       //
//                 Copyright (C) 2008 by                 //
//                     Benni Bechtel                     //
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
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
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
#include "rga_basic.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define	SEEDFIELD_X		2
#define	SEEDFIELD_Y		(SEEDFIELD_X + 1)
#define	SEEDFIELD_Z		(SEEDFIELD_X + 2)

#define NO_SEGMENT		-1
#define NO_SIMILARITY	-1.0


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CCandidates::CCandidates(void)
{
	m_Candidates	= NULL;

	Create();
}

//---------------------------------------------------------
CCandidates::~CCandidates(void)
{
	Destroy();
}

//---------------------------------------------------------
void CCandidates::Create(void)
{
	Destroy();
}

//---------------------------------------------------------
void CCandidates::Destroy(void)
{
	if( m_Candidates )
	{
		SG_Free(m_Candidates);
	}

	m_nBuffer		= 0;
	m_nCandidates	= 0;
	m_Candidates	= NULL;
}

#include <string.h>
//---------------------------------------------------------
void CCandidates::Add(int x, int y, int Segment, double Similarity)
{
	int		iInsert	= _Find(Similarity);

	if( m_nCandidates >= m_nBuffer )
	{
		m_nBuffer		+= 32;
		m_Candidates	= (TCandidate *)SG_Realloc(m_Candidates, m_nBuffer * sizeof(TCandidate));
	}

	memmove(m_Candidates + iInsert + 1, m_Candidates + iInsert, sizeof(TCandidate) * (m_nCandidates - iInsert));
//	for(int i=m_nCandidates; i>iInsert; i--)
//	{
//		m_Candidates[i]	= m_Candidates[i - 1];
//	}

	m_nCandidates++;

	m_Candidates[iInsert].x				= x;
	m_Candidates[iInsert].y				= y;
	m_Candidates[iInsert].Segment		= Segment;
	m_Candidates[iInsert].Similarity	= Similarity;
}

//---------------------------------------------------------
int CCandidates::_Find(double Similarity)
{
	if( m_nCandidates == 0 )
	{
		return( 0 );
	}

	int		a, b;

	a	= 0;
	b	= m_nCandidates - 1;

	if( Similarity < m_Candidates[a].Similarity )
	{
		return( a );
	}

	if( Similarity > m_Candidates[b].Similarity )
	{
		return( b + 1 );
	}

	for(int d=(b-a)/2 ; d>0; d/=2)
	{
		int		i	= a + d;

		if( Similarity > m_Candidates[i].Similarity )
		{
			a	= a < i ? i : a + 1;
		}
		else
		{
			b	= b > i ? i : b - 1;
		}
	}

	for(int i=a; i<=b; i++)
	{
		if( Similarity < m_Candidates[i].Similarity )
		{
			return( i );
		}
	}

	return( b );
}

/*int CCandidates::_Find(double Similarity)
{
	for(int i=0; i<m_nCandidates; i++)
	{
		if( Similarity < m_Candidates[i].Similarity )
		{
			return( i );
		}
	}

	return( m_nCandidates );
}/**/

//---------------------------------------------------------
bool CCandidates::Get(int &x, int &y, int &Segment)
{
	if( m_nCandidates > 0 )
	{
		m_nCandidates--;

		x		= m_Candidates[m_nCandidates].x;
		y		= m_Candidates[m_nCandidates].y;
		Segment	= m_Candidates[m_nCandidates].Segment;

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CRGA_Basic::CRGA_Basic(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name		(_TL("Simple Region Growing"));

	Set_Author		(SG_T("B. Bechtel, O. Conrad (c) 2008"));

	Set_Description	(_TW(
		"!!!UNDER DEVELOPMENT!!!\n"
		"\n"
		"References\n"
		"Bechtel, B., Ringeler, A., Boehner, J. (2008): "
		"Segmentation for Object Extraction of Trees using MATLAB and SAGA. "
		"In: Boehner, J., Blaschke, T., Montanarella, L. [Eds.]: SAGA - Seconds Out. "
		"Hamburger Beitraege zur Physischen Geographie und Landschaftsoekologie, 19:59-70. "
		"<a href=\"http://downloads.sourceforge.net/saga-gis/hbpl19_01.pdf\">download</a>\n"
		"\n"
	));

	//-----------------------------------------------------
	Parameters.Add_Grid(
		NULL	, "SEEDS"		, _TL("Seeds"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid_List(
		NULL	, "FEATURES"	, _TL("Features"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "SEGMENTS"	, _TL("Segments"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Grid(
		NULL	, "SIMILARITY"	, _TL("Similarity"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Table(
		NULL	, "TABLE"		, _TL("Seeds"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice(
		NULL	, "METHOD"		, _TL("Method"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("colour and position"),
			_TL("colour")
		), 0
	);

	Parameters.Add_Choice(
		NULL	, "NEIGHBOUR"	, _TL("Neighbourhood"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("4 (von Neumann)"),
			_TL("8 (Moore)")
		), 0
	);

	pNode	= Parameters.Add_Node(
		NULL	, "NODE_COLSPACE"	, _TL("Colour and Space Options"),
		_TL("")
	);

	Parameters.Add_Value(
		pNode	, "SIG_1"			, _TL("Variance in colour space"),
		_TL(""),
		PARAMETER_TYPE_Double		, 1.0, 0.0, true	// 0.36
	);

	Parameters.Add_Value(
		pNode	, "SIG_2"			, _TL("Variance in position space"),
		_TL(""),
		PARAMETER_TYPE_Double		, 1.0, 0.0, true	// 8.2141
	);

	Parameters.Add_Value(
		pNode	, "THRESHOLD"		, _TL("Threshold - similarity"),
		_TL(""),
		PARAMETER_TYPE_Double		, 0.0, 0.0, true	// 0.15
	);

	Parameters.Add_Value(
		pNode	, "REFRESH"			, _TL("Refresh"),
		_TL(""),
		PARAMETER_TYPE_Bool			, false
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CRGA_Basic::On_Execute(void)
{
	bool		bRefresh;
	int			x, y, i, n, Segment;
	CSG_Grid	*pSeeds;

	//-----------------------------------------------------
	m_pSegments		= Parameters("SEGMENTS")	->asGrid();
	m_pFeatures		= Parameters("FEATURES")	->asGridList();
	m_nFeatures		= m_pFeatures->Get_Count();

	pSeeds			= Parameters("SEEDS")		->asGrid();
	m_pSeeds		= Parameters("TABLE")		->asTable();

	m_pSimilarity	= Parameters("SIMILARITY")	->asGrid();

	m_dNeighbour	= Parameters("NEIGHBOUR")->asInt() == 0 ? 2 : 1;

	m_Var_1			= SG_Get_Square(Parameters("SIG_1")->asDouble());
	m_Var_2			= SG_Get_Square(Parameters("SIG_2")->asDouble());
	m_Threshold		= Parameters("THRESHOLD")	->asDouble();

	m_Method		= Parameters("METHOD")		->asInt();
	bRefresh		= Parameters("REFRESH")		->asBool();

	//-----------------------------------------------------
	m_pSegments		->Assign(-1);
	m_pSegments		->Set_NoData_Value(-1);

	m_pSimilarity	->Assign(-1);
	m_pSimilarity	->Set_NoData_Value(-1);

	//-----------------------------------------------------
	m_pSeeds->Destroy();

	m_pSeeds->Add_Field(_TL("ID")	, TABLE_FIELDTYPE_Int);
	m_pSeeds->Add_Field(_TL("AREA")	, TABLE_FIELDTYPE_Double);
	m_pSeeds->Add_Field(_TL("X")	, TABLE_FIELDTYPE_Double);
	m_pSeeds->Add_Field(_TL("Y")	, TABLE_FIELDTYPE_Double);

	for(i=0; i<m_pFeatures->Get_Count(); i++)
	{
		m_pSeeds->Add_Field(m_pFeatures->asGrid(i)->Get_Name(), TABLE_FIELDTYPE_Double);
	}

	//-----------------------------------------------------
	for(y=0, n=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{
			if( pSeeds->asInt(x, y) != 0 )
			{
				CSG_Table_Record	*pRec	= m_pSeeds->Add_Record();

				pRec->Set_Value(0, n);
				pRec->Set_Value(SEEDFIELD_X, x);
				pRec->Set_Value(SEEDFIELD_Y, y);

				for(i=0; i<m_pFeatures->Get_Count(); i++)
				{
					pRec->Set_Value(SEEDFIELD_Z + i, m_pFeatures->asGrid(i)->asDouble(x, y));
				}

				m_pSimilarity->Set_Value(x, y, 1.0);

				Add_To_Segment(x, y, n++);
			}
		}
	}

	//-----------------------------------------------------
	if( n > 0 )
	{
		n	= 0;

		while( n++ < Get_NCells() && Set_Progress_NCells(n) && Get_Next_Candidate(x, y, Segment) )
		{
			Add_To_Segment(x, y, Segment);

			if( bRefresh && (n % Get_NX()) == 0 )
			{
				DataObject_Update(m_pSegments, 0, m_pSeeds->Get_Record_Count());

				Process_Set_Text(CSG_String::Format(SG_T("%.2f"), 100.0 * m_Candidates.Get_Count() / Get_NCells()));
			}
		}

		m_Candidates.Destroy();

		return( true );
	}

	//-----------------------------------------------------
	m_Candidates.Destroy();

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CRGA_Basic::Get_Next_Candidate(int &x, int &y, int &Segment)
{
	while( m_Candidates.Get(x, y, Segment) )
	{
		if( m_pSegments->is_NoData(x, y) )
		{
			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CRGA_Basic::Add_To_Segment(int x, int y, int Segment)
{
	if( is_InGrid(x, y) && m_pSegments->is_NoData(x, y) )			// if the pixel is not element of any segment...
	{
		int		i, ix, iy;

		m_pSegments->Set_Value(x, y, Segment);						// the candidate is added to the correspondig region
	
		for(i=0; i<8; i+=m_dNeighbour)								// update of candidate-grid - all 8-Neigbours of the added pixel are checked
		{
			if( Get_System()->Get_Neighbor_Pos(i, x, y, ix, iy) && m_pSegments->is_NoData(ix, iy) )
			{
				double	Similarity	= Get_Similarity(ix, iy, Segment);

				if(	Similarity >= m_Threshold						// a neigbour-pixel is only added as candidate if its similarity is higher than the preset threshold
				&&	Similarity > m_pSimilarity->asDouble(ix, iy) )	// and it is not candidate for another region with a higher similarity-value yet
				{
					m_Candidates.Add(ix, iy, Segment, Similarity);

					m_pSimilarity->Set_Value(ix, iy, Similarity);
				}
			}
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
double CRGA_Basic::Get_Similarity(int x, int y, int Segment)
{
	CSG_Table_Record	*pSeed;

	if( is_InGrid(x, y) && (pSeed = m_pSeeds->Get_Record(Segment)) != NULL )
	{
		int		i;
		double	a, b, Result;

		switch( m_Method )
		{
		//-------------------------------------------------
		case 0:	// Colour and position
			for(i=0, a=0.0; i<m_nFeatures; i++)
			{
				a	+= SG_Get_Square(m_pFeatures->asGrid(i)->asDouble(x, y) - pSeed->asDouble(SEEDFIELD_Z + i));
			}

			b	= SG_Get_Square(x - pSeed->asDouble(SEEDFIELD_X))
				+ SG_Get_Square(y - pSeed->asDouble(SEEDFIELD_Y));

			Result	= a / m_Var_1 + b / m_Var_2;

			break;

		//-------------------------------------------------
		case 1:	// Colour
			for(i=0, a=0.0; i<m_nFeatures; i++)
			{
				a	+= SG_Get_Square(m_pFeatures->asGrid(i)->asDouble(x, y) - pSeed->asDouble(SEEDFIELD_Z + i));
			}

			Result	= a / m_Var_1;

			break;
		}

		return( 1.0 / (1.0 + Result) );	// from 'distance' to 'similarity' !!!
	//	return( exp(-0.5 * Result) );
	}

	return( -1.0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
