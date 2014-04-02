#pragma once
#using <System.dll>
#include "Aree\AreaCritica.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

ref class MissioneLiveness
{
public:
	property int Trn;
	property array<int>^ Cdbs;
	property List<array<int>^>^ AzioniCdb;
	property int CurrentStep;

	        MissioneLiveness(int trn)
        {
            Trn = trn;
        }

        void MoveNext()
        {
            CurrentStep++;
        }

       
internal:
	 property bool Terminata
        {
            bool get()
            {
                return CurrentStep == Cdbs->Length - 1;
            }
        }
        MissioneLiveness^ Clone();


};

