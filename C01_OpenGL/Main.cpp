#include "Main.h"
//Callbacks
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Callbacks for winapi 
	switch (msg)
	{
	case WM_CLOSE: // if the close message was sent
		DestroyWindow(hwnd); //destroy the window
		break;
	case WM_DESTROY:// if the destroy message was sent
		PostQuitMessage(0); //post the quit message to the message manager
		break;
	case WM_KEYUP:
		//if escape
		if (wParam == 27)
		{
			PostQuitMessage(0); //post the quit message to the message manager
		}
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			std::cout << wParam;
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}
	return 0; //end the program

}

// Shader sources
const GLchar* vertexShader = R"glsl(
	#version 330
	in vec3 positionBuffer;
	void main()
	{
		gl_Position = vec4( positionBuffer, 1.0 );
	}
)glsl";
const GLchar* fragmentShader = R"glsl(
	#version 330
	out vec4 fragment;
	void main()
	{
		fragment = vec4( 0.0, 1.0, 0.0, 1.0 );
		return;
	}
)glsl";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
//int main(void)
{
#pragma region Window Initialization
	if (AllocConsole())
	{
		FILE* stream;
		_wfreopen_s(&stream, TEXT("CONIN$"), TEXT("rb"), stdin);
		_wfreopen_s(&stream, TEXT("CONOUT$"), TEXT("wb"), stdout);
		_wfreopen_s(&stream, TEXT("CONOUT$"), TEXT("wb"), stderr);
	}

	//Set the value to the provided arguments

	WNDCLASSEX wc; //Window Information Container
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(wc.hInstance, (LPCTSTR)IDI_ICON);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"MEWINDOW";
	wc.hIconSm = LoadIcon(wc.hInstance, (LPCTSTR)IDI_ICON);

	//Register the container
	if (!RegisterClassEx(&wc))
	{
		//If this fails we have no window and we should exit the program
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
		return E_FAIL;
	}

	//Setting window stile accordingly
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
	DWORD dwWindowStyle = WS_OVERLAPPEDWINDOW; // Window Borderless Style

	RECT rc = { (long)0, (long)0, (long)600, (long)600 };// Grabs Rectangle Upper Left / Lower Right Values
	AdjustWindowRectEx(&rc, dwWindowStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	LPCWSTR m_sWindowName = L"Window"; //Name of the window
																	//Create the window and store the returning handler in our window handler
	HWND m_hWindowHandler = CreateWindowEx(	dwExStyle,
											L"MEWINDOW",
											m_sWindowName,
											dwWindowStyle |
											WS_CLIPSIBLINGS |
											WS_CLIPCHILDREN,
											100, 100,
											600, 600,
											NULL, NULL,
											hInstance,
											NULL);

	//If it failed...
	if (m_hWindowHandler == NULL)
	{
		//Inform the user and return fail
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		exit(0);
		return E_FAIL;
	}

	ShowWindow(m_hWindowHandler, nCmdShow); //Show the window
	UpdateWindow(m_hWindowHandler); //Update the window
#pragma endregion
	//---------------------------------------------------------------------------------------------------------
#pragma region OpenGL Initialization
	HDC   m_hDC = NULL;			// Private GDI Device Context
	if (!(m_hDC = GetDC(m_hWindowHandler)))				// Did We Get A Device Context?
	{
		MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =	// pfd tells the window how we want render to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
		1,								// Version Number
		PFD_DRAW_TO_WINDOW |			// Format Must Support Window
		PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,				// Must Support Double Buffering
		PFD_TYPE_RGBA,					// Request An RGBA Format
		16,								// Select Our Color Depth
		0, 0, 0, 0, 0, 0,				// Color Bits Ignored
		0,								// No Alpha Buffer
		0,								// Shift Bit Ignored
		0,								// No Accumulation Buffer
		0, 0, 0, 0,						// Accumulation Bits Ignored
		32,								// 32Bit Z-Buffer (Depth Buffer)  
		0,								// No Stencil Buffer
		0,								// No Auxiliary Buffer
		PFD_MAIN_PLANE,					// Main Drawing Layer
		0,								// Reserved
		0, 0, 0							// Layer Masks Ignored
	};

	GLuint PixelFormat;								// Holds The Results After Searching For A Match
	if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;	// failure
	}

	if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;	// failure
	}

	HGLRC hRC = NULL;					// Permanent Rendering Context
	if (!(hRC = wglCreateContext(m_hDC)))	// Are We Able To Get A Rendering Context?
	{
		MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;	// failure
	}

	//Create a temporal context to initialize OpenGL, then try to update it to "modern" OpenGL
	HGLRC tempHRC = wglCreateContext(m_hDC);	// Create an OpenGL 2.1 context for our device context
	if (!wglMakeCurrent(m_hDC, tempHRC))			// Try To Activate The Rendering Context
	{
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;	// failure
	}

	int nMajor = 4, nMinor = 5;
	int attributes[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, nMajor, // Set the major version of OpenGL
		WGL_CONTEXT_MINOR_VERSION_ARB, nMinor, // Set the minor version of OpenGL
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
		0
	};
	//version and major where available
	if (wglewIsSupported("WGL_ARB_create_context") == 1) // If the OpenGL 3.x context creation extension is available
	{
		hRC = wglCreateContextAttribsARB(m_hDC, NULL, attributes); //Try to create a context with the major an minor version specified
		while (hRC == NULL && nMajor > 2) //if it fails it will decrement the minor version, once it reaches 0 it will decrement major
		{
			nMinor--;
			if (nMinor < 0)
			{
				nMajor--;
				nMinor = 9;
			}
			attributes[1] = nMajor;
			attributes[3] = nMinor;

			hRC = wglCreateContextAttribsARB(m_hDC, NULL, attributes); // Create and OpenGL 4.5 context based on the given attributes
		}
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
		wglDeleteContext(tempHRC); // Delete the temporary context (OpenGL 2.1)
		wglMakeCurrent(m_hDC, hRC); // Make our context current (Whatever context was created)
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return FALSE;	// failure
	}

	int glVersion[2] = { -1, -1 }; // Set some values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL major version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL minor version we are using

	std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
