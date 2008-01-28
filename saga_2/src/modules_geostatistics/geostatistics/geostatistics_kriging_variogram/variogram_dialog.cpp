
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//            geostatistics_kriging_variogram            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 Variogram_Dialog.cpp                  //
//                                                       //
//                 Copyright (C) 2008 by                 //
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
//                Bundesstr. 55                          //
//                20146 Hamburg                          //
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
#include "sgui_diagram.h"

#include "variogram_dialog.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CVariogram_Diagram : public CSGUI_Diagram
{
public:
	CVariogram_Diagram(wxWindow *pParent, CSG_Trend *pVariogram, CSG_Points *pVariances);
	virtual ~CVariogram_Diagram(void)	{}

	CSG_Points					*m_pVariances;

	CSG_Trend					*m_pVariogram;


	void						Set_Distance			(double d = -1.0);


private:

	virtual void				On_Draw					(wxDC &dc, wxRect rDraw);


	DECLARE_EVENT_TABLE()

};

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVariogram_Diagram, CSGUI_Diagram)
END_EVENT_TABLE()

//---------------------------------------------------------
CVariogram_Diagram::CVariogram_Diagram(wxWindow *pParent, CSG_Trend *pVariogram, CSG_Points *pVariances)
	: CSGUI_Diagram(pParent)
{
	m_pVariogram	= pVariogram;
	m_pVariances	= pVariances;

	m_xName			= _TL("Distance"); 
	m_yName			= _TL("Semi-Variance"); 

	//-----------------------------------------------------
	m_pVariogram->Set_Data(*m_pVariances);

	if( m_pVariogram->Get_Data_Count() > 1 )
	{
		m_xMin	= m_yMin	= 0.0;
		m_xMax	= (1.0 + 0.02) * m_pVariogram->Get_Data_XMax();
		m_yMax	= (1.0 + 0.02) * m_pVariogram->Get_Data_YMax();
	}
}

//---------------------------------------------------------
void CVariogram_Diagram::Set_Distance(double d)
{
	m_pVariogram->Clr_Data();

	for(int i=0; i<m_pVariances->Get_Count(); i++)
	{
		if( m_pVariances->Get_X(i) <= d )
		{
			m_pVariogram->Add_Data(m_pVariances->Get_X(i), m_pVariances->Get_Y(i));
		}
	}
}

