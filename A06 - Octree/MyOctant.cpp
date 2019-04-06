#include "MyOctant.h"

using namespace Simplex;

MyOctant::MyOctant(uint maxLevel, uint idealChildCount)
{
	Init();
	if (!m_pMasterOctant) {
		SetMasterOctant();
	}
	m_uMaxDepth = maxLevel;
	m_uIdealChildren = idealChildCount;
	m_lPopulated.push_back(this);

}
MyOctant::MyOctant(MyOctant* master) {
	Init();
	this->m_uMaxDepth = master->m_uMaxDepth;
	this->m_uIdealChildren = master->m_uIdealChildren;


}

vector3 MyOctant::GetCenterGlobal() {
	return this->m_v3Center;
}

vector3 MyOctant::GetMaxGlobal() {
	return this->m_v3Max;
}

vector3 MyOctant::GetMinGlobal() {
	return this->m_v3Min;
}


MyOctant::MyOctant() {

}


MyOctant::~MyOctant()
{
}


void MyOctant::SetMasterOctant() {
	m_pMasterOctant = this;
	this->m_uLevel = 0;
	CreateMasterSize();
}
void MyOctant::CreateMasterSize() {
	uint ObjectCount = m_pEntityManager->GetEntityCount();
	vector3 Max = m_pEntityManager->GetRigidBody(m_pEntityManager->GetUniqueID(0))->GetMaxGlobal();//set the max to the max of the first object
	vector3 Min = m_pEntityManager->GetRigidBody(m_pEntityManager->GetUniqueID(0))->GetMinGlobal();//set the min to the min of the first object
	for (int i = ObjectCount-1; i >0;i--) {
		m_lEntityList.push_back(i);
		String ID = m_pEntityManager->GetUniqueID(i);
		vector3 maxTemp = m_pEntityManager->GetRigidBody(ID)->GetMaxGlobal();
		vector3 minTemp = m_pEntityManager->GetRigidBody(ID)->GetMinGlobal();
		
		
		if (maxTemp.x > Max.x) {
			Max.x = maxTemp.x;
		}
		else if (minTemp.x < Min.x) {
			Min.x = minTemp.x;
		}


		if (maxTemp.y > Max.y) {
			Max.y = maxTemp.y;
		}
		else if (minTemp.y < Min.y) {
			Min.y = minTemp.y;
		}


		if (maxTemp.z > Max.z) {
			Max.z = maxTemp.z;
		}
		else if (minTemp.z < Min.z) {
			Min.z = minTemp.z;
		}

	}
	m_v3Max = Max;
	m_v3Min = Min;
	m_v3Center = (Max + Min) / 2.0f;
	m_fSize = Max.x - Min.x;

}

void MyOctant::Subdivide() {
//The children are arranged according to the cube model below, with each number representing the index of the cube that corner belongs to.
//      1-----3   
//	   /|    /|
//	  / 5---/-7
//   0-----2 /
//   |/    |/
//   4-----6

	if(IsLeaf()){
		m_pMasterOctant->m_uOctantCount += 8;
		if (this->m_uLevel != m_uMaxDepth) {
			this->m_uChildren = 8;
			m_pMasterOctant->currentDepth++;
			MyOctant* child0 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child1 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child2 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child3 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child4 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child5 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child6 = new MyOctant(this->m_pMasterOctant);
			MyOctant* child7 = new MyOctant(this->m_pMasterOctant);

			//global maximums of children
			child0->m_v3Max = vector3(this->GetCenterGlobal().x, this->GetMaxGlobal().y, this->GetCenterGlobal().z);
			child1->m_v3Max = vector3(this->GetCenterGlobal().x, this->GetMaxGlobal().y, this->GetMaxGlobal().z);
			child2->m_v3Max = vector3(this->GetMaxGlobal().x, this->GetMaxGlobal().y, this->GetCenterGlobal().z);
			child3->m_v3Max = vector3(this->GetMaxGlobal());
			child4->m_v3Max = vector3(this->GetCenterGlobal());
			child5->m_v3Max = vector3(this->GetCenterGlobal().x, this->GetCenterGlobal().y, this->GetMaxGlobal().z);
			child6->m_v3Max = vector3(this->GetMaxGlobal().x, this->GetCenterGlobal().y, this->GetCenterGlobal().z);
			child7->m_v3Max = vector3(this->GetMaxGlobal().x, this->GetCenterGlobal().y, this->GetMaxGlobal().z);

			//global minimums of children
			child0->m_v3Min = vector3(this->GetMinGlobal().x, this->GetCenterGlobal().y, this->GetMinGlobal().z);
			child1->m_v3Min = vector3(this->GetMinGlobal().x, this->GetCenterGlobal().y, this->GetCenterGlobal().z);
			child2->m_v3Min = vector3(this->GetCenterGlobal().x, this->GetCenterGlobal().y, this->GetMinGlobal().z);
			child3->m_v3Min = vector3(this->GetCenterGlobal());
			child4->m_v3Min = vector3(this->GetMinGlobal());
			child5->m_v3Min = vector3(this->GetMinGlobal().x, this->GetMinGlobal().y, this->GetCenterGlobal().z);
			child6->m_v3Min = vector3(this->GetCenterGlobal().x, this->GetMinGlobal().y, this->GetMinGlobal().z);
			child7->m_v3Min = vector3(this->GetCenterGlobal().x, this->GetMinGlobal().y, this->GetCenterGlobal().z);


			m_pChildren[0] = child0;
			m_pChildren[1] = child1;
			m_pChildren[2] = child2;
			m_pChildren[3] = child3;
			m_pChildren[4] = child4;
			m_pChildren[5] = child5;
			m_pChildren[6] = child6;
			m_pChildren[7] = child7;

			for (int i = 0; i < 8; i++) {
				m_lPopulated.push_back(m_pChildren[i]);
				m_pChildren[i]->m_pMasterOctant = this->m_pMasterOctant;
				m_pChildren[i]->m_pParent = this;
				m_pChildren[i]->m_uLevel = this->m_uLevel + 1;
				m_pChildren[i]->m_fSize = this->m_fSize / 2.0f;
				m_pChildren[i]->m_v3Center = (m_pChildren[i]->GetMaxGlobal() + m_pChildren[i]->GetMinGlobal()) / 2.0f;
				FindObjectsWithinMe();
			}
		}

	}
	else {
		for (int i = 0; i < 8; i++) {
			m_pChildren[i]->Subdivide();
		}
	}
}
void MyOctant::Display(vector3 a_v3Color) {
	m_pMeshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}

