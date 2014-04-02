#include "stdafx.h"
#include "MissioneLiveness.h"

MissioneLiveness^ MissioneLiveness::Clone()
{
	MissioneLiveness^ clone = gcnew MissioneLiveness(Trn);
	clone->Cdbs = Cdbs;
	clone->AzioniCdb = AzioniCdb;
	clone->CurrentStep = CurrentStep;

	return clone;
}