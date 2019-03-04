#include "AppClass.h"
#include "MyCamera.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);
	
	//allocate the primitive
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateTorus(3.0f, 2.0f, 6, 6, C_RED);

	//allocate the primitive
	m_pMesh2 = new MyMesh();
	m_pMesh2->GenerateTorus(3.0f, 2.0f, 8, 8, C_GREEN);

	m_pMesh3 = new MyMesh();
	m_pMesh3->GenerateCone(0.5f, 1.0f, 6, C_BLUE);

	//create a new camera
	m_pCamera = new MyCamera();

	//projection used
	m_uProjection = 1;
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
	//Clear the screen
	ClearScreen();
	
	//draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//calculate view and projection
	switch (m_uProjection)
	{
	default:
	case 1:
		m_pCamera->ResetCamera();
		//m_pCamera->SetPositionTargetAndUpward();
		break;
	case 2:
		m_pCamera->ResetCamera();
		m_pCamera->SetPerspective(false);
		break;
	case 3:
		m_pCamera->ResetCamera();
		m_pCamera->SetPositionTargetAndUpward(AXIS_X*25.0f,ZERO_V3,-1.0f*AXIS_Z);
		break;
	case 4:
		m_pCamera->ResetCamera();
		m_pCamera->SetPositionTargetAndUpward(AXIS_Z*-15.0f,ZERO_V3,AXIS_Y);
		break;
	case 5:
		m_pCamera->ResetCamera();
		m_pCamera->SetNearFar(vector2(7.0f,1000.0f));
		m_pCamera->SetPositionTargetAndUpward(AXIS_Z*-15.0f, ZERO_V3, AXIS_Y);
		break;
	case 6:
		m_pCamera->ResetCamera();
		m_pCamera->SetNearFar(vector2(0.001f, 10.0f));
		m_pCamera->SetPositionTargetAndUpward(AXIS_Z*-15.0f, ZERO_V3, AXIS_Y);
		break;
	case 7:
		m_pCamera->ResetCamera();
		m_pCamera->SetPositionTargetAndUpward(AXIS_Z*10.0f,AXIS_Z*-10.0f,-1.0f*AXIS_Y);
		break;
	}

	m_pCamera->CalculateProjectionMatrix();
	m_pCamera->CalculateViewMatrix();

	//draw the primitive
	m_pMesh1->Render(m_pCamera, glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh2->Render(m_pCamera, glm::translate(IDENTITY_M4, vector3(0.0f,0.0f,-5.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh3->Render(m_pCamera, glm::translate(vector3(0.0f, 0.0f, -10.0f)));

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
	//release primitive
	SafeDelete(m_pMesh1);
	SafeDelete(m_pMesh2);
	SafeDelete(m_pMesh3);
	
	//release the camera
	SafeDelete(m_pCamera);
	
	//release GUI
	ShutdownGUI();
}
