#ifndef _MYOCTANT_
#define _MYOCTANT_
#include "MyEntityManager.h"


namespace Simplex {
	class MyOctant
	{
		MyOctant* m_pMasterOctant;//root octant
		
		


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

		void SetMasterOctant();//sets the root octant
		void CreateMasterSize();//sets the proper dimensions of the root octant
		void Init();


	public:
		uint m_uOctantCount = 0;//number of octants existing
		uint m_uMaxDepth;//how deep it can go
		uint m_uIdealChildren;//used to signal need for subdivision
		uint currentDepth = 0;//the current depth of the tree
		uint DisplayLevel=currentDepth;

		//This constructor is the first one called, constructs the master octant. Do not call more than once within the same tree
		MyOctant(uint maxLevel = 2, uint idealChildCount = 5);
		
		//This constructor is used to subdivide and create child octants
		MyOctant(MyOctant* master);
		

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

		//checks collision between two octants
		bool IsColliding(uint a_RBIndex, uint other);
		
		//displays this octant
		void Display(vector3 a_v3Color = C_GREEN);
		
		//displays the children of this octant
		void DisplayLeaves(vector3 a_v3Color = C_GREEN);
		
		//subdivides this octant
		void Subdivide();
		
		//tests if this node has children
		bool IsLeaf();
		
		//kills all children of this node
		void KillChildren();
		
		//kills all children below the given level. Called on root node
		void KillChildrenAtLevel(uint level);
		
		//gets the number of octants in the tree
		uint GetOctantCount();
		
		//populates the entity list with the entities within this octant
		void FindObjectsWithinMe();

	};
}

#endif // !_MYOCTANT_