#include "panic.h"
#include "BasicRenderer.h"

void Panic(const char* panicMessage) {
	GlobalRenderer->ClearColour = 0xff000000;
	GlobalRenderer->Clear(); 

	GlobalRenderer->CursorPosition = { 0,0 };

	GlobalRenderer->Colour = 0; 

	GlobalRenderer->Print("Kernel panic");

	GlobalRenderer->Next();
	GlobalRenderer->Next();

	GlobalRenderer->Print(panicMessage); 
}