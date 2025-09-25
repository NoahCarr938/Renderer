#include "Utils.h"

/* File opening */
#include <fstream>
/* To make sure things dont fail */
#include <cassert>

std::string DumpToString(const std::string& InputPath)
{
    // Output to return later
    std::string OutputString;

    if (std::ifstream InputFile{ InputPath, std::ios::in | std::ios::ate })
    {
        // This has to be true or we crash the program (intentionally)
        assert(InputFile.is_open() && "File did not actually open");
        // Get the current position to determine how big it is
        auto Size = InputFile.tellg();
        // Allocate a string that is suitably large
        OutputString = std::string(Size, '\0');

        // Move the get position back to the beginning
        InputFile.seekg(0);

        // Write input file to output stirng
        InputFile.read(&OutputString[0], Size);

        // Close the file
        InputFile.close();
    }

    return OutputString;
}
