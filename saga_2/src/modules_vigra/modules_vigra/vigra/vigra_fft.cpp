
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                        VIGRA                          //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     vigra_fft.cpp                     //
//                                                       //
//                 Copyright (C) 2009 by                 //
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
#include "vigra_fft.h"

//---------------------------------------------------------
#include <vigra/fftw3.hxx>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool	Copy_ComplexGrid_SAGA_to_VIGRA	(CSG_Grid &Real, CSG_Grid &Imag, FFTWComplexImage &Image, bool bCreate)
{
	if( bCreate )
	{
		Image.resize(Real.Get_NX(), Real.Get_NY());
	}

	if(	Real.Get_NX() != Image.width() || Real.Get_NY() != Image.height()
	||	Imag.Get_NX() != Image.width() || Imag.Get_NY() != Image.height() )
	{
		return( false );
	}

	for(int y=0; y<Real.Get_NY() && SG_UI_Process_Set_Progress(y, Real.Get_NY()); y++)
	{
		for(int x=0; x<Real.Get_NX(); x++)
		{
			Image(x, y)	= FFTWComplex(Real.asDouble(x, y), Imag.asDouble(x, y));
		}
	}

	SG_UI_Process_Set_Progress(0.0, 1.0);

	return( true );
}

//---------------------------------------------------------
bool	Copy_ComplexGrid_VIGRA_to_SAGA	(CSG_Grid &Real, CSG_Grid &Imag, FFTWComplexImage &Image, bool bCreate)
{
	if( bCreate )
	{
		Real.Create(SG_DATATYPE_Float, Image.width(), Image.height());
		Imag.Create(SG_DATATYPE_Float, Image.width(), Image.height());
	}

	if(	Real.Get_NX() != Image.width() || Real.Get_NY() != Image.height()
	||	Imag.Get_NX() != Image.width() || Imag.Get_NY() != Image.height() )
	{
		return( false );
	}

	for(int y=0; y<Real.Get_NY() && SG_UI_Process_Set_Progress(y, Real.Get_NY()); y++)
	{
		for(int x=0; x<Real.Get_NX(); x++)
		{
		//	Real.Set_Value(x, y, c.magnitude());
		//	Imag.Set_Value(x, y, c.phase());
			Real.Set_Value(x, y, Image(x, y).re());
			Imag.Set_Value(x, y, Image(x, y).im());
		}
	}

	SG_UI_Process_Set_Progress(0.0, 1.0);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CViGrA_FFT::CViGrA_FFT(void)
{
	Set_Name		(_TL("ViGrA - FFT"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"References:\n"
		"ViGrA - Vision with Generic Algorithms\n"
		"<a target=\"_blank\" href=\"http://hci.iwr.uni-heidelberg.de/vigra\">http://hci.iwr.uni-heidelberg.de</a>"
	));

	Parameters.Add_Grid(
		NULL	, "INPUT"		, _TL("Input"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "REAL"		, _TL("Real"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Grid(
		NULL	, "IMAG"		, _TL("Imaginary"),
		_TL(""),
		PARAMETER_OUTPUT
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CViGrA_FFT::On_Execute(void)
{
	CSG_Grid	*pInput, *pReal, *pImag;

	pInput	= Parameters("INPUT")	->asGrid();
	pReal	= Parameters("REAL")	->asGrid();
	pImag	= Parameters("IMAG")	->asGrid();

	//-----------------------------------------------------
	vigra::FImage			Input;
	vigra::FFTWComplexImage	Output(Get_NX(), Get_NY());

	Copy_Grid_SAGA_to_VIGRA(*pInput, Input, true);

	fourierTransform(srcImageRange(Input), destImage(Output));

	//-----------------------------------------------------
	Copy_ComplexGrid_VIGRA_to_SAGA(*pReal, *pImag, Output, false);

	pReal->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), pInput->Get_Name(), _TL("Real")));
	pImag->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), pInput->Get_Name(), _TL("Imaginary")));

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CViGrA_FFT_Inverse::CViGrA_FFT_Inverse(void)
{
	Set_Name		(_TL("ViGrA - FFT Inverse"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"References:\n"
		"ViGrA - Vision with Generic Algorithms\n"
		"<a target=\"_blank\" href=\"http://hci.iwr.uni-heidelberg.de/vigra\">http://hci.iwr.uni-heidelberg.de</a>"
	));

	Parameters.Add_Grid(
		NULL	, "REAL"		, _TL("Real"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "IMAG"		, _TL("Imaginary"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "REAL_OUT"	, _TL("Real"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Grid(
		NULL	, "IMAG_OUT"	, _TL("Imaginary"),
		_TL(""),
		PARAMETER_OUTPUT
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CViGrA_FFT_Inverse::On_Execute(void)
{
	CSG_Grid	*pReal, *pImag;

	pReal	= Parameters("REAL")->asGrid();
	pImag	= Parameters("IMAG")->asGrid();

	//-----------------------------------------------------
	vigra::FFTWComplexImage	Input, Output(Get_NX(), Get_NY());

	Copy_ComplexGrid_SAGA_to_VIGRA(*pReal, *pImag, Input, true);

	fourierTransformInverse(srcImageRange(Input), destImage(Output));

	//-----------------------------------------------------
	pReal	= Parameters("REAL_OUT")->asGrid();
	pImag	= Parameters("IMAG_OUT")->asGrid();

	Copy_ComplexGrid_VIGRA_to_SAGA(*pReal, *pImag, Output, false);

	pReal->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), Get_Name(), _TL("Real")));
	pImag->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), Get_Name(), _TL("Imaginary")));

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CViGrA_FFT_Real::CViGrA_FFT_Real(void)
{
	Set_Name		(_TL("ViGrA - FFT Real"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"References:\n"
		"ViGrA - Vision with Generic Algorithms\n"
		"<a target=\"_blank\" href=\"http://hci.iwr.uni-heidelberg.de/vigra\">http://hci.iwr.uni-heidelberg.de</a>"
	));

	Parameters.Add_Grid(
		NULL	, "INPUT"		, _TL("Input"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "OUTPUT"		, _TL("Output"),
		_TL(""),
		PARAMETER_OUTPUT
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CViGrA_FFT_Real::On_Execute(void)
{
	CSG_Grid	*pInput, *pOutput;

	pInput	= Parameters("INPUT")	->asGrid();
	pOutput	= Parameters("OUTPUT")	->asGrid();

	//-----------------------------------------------------
	vigra::FImage	Input, Output(Get_NX(), Get_NY());

	Copy_Grid_SAGA_to_VIGRA(*pInput, Input, true);

	// forward cosine transform == reflective boundary conditions
	fourierTransformRealEE(srcImageRange(Input), destImage(Output), (fftw_real)1.0);

	//-----------------------------------------------------
	Copy_Grid_VIGRA_to_SAGA(*pOutput, Output, false);

	pOutput->Set_Name(CSG_String::Format(SG_T("%s [%s - %s]"), Get_Name(), pInput->Get_Name()));

	return( true );

	/*/ multiply with a first derivative of Gaussian in x-direction
	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=1; x<Get_NX(); x++)
		{
			double	dx	= x * M_PI / (Get_NX() - 1);
			double	dy	= y * M_PI / (Get_NY() - 1);

			fourier(x-1, y) = fourier(x, y) * dx * std::exp(-(dx*dx + dy*dy) * scale*scale / 2.0);
		}

		fourier(width-1, y) = 0.0;
	}

	// inverse transform -- odd symmetry in x-direction, even in y, due to symmetry of the filter
	fourierTransformRealOE(srcImageRange(fourier), destImage(spatial), (fftw_real)-4.0 * (Get_NX() + 1) * (Get_NY() - 1));
	/**/
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CViGrA_FFT_Filter::CViGrA_FFT_Filter(void)
{
	Set_Name		(_TL("ViGrA - FFT Filter"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"References:\n"
		"ViGrA - Vision with Generic Algorithms\n"
		"<a target=\"_blank\" href=\"http://hci.iwr.uni-heidelberg.de/vigra\">http://hci.iwr.uni-heidelberg.de</a>"
	));

	Parameters.Add_Grid(
		NULL	, "INPUT"		, _TL("Input"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "REAL"		, _TL("Output (real)"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Grid(
		NULL	, "IMAG"		, _TL("Output (imaginary)"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Value(
		NULL	, "SCALE"	, _TL("Size of smoothing filter"),
		_TL(""),
		PARAMETER_TYPE_Double, 2.0, 0.0, true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CViGrA_FFT_Filter::On_Execute(void)
{
	double		Scale;
	CSG_Grid	*pInput, *pReal, *pImag;

	pInput	= Parameters("INPUT")	->asGrid();
	pReal	= Parameters("REAL")	->asGrid();
	pImag	= Parameters("IMAG")	->asGrid();
	Scale	= Parameters("SCALE")	->asDouble();

	//-----------------------------------------------------
	vigra::FImage			Input, Filter_Gauss(Get_NX(), Get_NY()), Filter(Get_NX(), Get_NY());
	vigra::FFTWComplexImage	Output(Get_NX(), Get_NY());

	Copy_Grid_SAGA_to_VIGRA(*pInput, Input, true);

	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=0; x<Get_NX(); x++)
		{
			double	xx	= (x - Get_NX() / 2.0) / Get_NX();
			double	yy	= (y - Get_NY() / 2.0) / Get_NY();

			Filter_Gauss(x, y)	= std::exp(-(xx*xx + yy*yy) / 2.0 * Scale);
		}
	}

	moveDCToUpperLeft(srcImageRange(Filter_Gauss), destImage(Filter));    // applyFourierFilter() expects the filter's DC in the upper left

	vigra::applyFourierFilter(srcImageRange(Input), srcImage(Filter), destImage(Output));

	//-----------------------------------------------------
	Copy_ComplexGrid_VIGRA_to_SAGA(*pReal, *pImag, Output, false);

	pReal->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), pInput->Get_Name(), _TL("Real")));
	pImag->Set_Name(CSG_String::Format(SG_T("%s [FFT - %s]"), pInput->Get_Name(), _TL("Imaginary")));

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------