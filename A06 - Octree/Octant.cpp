#include "Octant.h"
using namespace Simplex;

Simplex::Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
}

Simplex::Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
}

//Simplex::Octant::Octant(Octant const & other)
//{
//}
//
//Octant & Simplex::Octant::operator=(Octant const & other)
//{
//	// TODO: insert return statement here
//}

Simplex::Octant::~Octant(void)
{
	Release();
}

//void Simplex::Octant::Swap(Octant & other)
//{
//}

float Simplex::Octant::GetSize(void)
{
	return m_fSize;
}

vector3 Simplex::Octant::GetCenterGlobal(void)
{
	return m_v3Center;
}

vector3 Simplex::Octant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 Simplex::Octant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool Simplex::Octant::IsColliding(uint a_uRBIndex)
{
	bool colliding = true;
	MyEntity* other = m_pEntityMngr->GetEntity(a_uRBIndex);
	if (m_v3Max.x < other->GetRigidBody()->GetMinGlobal().x) //this to the right of other
		colliding = false;
	if (m_v3Min.x > other->GetRigidBody()->GetMaxGlobal().x) //this to the left of other
		colliding = false;

	if (m_v3Max.y < other->GetRigidBody()->GetMinGlobal().y) //this below of other
		colliding = false;
	if (m_v3Min.y > other->GetRigidBody()->GetMaxGlobal().y) //this above of other
		colliding = false;

	if (m_v3Max.z < other->GetRigidBody()->GetMinGlobal().z) //this behind of other
		colliding = false;
	if (m_v3Min.z > other->GetRigidBody()->GetMaxGlobal().z) //this in front of other
		colliding = false;

	return colliding;
}

void Simplex::Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	
}

void Simplex::Octant::Display(vector3 a_v3Color)
{
}

void Simplex::Octant::DisplayLeafs(vector3 a_v3Color)
{
}

void Simplex::Octant::ClearEntityList(void)
{
}

void Simplex::Octant::Subdivide(void)
{
}

Octant * Simplex::Octant::GetChild(uint a_nChild)
{
	return nullptr;
}

Octant * Simplex::Octant::GetParent(void)
{
	return nullptr;
}

bool Simplex::Octant::IsLeaf(void)
{
	return false;
}

bool Simplex::Octant::ContainsMoreThan(uint a_nEntities)
{
	return false;
}

void Simplex::Octant::KillBranches(void)
{
}

void Simplex::Octant::ConstructTree(uint a_nMaxLevel)
{
}

void Simplex::Octant::AssignIDtoEntity(void)
{
}

uint Simplex::Octant::GetOctantCount(void)
{
	return uint();
}

void Simplex::Octant::Release(void)
{
}

void Simplex::Octant::Init(void)
{
}

void Simplex::Octant::ConstructList(void)
{
}
