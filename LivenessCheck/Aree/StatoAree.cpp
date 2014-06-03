#include "stdafx.h"
#include "StatoAree.h"
#include "..\MissioneLiveness.h"

namespace LivenessCheck
{
	bool StatoAree::EntrataPermessa(MissioneLiveness^ missione, int idx, int cdb)
	{
		bool entrataValida = true;

		array<int>^ azioni = missione->AzioniCdb[idx];

		for (int i = 0; i < Aree->Count; i++)
		{
			int azione = azioni[i];

			if (azione != 0)
			{
				AreaCritica^ area = Aree[i];
				if (!area->entrataPermessa(missione->Trn, cdb, azione))
				{
					entrataValida = false;
					break;
				}
			}
		}

		return entrataValida;
	}

	bool StatoAree::Entrata(MissioneLiveness^ missione, int idx, int cdb)
	{
		bool entrataValida = true;
		array<int>^ azioni = missione->AzioniCdb[idx];

		for (int i = 0; i < Aree->Count; i++)
		{
			int azione = azioni[i];
			if (azione != 0)
			{
				AreaCritica^ area = Aree[i];
				area->entrata(missione->Trn, cdb, azione);
			}
		}
		return entrataValida;
	}

	StatoAree^ StatoAree::Clone()
	{
		StatoAree^ areeClone = gcnew StatoAree();
		for each (AreaCritica^ area in Aree)
		{
			AreaCritica^ areaClone = (AreaCritica^)area->Clone();
			areeClone->Aree->Add(areaClone);
		}
		return areeClone;
	}
}