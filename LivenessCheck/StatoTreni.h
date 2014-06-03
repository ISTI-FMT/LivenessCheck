#pragma once
#using <System.dll>
#include "MissioneLiveness.h"

using namespace System;
using namespace System::Collections::Generic;

namespace LivenessCheck
{
	public ref class StatoTreni
	{
	public:
		property List<MissioneLiveness^>^ Missioni;
		StatoTreni(void)

		{
			Missioni = gcnew List<MissioneLiveness^>();
		}


		StatoTreni^ Clone();

		virtual bool Equals(Object^ obj) override;

		virtual String^ ToString() override;

		virtual int GetHashCode() override
		{
			return GetHashCode(Missioni);
		}

		void Muovi(int trn, int cdb);

		static int GetHashCode(List<MissioneLiveness^>^ values);

	};
}