#include "AppClass.h"
#include <glm/gtx/quaternion.hpp>
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCone(2.0f, 5.0f, 3, C_WHITE);
	
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

	glm::quat orientation = glm::angleAxis(glm::radians(0.0f), vector3(0.0f, 0.0f, 0.0f));
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	glm::quat quat_RotationX = glm::angleAxis(glm::radians(m_v3Rotation.x), vector3(1.0f, 0.0f, 0.0f));
	glm::quat quat_RotationY = glm::angleAxis(glm::radians(m_v3Rotation.y), vector3(0.0f, 1.0f, 0.0f));
	glm::quat quat_RotationZ = glm::angleAxis(glm::radians(m_v3Rotation.z), vector3(0.0f, 0.0f, 1.0f));

	orientation *= quat_RotationX * quat_RotationY*quat_RotationZ;
	
	//m_m4Model = glm::toMat4(quat_RotationX*quat_RotationY*quat_RotationZ);
	m_m4Model = glm::toMat4(orientation);
	m_pMesh->Render(m4Projection, m4View, m_m4Model);

	//m_qOrientation = m_qOrientation * glm::angleAxis(glm::radians(1.0f), vector3(1.0f));
	//m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));
	
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}