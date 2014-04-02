#pragma once

#using <System.dll>
#include "AreaCritica.h"
#include "..\MissioneLiveness.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

ref class StatoAree
{
public:
	property List<AreaCritica^>^ Aree;

	StatoAree(void)
	{
		Aree = gcnew List<AreaCritica^>();
	}

	bool EntrataPermessa(MissioneLiveness^ missione, int idx, int cdb);

	bool Entrata(MissioneLiveness^ missione, int idx, int cdb);

	StatoAree^ Clone();
};

