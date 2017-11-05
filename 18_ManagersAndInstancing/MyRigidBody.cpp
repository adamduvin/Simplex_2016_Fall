#include "MyRigidBody.h"

Simplex::MyRigidBody::MyRigidBody(MyMesh* a_pObject)
{
	if (a_pObject == nullptr) {
		return;
	}

	std::vector<vector3> pointsList = a_pObject->GetVertexList();
	if (pointsList.size() < 1) {
		return;
	}

	
	m_v3Min = pointsList[0];
	m_v3Max = pointsList[0];
	for (uint i = 1; i < pointsList.size(); ++i) {
		if (pointsList[i].x < v3Min.x)
			v3Min.x = pointsList[i].x;
		else if (pointsList[i].x > v3Max.x)
			v3Max.x = pointsList[i].x;

		if (pointsList[i].y < v3Min.y)
			v3Min.y = pointsList[i].y;
		else if (pointsList[i].y > v3Max.y)
			v3Max.y = pointsList[i].y;

		if (pointsList[i].z < v3Min.z)
			v3Min.z = pointsList[i].z;
		else if (pointsList[i].z > v3Max.z)
			v3Max.z = pointsList[i].z;
	}

	m_v3Center = m_v3Min + m_v3Max;
	m_v3Center = m_v3Center / 2.0f;
	float fRadius = glm::distance(v3Center, v3Max);

	m_BS = new Mesh();
	m_BS->GenerateIcoSphere(fRadius, 5, C_BLUE);

	m_BB = new Mesh();
	m_BB->GenerateCube(fRadius, C_WHITE);
}

void Simplex::MyRigidBody::Render(MyCamera* a_pCamera, matrix4 a_m4Model)
{
	float* matrix = new float[16];
	memcpy(matrix, glm::value_ptr(a_m4Model), 16 * sizeof(float));
	m_BS->RenderWire(a_pCamera->GetProjectionMatrix(), a_pCamera->GetViewMatrix(), matrix, 1);
	vector3 v3Size = m_v3Max - m_v3Min;
	matrix4 resize = a_m4Model * glm::scale(IDENTITY_M4, v3Size);
	m_BB->RenderWire(a_pCamera->GetProjectionMatrix(), a_pCamera->GetViewMatrix(), matrix, 1);
	SafeDelete(matrix);
}
