#pragma once
#using <System.dll>

using namespace System;

ref class AreaCritica abstract : public ICloneable
{
protected:
	AreaCritica(void) { }
public:
	virtual bool entrataPermessa(int idTreno, int cdb, int tipoEntrata) abstract;
	virtual void entrata(int idTreno, int cdb, int tipoEntrata) abstract;
	virtual Object^ Clone() abstract;
};

