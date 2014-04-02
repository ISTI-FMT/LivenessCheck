#pragma once
#include "areacritica.h"
ref class AreaCriticaLineare : public AreaCritica
{
private:
	int treniSinistra;
	int treniDestra;
public:
	AreaCriticaLineare(void) { }
	bool entrataPermessa(int idTreno, int cdb, int tipoEntrata) override;
	void entrata(int idTreno, int cdb, int tipoEntrata) override;
	Object^ Clone() override;
};

