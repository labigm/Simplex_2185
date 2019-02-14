#include "AppClass.h"
void Application::InitVariables(void)
{

	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
	m_pMesh->GenerateCube(1.0f, C_BLACK);
	//create a vector of coordinates to render at
#pragma region Coordinates
	
	coordinates.push_back(vector4(-3.0f, 4.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, 4.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-2.0f, 3.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(2.0f, 3.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-3.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-2.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-1.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-0.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(1.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(2.0f, 2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, 2.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-4.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-3.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-1.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(0.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(1.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, 1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(4.0f, 1.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-5.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-4.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-3.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-2.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-1.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(0.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(1.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(2.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(4.0f, 0.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(5.0f, 0.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-5.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-3.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-2.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-1.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(0.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(1.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(2.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, -1.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(5.0f, -1.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-5.0f, -2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-3.0f, -2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(3.0f, -2.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(5.0f, -2.0f, 0.0f, 1.0f));

	coordinates.push_back(vector4(-2.0f, -3.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(-1.0f, -3.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(1.0f, -3.0f, 0.0f, 1.0f));
	coordinates.push_back(vector4(2.0f, -3.0f, 0.0f, 1.0f));
#pragma endregion

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
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(1.0f,1.0f,1.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	//m_pMesh->Render(m4Projection, m4View, m4Model);
	//Goes through the coordinates vector (initialized in a pragma region above in the InitVariables method) and applies the transformation of each vector by column
	//then adds the movement float
	for (int i = 0; i < coordinates.size();i++) 
	{
		m4Model[3] = coordinates[i]+vector4(value,0.0f,0.0f,0.0f);
		m_pMesh->Render(m4Projection, m4View, m4Model);
	}
	

	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddGridToRenderList();
	
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
	SafeDelete(m_pMesh);
	coordinates.clear();
	//release GUI
	ShutdownGUI();
}