//---------------------------------------------------------
void CVariogram_Diagram::On_Draw(wxDC &dc, wxRect rDraw)
{
	if( m_pVariances->Get_Count() > 0 )
	{
		int		i, ix, iy, jx, jy;
		double	x, dx;

		//-------------------------------------------------
		if( m_pVariogram->Get_Data_Count() > 0 )
		{
			ix	= Get_xToScreen(m_pVariogram->Get_Data_XMax());
			dc.SetPen  (wxPen(wxColour(  0, 127,   0), 2));
			dc.DrawLine(ix, Get_yToScreen(m_yMin), ix, Get_yToScreen(m_yMax));
		}

		//-------------------------------------------------
	//	dc.SetPen  (*wxWHITE_PEN);
		dc.SetPen  (wxColour(127, 127, 127));
		dc.SetBrush(*wxBLACK_BRUSH);

		for(i=0; i<m_pVariances->Get_Count(); i++)
		{
			ix	= Get_xToScreen(m_pVariances->Get_X(i));
			iy	= Get_yToScreen(m_pVariances->Get_Y(i));

			dc.DrawCircle(ix, iy, 3);
		}

		//-------------------------------------------------
		if( m_pVariogram->is_Okay() )
		{
			dc.SetPen(wxPen(*wxRED, 2));

			dx	= (m_xMax - m_xMin) / (double)rDraw.GetWidth();

			ix	= Get_xToScreen(m_xMin);
			iy	= Get_yToScreen(m_pVariogram->Get_Value(m_xMin));

			for(x=m_xMin+dx; x<=m_xMax; x+=dx)
			{
				jx	= ix;
				jy	= iy;
				ix	= Get_xToScreen(x);
				iy	= Get_yToScreen(m_pVariogram->Get_Value(x));

				dc.DrawLine(jx, jy, ix, iy);
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVariogram_Dialog, CSGUI_Dialog)
	EVT_CHOICE		(wxID_ANY	, CVariogram_Dialog::On_Update_Choices)
	EVT_TEXT_ENTER	(wxID_ANY	, CVariogram_Dialog::On_Update_Text)
//	EVT_SPINCTRL	(wxID_ANY	, CVariogram_Dialog::On_Update_SpinCtrl)
	EVT_SLIDER		(wxID_ANY	, CVariogram_Dialog::On_Update_Text)
END_EVENT_TABLE()

//---------------------------------------------------------
CVariogram_Dialog::CVariogram_Dialog(CSG_Trend *pVariogram, CSG_Points *pVariances)
	: CSGUI_Dialog(_TL("Semi-Variogram"))
{
	int				i;
	double			Distance;
	wxArrayString	Formulas;

	Formulas.Empty();
	Formulas.Add(SG_T("a + b * x"));
	Formulas.Add(SG_T("a + b * x + c * x^2"));
	Formulas.Add(SG_T("a + b * x + c * x^2 + d * x^3"));
	Formulas.Add(SG_T("a + b * x + c * x^2 + d * x^3 + e * x^4"));
	Formulas.Add(SG_T("a + b * ln(x)"));
	Formulas.Add(SG_T("a + b * x^c"));
	Formulas.Add(SG_T("a + b / x"));
	Formulas.Add(SG_T("a + b * sqrt(c + x)"));
	Formulas.Add(SG_T("a * (1 - exp(-(abs(x) / b)^2))"));

	//-----------------------------------------------------
	for(i=0, Distance=0.0; i<pVariances->Get_Count(); i++)
	{
		if( Distance < pVariances->Get_X(i) )
		{
			Distance	= pVariances->Get_X(i);
		}
	}

	//-----------------------------------------------------
	Add_Button(_TL("Ok")		, wxID_OK);
	Add_Button(_TL("Cancel")	, wxID_CANCEL);

	Add_Spacer();
	m_pFormulas		= Add_Choice(_TL("Predefined Functions"), Formulas, 0);

	Add_Spacer();
//	m_pDistance		= Add_SpinCtrl(_TL("Function Fitting Range"), Distance, 0.0, Distance);
	m_pDistance		= Add_Slider(_TL("Function Fitting Range"), Distance, 0.0, Distance);

	Add_Spacer();
	m_pParameters	= Add_TextCtrl(_TL("Function Parameters"), wxTE_MULTILINE|wxTE_READONLY);

	//-----------------------------------------------------
	Add_Output(
		m_pDiagram = new CVariogram_Diagram(this, pVariogram, pVariances),
		m_pFormula = new wxTextCtrl(this, wxID_ANY, Formulas[0], wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER),
		1, 0
	);

	//-----------------------------------------------------
	Fit_Function();
}

//---------------------------------------------------------
CVariogram_Dialog::~CVariogram_Dialog(void)
{}

//---------------------------------------------------------
void CVariogram_Dialog::On_Update_Choices(wxCommandEvent &WXUNUSED(event))
{
	m_pFormula->SetValue(m_pFormulas->GetStringSelection().c_str());

	Fit_Function();
}

//---------------------------------------------------------
void CVariogram_Dialog::On_Update_Text(wxCommandEvent &WXUNUSED(event))
{
	Fit_Function();
}

//---------------------------------------------------------
void CVariogram_Dialog::On_Update_SpinCtrl(wxSpinEvent &WXUNUSED(event))
{
	Fit_Function();
}

//---------------------------------------------------------
void CVariogram_Dialog::Fit_Function(void)
{
	m_pDiagram->Set_Distance(m_pDistance->Get_Value());

	if(	!m_pDiagram->m_pVariogram->Set_Formula(m_pFormula->GetValue().c_str()) )
	{
		m_pParameters->SetValue(_TL("Invalid formula !!!"));
	}
	else if( !m_pDiagram->m_pVariogram->Get_Trend() )
	{
		m_pParameters->SetValue(_TL("Function fitting failed !!!"));
	}
	else
	{
		m_pParameters->SetValue(m_pDiagram->m_pVariogram->Get_Formula().c_str());
	}

	m_pDiagram->Refresh(true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
