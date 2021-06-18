#include "QtOsgBridgeApplication.h"
#include "LoadingInitialState.h"

int main(int argc, char** argv)
{
  QtOsgBridgeApplication app(argc, argv);
  return app.runGame<LoadingInitialState>();
}
