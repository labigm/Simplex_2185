#include "MyOctant.h"

using namespace Simplex;

MyOctant::MyOctant(uint maxLevel, uint idealChildCount)
{
	Init();
	if (!m_pMasterOctant) {
		SetMasterOctant();
	}


}
MyOctant::MyOctant(MyOctant* master) {

}


MyOctant::~MyOctant()
{
}


void MyOctant::SetMasterOctant() {
	m_pMasterOctant = this;
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

void MyOctant::Display(vector3 a_v3Color) {
	m_pMeshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}

void MyOctant::Init() {
	m_pEntityManager = MyEntityManager::GetInstance();
	m_pMeshManager = MeshManager::GetInstance();
}