void MyOctant::DisplayLeaves(vector3 a_v3Color) {
	//m_lEntityList.clear();
	//FindObjectsWithinMe();
	if (m_lEntityList.size() > 0&&this->m_uLevel==m_pMasterOctant->currentDepth) {
		for (int i = 0; i < m_lEntityList.size()-1; i++) {
			for (int j = i+1; j < m_lEntityList.size(); j++) {
				IsColliding(i, j);

			}
		}
		
	}
	if (IsLeaf()) {
		Display(a_v3Color);
	}
	else {
		for (int i = 0; i < 8; i++) {
			m_pChildren[i]->DisplayLeaves(a_v3Color);
			m_pMeshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
		}
	}
}

bool MyOctant::IsColliding(uint thisObject, uint other ) {
	MyRigidBody* thisEntity = m_pEntityManager->GetRigidBody(m_pEntityManager->GetUniqueID(thisObject));
	MyRigidBody* otherEntity = m_pEntityManager->GetRigidBody(m_pEntityManager->GetUniqueID(other));
	return thisEntity->IsColliding(otherEntity);

}

void MyOctant::ConstructPopulatedList() {
	

}

bool MyOctant::IsLeaf() {
	if (m_uChildren == 0) {
		return true;
	}
	return false;
}

uint MyOctant::GetOctantCount() {
	return m_pMasterOctant->m_uOctantCount;
}

void MyOctant::FindObjectsWithinMe() {
	uint ObjectCount = m_pEntityManager->GetEntityCount();
	for (int i = ObjectCount - 1; i > 0; i--) {
		m_lEntityList.push_back(i);
		String ID = m_pEntityManager->GetUniqueID(i);
		vector3 maxTemp = m_pEntityManager->GetRigidBody(ID)->GetMaxGlobal();
		vector3 minTemp = m_pEntityManager->GetRigidBody(ID)->GetMinGlobal();
		if (minTemp.x > GetMaxGlobal().x) {
			m_lEntityList.pop_back();
		}
		else if (minTemp.y > GetMaxGlobal().y) {
			m_lEntityList.pop_back();
		}
		else if (minTemp.z > GetMaxGlobal().z) {
			m_lEntityList.pop_back();
		}
		else if (maxTemp.x < GetMinGlobal().x) {
			m_lEntityList.pop_back();
		}
		else if (maxTemp.y < GetMinGlobal().y) {
			m_lEntityList.pop_back();
		}
		else if (maxTemp.z < GetMinGlobal().z) {
			m_lEntityList.pop_back();
		}

	}
}

void MyOctant::Init() {
	m_pEntityManager = MyEntityManager::GetInstance();
	m_pMeshManager = MeshManager::GetInstance();
}