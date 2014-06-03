#pragma once
#using <System.dll>
#include "StatoTreni.h"
#include "Movimento.h"
#include "Aree\StatoAree.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LivenessCheck
{
	public ref class Liveness
	{
	private:
		 StatoAree^ statoAree;
		 StatoTreni^ statoTreni;
		 static bool LivenessCheck(StatoTreni^ statoTreni, StatoAree^ statoAree, System::Collections::Generic::Stack<Movimento^>^ sequenza, System::Collections::Generic::HashSet<StatoTreni^>^ visitati, bool consideraAreeCritiche);
		static System::Collections::Generic::Stack<Movimento^>^ LivenessCheck(StatoTreni^ statoTreni, StatoAree^ statoAree, bool consideraAreeCritiche);
	public:
		Liveness(array<int>^ limitiAree);
		void AggiungiMissione(MissioneLiveness^ missione);
		void RimuoviMissione(int trn);
		void MuoviTreno(int trn, int cdb);
		System::Collections::Generic::Stack<Movimento^>^ LivenessCheck(int trn, int cdb, bool consideraAreeCritiche);
	};
}
