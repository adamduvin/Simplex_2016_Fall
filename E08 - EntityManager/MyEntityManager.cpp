#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	SafeDelete(m_pInstance);
	m_pInstance = nullptr;
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size; i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			return i;
		}
	}

	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size; i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			return m_entityList[i]->GetModel();
		}
	}

	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size; i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			return m_entityList[i]->GetRigidBody();
		}
	}

	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (int i = 0; i < m_entityList.size; i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			return m_entityList[i]->GetModelMatrix();
		}
	}
	
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* current = nullptr;
	for (int i = 0; i < m_entityList.size; i++) {
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID) {
			current = m_entityList[i];
			break;
		}
	}

	if (current == nullptr) {
		return;
	}



}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{

}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{

}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{

}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{

}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{

}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{

}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{

}