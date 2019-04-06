#ifndef _MYOCTANT_
#define _MYOCTANT_
#include "MyEntityManager.h"


namespace Simplex {
	class MyOctant
	{
		MyOctant* m_pMasterOctant;//root octant
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

		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChildren[8];

		std::vector<uint> m_lEntityList;
		std::vector<MyOctant*> m_lPopulated;

		void SetMasterOctant();
		void CreateMasterSize();
		void Release();
		void Init();
		void ConstructPopulatedList();


	public:
		//This constructor is the first one called, constructs the master octant. Do not call more than once within the same tree
		MyOctant(uint maxLevel = 2, uint idealChildCount = 5);
		
		//This constructor is used to subdivide and create child octants
		MyOctant::MyOctant(MyOctant* master);
		
		//destructor
		~MyOctant();
		
		//Copy constuctor
		MyOctant(MyOctant const& other);
		
		//Copy assignment operator
		MyOctant& operator=(MyOctant const& other);
		
		//gets the size of the octant
		float GetSize();

		//gets the global center of the octant
		vector3 GetCenterGlobal();
		
		//gets the global minimum corner
		vector3 GetMinGlobal();

		//gets the global maximum corner
		vector3 GetMaxGlobal();


		bool IsColliding(uint a_RBIndex);
		void Display(vector3 a_v3Color = C_GREEN);
		void DisplayLeaves(vector3 a_v3Color = C_GREEN);
		void ClearEntityList();
		void Subdivide();
		MyOctant* GetChild(uint a_nChild);
		MyOctant* GetParent();
		bool IsLeaf();
		bool ContainsMoreThan(uint a_nEntities);
		void KillChildren();
		void ConstructTree(uint a_nMaxLevel=3);
		void AssignIDtoEntity();
		uint GetOctantCount();

	};
}

#endif // !_MYOCTANT_