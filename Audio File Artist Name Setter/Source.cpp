#include <iostream>
#include <string>
#include <filesystem>   // C++17
#include <tag.h>
#include <fileref.h>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Drag and drop audio files onto this .exe, or pass filenames as arguments.\n";
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        std::string filePath = argv[i];
        std::cout << "\nProcessing: " << filePath << "\n";

        // Extract just the filename without extension
        fs::path path(filePath);
        std::string filename = path.stem().string(); // e.g. "Artist - Title"

        // Look for " - " separator
        size_t pos = filename.find(" - ");
        if (pos == std::string::npos) {
            std::cout << "  Skipped (no ' - ' found in filename)\n";
            continue;
        }

        std::string artist = filename.substr(0, pos);
        std::string title = filename.substr(pos + 3);

        // Open the file with TagLib
        TagLib::FileRef f(filePath.c_str());
        if (!f.isNull() && f.tag()) {
            TagLib::Tag* tag = f.tag();
            tag->setArtist(artist);
            tag->setTitle(title);
            f.save();

            std::cout << "  Artist set to: " << artist << "\n";
            std::cout << "  Title  set to: " << title << "\n";
        }
        else {
            std::cout << "  Failed to open file.\n";
        }
    }

    std::cout << "\nDone! Press Enter to exit...";
    std::cin.get();
    return 0;
}
