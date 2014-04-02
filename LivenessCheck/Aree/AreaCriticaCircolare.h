#pragma once
#include "areacritica.h"
ref class AreaCriticaCircolare : public AreaCritica
{
private:
	 int treni;
     int limite;
public:
	AreaCriticaCircolare(int limite)
    {
        this->limite = limite;
    }

    AreaCriticaCircolare(array<int>^ cdbs, int limite)
    {
		this->limite = limite;
    }

	bool entrataPermessa(int idTreno, int cdb, int tipoEntrata) override;
	void entrata(int idTreno, int cdb, int tipoEntrata) override;
	Object^ Clone() override;
};