#pragma endregion
	//---------------------------------------------------------------------------------------------------------
#pragma region Shader Initialization
	// Create and compile the vertex shader
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderID);

	// Create and compile the fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderID);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	//glBindFragDataLocation(shaderProgramID, 0, "fragment");
	glLinkProgram(shaderProgramID);
	//Check the program
	GLint result = GL_FALSE;
	GLint log = GL_FALSE;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &log);
	if (log > 0) {
		std::vector<char> ProgramErrorMessage(log + 1);
		glGetProgramInfoLog(shaderProgramID, log, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glUseProgram(shaderProgramID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
#pragma endregion
	//---------------------------------------------------------------------------------------------------------
#pragma region Shape Initialization
	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLfloat positions[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
	// Get the Attribute of position from the shader program
	GLint positionBufferID = glGetAttribLocation(shaderProgramID, "positionBuffer");
	glEnableVertexAttribArray(positionBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
			
#pragma endregion
	//---------------------------------------------------------------------------------------------------------
#pragma region Main Loop
	std::cout << "Main loop started!" << std::endl;
	MSG Msg = { 0 };
	while (WM_QUIT != Msg.message)
	{
		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
			// Clear the screen
			glClear(GL_COLOR_BUFFER_BIT);
			
			//Color and draw
			glDrawArrays(GL_TRIANGLES, 0, 3);

			//swap buffers
			SwapBuffers(m_hDC);
		}
	}
	
	
#pragma endregion
	glDeleteProgram(shaderProgramID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(vertexShaderID);

	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0); // Unbind VAO
	glDeleteVertexArrays(1, &vao);

	return 0;
}