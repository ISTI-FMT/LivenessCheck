#include "stdafx.h"
#include "Liveness.h"
#include "Aree\statoAree.h"
#include "Aree\AreaCritica.h"
#include "Aree\AreaCriticaLineare.h"
#include "Aree\AreaCriticaCircolare.h"

using namespace System;
using namespace System::Collections::Generic;



namespace LivenessCheck
{
	Liveness::Liveness(array<int>^ limitiAree)
	{
		statoAree = gcnew StatoAree();
		statoTreni = gcnew StatoTreni();

		AreaCritica^ area;
		for each (int limiteArea in limitiAree)
		{
			if (limiteArea == 0)
			{
				area = gcnew AreaCriticaLineare();
			}
			else
			{
				area = gcnew AreaCriticaCircolare(limiteArea);
			}
			statoAree->Aree->Add(area);
		}
	}

	void Liveness::AggiungiMissione(MissioneLiveness^ missione)
	{
		//Cerco se esiste già una missione con questo TRN
		MissioneLiveness^  missioneEsistente = nullptr;
		for each (MissioneLiveness^ m in  statoTreni->Missioni)
		{
			if (m->Trn == missione->Trn)
			{
				missioneEsistente = m;
				break;
			}
		}
		//Se esiste già una missione con questo TRN, la sostituisco con quella nuova
		if (missioneEsistente != nullptr)
		{
			statoTreni->Missioni->Remove(missioneEsistente);
		}
		statoTreni->Missioni->Add(missione);
	}

	void Liveness::RimuoviMissione(int trn)
	{
		MissioneLiveness^  missioneEsistente = nullptr;
		for each (MissioneLiveness^ m in  statoTreni->Missioni)
		{
			if (m->Trn == trn)
			{
				missioneEsistente = m;
				break;
			}
		}

		if (missioneEsistente != nullptr)
		{
			statoTreni->Missioni->Remove(missioneEsistente);
		}
	}

	void Liveness::MuoviTreno(int trn, int cdb)
	{
		statoTreni->Muovi(trn, cdb);
	}

	/// <summary>
	/// Se esiste, Ritorna una sequenza di movimenti che porta tutte le missioni a destinazione
	/// Altrimenti, ritorna nullptr
	/// </summary>
	/// <param name="limitiAree">lista dei limiti delle aree. Zero per aree lineari, maggiore di zero per aree circolari</param>
	/// <param name="missioniLiveness">Lista delle missioni dei treni</param>
	/// <param name="consideraAreeCritiche">
	/// TRUE se il check deve considerare anche i limiti delle aree critiche. FALSE altrimenti.
	/// Considerando i limiti delle aree critiche il check è più veloce ma può portare a falsi negativi.
	/// </param>
	System::Collections::Generic::Stack<Movimento^>^ Liveness::LivenessCheck(int trn, int cdb, bool consideraAreeCritiche)
	{
		StatoTreni^ stato2 = statoTreni->Clone();
		stato2->Muovi(trn, cdb);

		return LivenessCheck(stato2, statoAree, consideraAreeCritiche);
	}

	bool Liveness::LivenessCheck(StatoTreni^ statoTreni, StatoAree^ statoAree, System::Collections::Generic::Stack<Movimento^>^ sequenza, System::Collections::Generic::HashSet<StatoTreni^>^ visitati, bool consideraAreeCritiche)
	{
		if (visitati->Contains(statoTreni))
		{
			return false;
		}

		bool res = false;
		bool cannotAdvance = true;
		for (int i = 0; i < statoTreni->Missioni->Count; i++)
		{
			MissioneLiveness^missione = statoTreni->Missioni[i];
			if (missione->Terminata)
			{
				Movimento^ movement = gcnew Movimento(missione->Trn, missione->Cdbs[missione->CurrentStep]);
				if (!sequenza->Contains(movement))
				{
					sequenza->Push(movement);
				}
			}
			else
			{
				int cdbCorrente = missione->Cdbs[missione->CurrentStep];
				int cdbNext = missione->Cdbs[missione->CurrentStep + 1];

				bool evolving = true;
				for (int j = 0; j < statoTreni->Missioni->Count; j++)
				{
					if (j != i)
					{
						MissioneLiveness^missione2 = statoTreni->Missioni[j];
						int cdbCorrente2 = missione2->Cdbs[missione2->CurrentStep];
						if (cdbNext == cdbCorrente2)
						{
							//il prossimo cdb è occupato, non posso avanzare
							evolving = false;
							break;
						}
					}
				}

				if (evolving)
				{
					if (consideraAreeCritiche)
					{
						//controllo vincoli aree critich
						if (!statoAree->EntrataPermessa(missione, missione->CurrentStep + 1, cdbNext))
						{
							evolving = false;
						}
					}
				}

				if (evolving)
				{
					cannotAdvance = false;

					StatoTreni^ stato2 = statoTreni->Clone();
					stato2->Missioni[i]->MoveNext();

					StatoAree^ aree2 = nullptr;
					if (consideraAreeCritiche)
					{
						aree2 = statoAree->Clone();
						aree2->Entrata(stato2->Missioni[i], stato2->Missioni[i]->CurrentStep, cdbNext);
					}

					//se esiste un path che porta alla fine non vado oltre
					if (LivenessCheck(stato2, aree2, sequenza, visitati, consideraAreeCritiche))
					{
						sequenza->Push(gcnew Movimento(missione->Trn, missione->Cdbs[missione->CurrentStep]));
						res = true;
						break;
					}
					else
					{
						sequenza->Clear();
					}
				}
			}
		}

		//Non posso più andare oltre (tutti i treni hanno evolving a false)
		//Controllo se il motivo è un deadlock o se tutte le missioni sono terminate
		if (cannotAdvance)
		{
			bool final = true;
			for each (MissioneLiveness^ missione in statoTreni->Missioni)
			{
				if (!missione->Terminata)
				{
					final = false;
					break;
				}
			}

			if (final)
			{
				res = true;
			}
		}
		if (!res)
		{
			visitati->Add(statoTreni);
		}
		return res;
	}

	/// <summary>
	/// Ritorna una sequenza di coppie (Trn,Cdb), se esiste, che porta ad uno stato finale senza deadlock
	/// Se la sequenza non esiste ritorna nullptr
	/// </summary>
	/// <param name="stato">lo stato dei treni</param>
	/// <param name="sequenza">sequenza dei movimenti effettuati dai treni (contiene i TRN dei treni mossi)</param>
	System::Collections::Generic::Stack<Movimento^>^ Liveness::LivenessCheck(StatoTreni^ statoTreni, StatoAree^ statoAree, bool consideraAreeCritiche)
	{
		System::Collections::Generic::Stack<Movimento^>^ sequenza = gcnew System::Collections::Generic::Stack<Movimento^>();
		System::Collections::Generic::HashSet<StatoTreni^>^ visitati = gcnew System::Collections::Generic::HashSet<StatoTreni^>();

		if (LivenessCheck(statoTreni, statoAree, sequenza, visitati, consideraAreeCritiche))
		{
			return sequenza;
		}
		else
		{
			return nullptr;
		}
	}

}