#ifndef _OCTANT
#define _OCTANT
#include "MyEntityManager.h"


namespace Simplex {
	class Octant
	{
		static Octant* m_pMasterOctant;//root octant
		static uint m_uOctantCount;//number of octants existing
		static uint m_uMaxDepth;//how deep it can go
		static uint m_uIdealChildren;//used to signal need for subdivision

		MeshManager* m_pMeshManager = nullptr;
		MyEntityManager* m_pEntityManager = nullptr;
		
		uint m_uID = 0;
		uint m_uLevel = 0;
		uint m_uChildren = 0;

		float m_fSize = 0.0f;

		vector3 m_v3Center = vector3(0.0f); //Will store the center point of the octant
		vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the octant
		vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the octant

		Octant* m_pParent = nullptr;
		Octant* m_pChildren[8];

		std::vector<uint> m_lEntityList;
		std::vector<Octant*> m_lPopulated;

		void SetMasterOctant();

	public:
		Octant();
		~Octant();
		Octant(Octant const& other);
		Octant& operator=(Octant const& other);
		float GetSize();
		vector3 GetCenterGlobal();
		vector3 GetMinGlobal();
		vector3 GetMaxGlobal();


	};
}

#endif // !_OCTANT