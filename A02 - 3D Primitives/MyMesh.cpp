#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Makes a center point for the base and the "point" of the cone
	std::vector<vector3> points;
	vector3 baseCenter(0.0f, -(a_fHeight/2), 0.0f);
	vector3 heightPoint(0.0f, a_fHeight/2, 0.0f);

	float angle = 360.0f / (float)a_nSubdivisions;

	// Creates points and makes triangles between themselves and the point at the top
	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(baseCenter.x + (a_fRadius * cos((angle * i) * (PI / 180.0))), baseCenter.y, baseCenter.z + (a_fRadius * sin((angle * i) * (PI / 180.0))));
		points.push_back(point);
		if (i != 0) {
			AddTri(point, points[i - 1], baseCenter);
			AddTri(points[i - 1], point, heightPoint);
		}
	}

	AddTri(points[0], points[points.size() - 1], baseCenter);
	AddTri(points[points.size() - 1], points[0], heightPoint);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Creates set of points for the bottom base
	std::vector<vector3> bottomPoints;
	vector3 bottomCenter(0.0f, -(a_fHeight/2), 0.0f);
	vector3 topCenter(0.0f, a_fHeight/2, 0.0f);

	float angle = 360.0f / (float)a_nSubdivisions;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(bottomCenter.x + (a_fRadius * cos((angle * i) * (PI / 180.0))), bottomCenter.y, bottomCenter.z + (a_fRadius * sin((angle * i) * (PI / 180.0))));
		bottomPoints.push_back(point);
		if (i != 0) {
			AddTri(point, bottomPoints[i - 1], bottomCenter);
		}
	}

	AddTri(bottomPoints[0], bottomPoints[bottomPoints.size() - 1], bottomCenter);

	// Creates set of points for the top base
	std::vector<vector3> topPoints;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(topCenter.x + (a_fRadius * cos((angle * i) * (PI / 180.0))), topCenter.y, topCenter.z + (a_fRadius * sin((angle * i) * (PI / 180.0))));
		topPoints.push_back(point);
		if (i != 0) {
			AddTri(topPoints[i - 1], point, topCenter);
			AddQuad(bottomPoints[i - 1], bottomPoints[i], topPoints[i-1], point);
		}
	}

	AddTri(topPoints[topPoints.size() - 1], topPoints[0], topCenter);
	AddQuad(bottomPoints[bottomPoints.size() - 1], bottomPoints[0], topPoints[topPoints.size() - 1], topPoints[0]);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Creates set of points for the bottom outside base
	std::vector<vector3> bottomPoints;
	vector3 bottomCenter(0.0f, -(a_fHeight / 2), 0.0f);
	vector3 topCenter(0.0f, a_fHeight / 2, 0.0f);

	float angle = 360.0f / (float)a_nSubdivisions;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(bottomCenter.x + (a_fOuterRadius * cos((angle * i) * (PI / 180.0))), bottomCenter.y, bottomCenter.z + (a_fOuterRadius * sin((angle * i) * (PI / 180.0))));
		bottomPoints.push_back(point);
	}

	// Creates set of points for the top outside base
	std::vector<vector3> topPoints;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(topCenter.x + (a_fOuterRadius * cos((angle * i) * (PI / 180.0))), topCenter.y, topCenter.z + (a_fOuterRadius * sin((angle * i) * (PI / 180.0))));
		topPoints.push_back(point);
		if (i != 0) {
			AddQuad(bottomPoints[i], bottomPoints[i - 1], point, topPoints[i - 1]);
		}
	}

	AddQuad(bottomPoints[0], bottomPoints[bottomPoints.size() - 1], topPoints[0], topPoints[bottomPoints.size() - 1]);

	// Creates set of points for the bottom inside base
	std::vector<vector3> bottomInnerPoints;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(bottomCenter.x + (a_fInnerRadius * cos((angle * i) * (PI / 180.0))), bottomCenter.y, bottomCenter.z + (a_fInnerRadius * sin((angle * i) * (PI / 180.0))));
		bottomInnerPoints.push_back(point);
		if (i != 0) {
			AddQuad(point, bottomInnerPoints[i - 1], bottomPoints[i], bottomPoints[i - 1]);
		}
	}

	AddQuad(bottomInnerPoints[0], bottomInnerPoints[bottomInnerPoints.size() - 1], bottomPoints[0], bottomPoints[bottomPoints.size() - 1]);

	// Creates set of points for the top inside base
	std::vector<vector3> topInnerPoints;

	for (int i = 0; i < a_nSubdivisions; i++) {
		vector3 point(topCenter.x + (a_fInnerRadius * cos((angle * i) * (PI / 180.0))), topCenter.y, topCenter.z + (a_fInnerRadius * sin((angle * i) * (PI / 180.0))));
		topInnerPoints.push_back(point);
		if (i != 0) {
			AddQuad(topPoints[i], topPoints[i - 1], point, topInnerPoints[i - 1]);
			AddQuad(bottomInnerPoints[i - 1], bottomInnerPoints[i], topInnerPoints[i - 1], point);
		}
	}

	AddQuad(topPoints[0], topPoints[topPoints.size() - 1], topInnerPoints[0], topInnerPoints[topInnerPoints.size() - 1]);
	AddQuad(bottomInnerPoints[bottomInnerPoints.size() - 1], bottomInnerPoints[0], topInnerPoints[topInnerPoints.size() - 1], topInnerPoints[0]);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	std::vector<vector3> bottomPoints;
	std::vector<vector3> topPoints;
	vector3 bottomCenter(0.0f, -(a_fRadius), 0.0f);
	vector3 topCenter(0.0f, a_fRadius, 0.0f);
	vector3 midpoint(0.0f, 0.0f, 0.0f);

	float angle = 360.0f / (float)a_nSubdivisions;

	float dist = a_fRadius / (float)a_nSubdivisions;

	std::vector<vector3> previousLineTop;
	std::vector<vector3> previousLineBottom;

	// Creates points on the sphere from the middle to the poles
	for (int i = 0; i < a_nSubdivisions; i++) {
		for (int j = 0; j < a_nSubdivisions; j++) {
			vector3 pointBottom(midpoint.x + ((a_fRadius - (dist * (i))) * cos((angle * j) * (PI / 180.0))), midpoint.y - (dist * i), midpoint.z + ((a_fRadius - (dist * (i))) * sin((angle * j) * (PI / 180.0))));
			
			float x = pointBottom.x - midpoint.x;
			float y = pointBottom.y - midpoint.y;
			float z = pointBottom.z - midpoint.z;

			float midToPoint = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

			x = (x * a_fRadius) / midToPoint;
			y = (y * a_fRadius) / midToPoint;
			z = (z * a_fRadius) / midToPoint;

			pointBottom = vector3(x, y, z);

			bottomPoints.push_back(pointBottom);
			if (i != 0) {
				if (j != 0) {
					AddQuad(pointBottom, bottomPoints[j - 1], previousLineBottom[j], previousLineBottom[j - 1]);
				}
			}

			vector3 pointTop(midpoint.x + ((a_fRadius - (dist * (i))) * cos((angle * j) * (PI / 180.0))), midpoint.y + (dist * i), midpoint.z + ((a_fRadius - (dist * (i))) * sin((angle * j) * (PI / 180.0))));
			
			x = pointTop.x - midpoint.x;
			y = pointTop.y - midpoint.y;
			z = pointTop.z - midpoint.z;

			midToPoint = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

			x = (x * a_fRadius) / midToPoint;
			y = (y * a_fRadius) / midToPoint;
			z = (z * a_fRadius) / midToPoint;

			pointTop = vector3(x, y, z);
			
			topPoints.push_back(pointTop);
			if (i != 0) {
				if (j != 0) {
					AddQuad(topPoints[j - 1], pointTop, previousLineTop[j - 1], previousLineTop[j]);
				}
			}
		}

		if (i != 0) {
			AddQuad(bottomPoints[0], bottomPoints[bottomPoints.size() - 1], previousLineBottom[0], previousLineBottom[previousLineBottom.size() - 1]);
			AddQuad(topPoints[topPoints.size() - 1], topPoints[0], previousLineTop[previousLineTop.size() - 1], previousLineTop[0]);
		}

		previousLineBottom.clear();
		previousLineTop.clear();
		for (int i = 0; i < bottomPoints.size(); i++) {
			previousLineBottom.push_back(bottomPoints[i]);
			previousLineTop.push_back(topPoints[i]);
		}

		bottomPoints.clear();
		topPoints.clear();
		
	}

	for (int i = 0; i < a_nSubdivisions; i++) {
		if (i != 0) {
			vector3 pointBottom = previousLineBottom[i];
			vector3 pointTop = previousLineTop[i];
			AddTri(previousLineBottom[i - 1], pointBottom, bottomCenter);
			AddTri(pointTop, previousLineTop[i - 1], topCenter);
		}
	}

	AddTri(previousLineBottom[previousLineBottom.size() - 1], previousLineBottom[0], bottomCenter);
	AddTri(previousLineTop[0], previousLineTop[previousLineTop.size() - 1], topCenter);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}