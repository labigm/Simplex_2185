#include "Octant.h"

using namespace Simplex;

Octant::Octant()
{
	if (!m_pMasterOctant) {
		SetMasterOctant();
	}


}


Octant::~Octant()
{
}


void Octant::SetMasterOctant() {
	m_pMasterOctant = this;
}