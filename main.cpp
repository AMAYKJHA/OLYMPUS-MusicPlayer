#include "mainwindow.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include <SDL_mixer.h>
#include <QApplication>

int init();
void cleanup();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Initialize SDL and SDL_mixer
    if (init() != 0) {
        return 1;  // Initialization failed, return early
    }

    w.setWindowIcon(QIcon(":/images/icons/music.ico"));
    w.setWindowTitle("Olympus");
    w.setFixedSize(800, 600);
    w.show();

    int exitCode = a.exec();  // Run the Qt event loop

    cleanup();  // Cleanup SDL and SDL_mixer before exiting the application

    return exitCode;  // Return the exit code of the event loop
}

int init() {
    // Initialize SDL and SDL_mixer
    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::cout << "Initializing SDL_mixer..." << std::endl;
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        std::cerr << "SDL_mixer initialization failed: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer audio initialization failed: " << Mix_GetError() << std::endl;
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    std::cout << "Initialization successful." << std::endl;
    return 0;
}

void cleanup() {
    std::cout << "Cleaning up SDL and SDL_mixer..." << std::endl;
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    std::cout << "Cleanup successful." << std::endl;
}
