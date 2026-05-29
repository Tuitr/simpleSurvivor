#include "Game.hpp"

#include <filesystem>
#include <fstream>
#include <exception>

int main()
{
    try
    {
        std::ofstream log(std::filesystem::path(PROJECT_ROOT) / "run_log.txt", std::ios::trunc);
        log << "Program entered main().\n";
        log << "Project root: " << PROJECT_ROOT << "\n";
        log << "Creating game window...\n";
        log.close();

        Game game;

        std::ofstream runningLog(std::filesystem::path(PROJECT_ROOT) / "run_log.txt", std::ios::app);
        runningLog << "Game object created. Entering loop.\n";
        runningLog.close();

        game.run();

        std::ofstream exitLog(std::filesystem::path(PROJECT_ROOT) / "run_log.txt", std::ios::app);
        exitLog << "Game loop ended normally.\n";
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::ofstream log(std::filesystem::path(PROJECT_ROOT) / "run_log.txt", std::ios::app);
        log << "Fatal exception: " << exception.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::ofstream log(std::filesystem::path(PROJECT_ROOT) / "run_log.txt", std::ios::app);
        log << "Unknown fatal exception.\n";
        return 1;
    }
}
