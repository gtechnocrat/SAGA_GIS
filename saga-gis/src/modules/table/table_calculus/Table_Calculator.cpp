/**********************************************************
 * Version $Id$
 *********************************************************/
/*******************************************************************************
    TableCalculator.cpp
    Copyright (C) Victor Olaya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Table_Calculator.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CTableCalculator::CTableCalculator(bool bShapes)
{
	Set_Name	(!bShapes ? _TL("Table Calculator") : _TL("Table Calculator for Shapes"));

	Set_Author	(SG_T("V.Olaya (c) 2004, O.Conrad (c) 2011"));

	CSG_String	s(_TW(
		"The table calculator calculates a new attribute from existing attributes based on a mathematical formula. "
		"Attributes are addressed by the character 'f' (for 'field') followed by the field number (i.e.: f1, f2, ..., fn) "
		"or by the field name in square brackets (e.g.: [Field Name]).\n"
		"Examples:\n"
		"sin(f1) * f2 + f3\n"
		"[Population] / [Area]\n"

		"\nThe following operators are available for the formula definition:\n"
	));

	s	+= CSG_Formula::Get_Help_Operators();

	Set_Description(s);

	if( !bShapes )
	{
		Parameters.Add_Table	(NULL, "TABLE"	, _TL("Table")		, _TL(""), PARAMETER_INPUT);
		Parameters.Add_Table	(NULL, "RESULT"	, _TL("Result")		, _TL(""), PARAMETER_OUTPUT_OPTIONAL);
	}
	else
	{
		Parameters.Add_Shapes	(NULL, "TABLE"	, _TL("Shapes")		, _TL(""), PARAMETER_INPUT);
		Parameters.Add_Shapes	(NULL, "RESULT"	, _TL("Result")		, _TL(""), PARAMETER_OUTPUT_OPTIONAL);
	}

	Parameters.Add_String	(NULL, "FORMULA", _TL("Formula")	, _TL(""), SG_T("f1 + f2"));
	Parameters.Add_String	(NULL, "NAME"	, _TL("Field Name")	, _TL(""), SG_T("Calculation"));
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CTableCalculator::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Table	*pTable		= Parameters("TABLE")	->asTable();

	if( !pTable->is_Valid() || pTable->Get_Field_Count() <= 0 || pTable->Get_Record_Count() <= 0 )
	{
		Error_Set(_TL("invalid table"));

		return( false );
	}

	//-----------------------------------------------------
	const SG_Char	vars[27]	= SG_T("abcdefghijklmnopqrstuvwxyz");

	int			iField, nFields, *Fields	= new int[pTable->Get_Field_Count()];

	CSG_String	sFormula	= Parameters("FORMULA")	->asString();

	for(iField=pTable->Get_Field_Count()-1, nFields=0; iField>=0 && nFields<26; iField--)
	{
		bool		bUse	= false;

		CSG_String	sField;

		sField.Printf(SG_T("f%d"), iField + 1);

		if( sFormula.Find(sField) >= 0 )
		{
			sFormula.Replace(sField, CSG_String(vars[nFields]));

			bUse	= true;
		}

		sField.Printf(SG_T("[%s]"), pTable->Get_Field_Name(iField));

		if( sFormula.Find(sField) >= 0 )
		{
			sFormula.Replace(sField, CSG_String(vars[nFields]));

			bUse	= true;
		}

		if( bUse )
		{
			Fields[nFields++]	= iField;
		}
	}

	Message_Add(sFormula + SG_T("\n"), false);

	//-----------------------------------------------------
	CSG_Formula	Formula;

	if( !Formula.Set_Formula(sFormula) )
	{
		int			Position;
		CSG_String	Message;

		Formula.Get_Error(&Position, &Message);

		Message_Add(Message);

		Message_Add(CSG_String::Format(SG_T("%s: #%d [%s]"), _TL("syntax error, position"), Position, Formula.Get_Formula().c_str()));

		delete[](Fields);

		return( false );
	}

	//-----------------------------------------------------
	if( Parameters("RESULT")->asTable() && Parameters("RESULT")->asTable() != pTable )
	{
		pTable	= Parameters("RESULT")->asTable();
		pTable->Create(*Parameters("TABLE")->asTable());
	}

	pTable->Set_Name(CSG_String::Format(SG_T("%s [%s]"), Parameters("TABLE")->asTable()->Get_Name(), Parameters("NAME")->asString()));
	pTable->Add_Field(Parameters("NAME")->asString(), SG_DATATYPE_Double);

	//-----------------------------------------------------
	CSG_Vector	Values(nFields);

	for(int iRecord=0; iRecord<pTable->Get_Count() && Set_Progress(iRecord, pTable->Get_Count()); iRecord++)
	{
		CSG_Table_Record	*pRecord	= pTable->Get_Record(iRecord);

		for(iField=0; iField<nFields; iField++)
		{
			Values[iField]	= pRecord->asDouble(Fields[iField]);
		}

		pRecord->Set_Value(pTable->Get_Field_Count() - 1, Formula.Get_Value(Values.Get_Data(), nFields));
	}

	//-----------------------------------------------------
	delete[](Fields);

	if( pTable == Parameters("TABLE")->asTable() )
	{
		DataObject_Update(pTable);
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
