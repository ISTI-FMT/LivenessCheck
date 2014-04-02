#pragma once
#using <System.dll>
#include "StatoTreni.h"
#include "Movimento.h"
#include "Aree\StatoAree.h"

using namespace System;
using namespace System::Collections::Generic;

ref class Liveness
{
private:
	 StatoAree^ statoAree;
     StatoTreni^ statoTreni;
public:
	Liveness(array<int>^ limitiAree);
	void AggiungiMissione(MissioneLiveness^ missione);
	void RimuoviMissione(int trn);
	void MuoviTreno(int trn, int cdb);
	System::Collections::Generic::Stack<Movimento^>^ CheckLiveness(int trn, int cdb, bool consideraAreeCritiche);
	static bool CheckLiveness(StatoTreni^ statoTreni, StatoAree^ statoAree, System::Collections::Generic::Stack<Movimento^>^ sequenza, System::Collections::Generic::HashSet<StatoTreni^>^ visitati, bool consideraAreeCritiche);
	static System::Collections::Generic::Stack<Movimento^>^ CheckLiveness(StatoTreni^ statoTreni, StatoAree^ statoAree, bool consideraAreeCritiche);

};

