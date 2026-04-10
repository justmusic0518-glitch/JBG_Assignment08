// SprataGameMode.cpp
#include "SprataGameMode.h"
#include "SprataPlayerController.h"
#include "SpartaGameState.h"

ASprataGameMode::ASprataGameMode(){
	DefaultPawnClass = ASprataGameMode::StaticClass();
	PlayerControllerClass = ASprataPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}
