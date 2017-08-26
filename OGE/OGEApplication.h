#pragma once
#include "OGEWindow.h"
#include "OGEGraphics.h"
#include "OGEInput.h"
#include <iostream>

using namespace std;




// Game application
class OGEApplication
{
	private:
		static bool Init();
		static void EnterLoop();
		static bool Update(float timeDelta);
		static void Release();


	public:
		static void Run();
};

