#include "stdafx.h"
#include "StatoTreni.h"


int StatoTreni::GetHashCode(List<MissioneLiveness^>^ values)
{
	int result = 0;
	int shift = 0;
	for (int i = 0; i < values->Count; i++)
	{
		shift = (shift + 11) % 21;
		result ^= (values[i]->CurrentStep + 1024) << shift;
	}
	return result;
}

void StatoTreni::Muovi(int trn, int cdb)

{
	MissioneLiveness^  missione = nullptr;
	for each (MissioneLiveness^ m in Missioni)
	{
		if (m->Trn == trn)
		{
			missione = m;
			break;
		}
	}

	if (missione != nullptr)
	{
		int idx = Array::IndexOf(missione->Cdbs, cdb);
		if (idx >= 0)
		{
			missione->CurrentStep = idx;
		}
	}
}

StatoTreni^ StatoTreni::Clone()
{
	StatoTreni^ clone = gcnew StatoTreni();
	for each (MissioneLiveness^ missione in Missioni)
	{
		clone->Missioni->Add(missione->Clone());
	}

	return clone;
}

bool StatoTreni::Equals(Object^ obj)
{
	StatoTreni^ stato2 = (StatoTreni^)obj;
	return GetHashCode() == stato2->GetHashCode();           
}

String^ StatoTreni::ToString()
{
	array<int>^ steps = gcnew array<int>(Missioni->Count);
	for (int i = 0; i < steps->Length; i++)
	{
		steps[i] = Missioni[i]->CurrentStep;
	}
	return String::Join(",", steps);
}
