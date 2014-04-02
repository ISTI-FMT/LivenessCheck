#include "stdafx.h"
#include "AreaCriticaCircolare.h"

bool AreaCriticaCircolare::entrataPermessa(int idTreno, int cdb, int tipoEntrata)
{
	bool res = true;
	//l'ingresso � negato se:
	//- il cdb � uno di quelli dell'area
	//- il treno non � gi� dentro l'area
	//- l'area ha gi� il numero massimo di treni
	if (tipoEntrata == 1)
	{
		if (treni >= limite)
		{
			res = false;
		}
	}
	return res;
}

void AreaCriticaCircolare::entrata(int idTreno, int cdb, int tipoEntrata)
{
	if (tipoEntrata == 1)
	{
		treni++;
	}
	else if (tipoEntrata == -1)
	{
		treni--;
	}
}

Object^ AreaCriticaCircolare::Clone()
{
	AreaCriticaCircolare^ areaClone = gcnew AreaCriticaCircolare(limite);
	areaClone->treni = treni;
	return areaClone;
}