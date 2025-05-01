#include "ModelViewer.h"

int main() {
	auto app = new ModelViewer();
	app->Run( "Model Viewer", 1280, 720, false );
	delete app;
}