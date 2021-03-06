#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));
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

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;
	



	
	// Sets the model's path
	vector3 thisPoint;
	vector3 nextPoint;
	static int path = 0;
	switch (path) {
	case 0:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 1:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 2:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 3:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 4:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 5:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 6:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 7:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 8:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 9:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[path + 1];
		break;
	case 10:
		thisPoint = m_stopsList[path];
		nextPoint = m_stopsList[0];
		break;
	}

	// Percentage of time
	float percent = MapValue(fTimer, 0.0f, 5.0f, 0.0f, 1.0f);

	// Lerp distance between the two points on the path
	v3CurrentPos = glm::lerp(thisPoint, nextPoint, percent);

	// Reset timer if model reached its target
	if (percent >= 1.0f) {
		fTimer = m_pSystem->GetDeltaTime(uClock);
		path++;
		path %= m_stopsList.size();
	}
	


	
	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
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
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}