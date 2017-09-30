#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Adam Duvin - axd3010@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//init the mesh
	//m_pMesh = new MyMesh();
	FormShape();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	

	

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 11; j++) {
			if (positions[i][j] == 1) {
				matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
				static float value = 0.0f;
				matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value - j, -i, 0.0f));
				value += 0.01f;

				//matrix4 m4Model = m4Translate * m4Scale;
				matrix4 m4Model = m4Scale * m4Translate;

				meshMatrix[i][j]->Render(m4Projection, m4View, m4Model);
			}
		}
	}

	
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 11; j++) {
			if (positions[i][j] == 1) {
				SafeDelete(meshMatrix[i][j]);
			}
		}
	}

	//SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}

// Creates all of the mesh objects
void Application::FormShape()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 11; j++) {
			if (positions[i][j] == 1) {
				MyMesh* pixel = new MyMesh();
				meshMatrix[i][j] = pixel;
				meshMatrix[i][j]->GenerateCube(1.0f, C_WHITE);
			}
		}
	}
}
