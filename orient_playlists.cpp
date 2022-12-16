// orient_playlists.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

// using namespace std;
using namespace std::filesystem;

std::string win_to_lin_path(std::string playlist_path);

class fileInformation
{
public:
    int numFiles {};
    path newPlaylistDir {};
    path origPlaylistDir {};
    path newMusicDir {};
    path origMusicDir {};
    std::string extension {};
    std::vector<std::string> newPlaylists {};
    std::vector<std::string> origPlaylists {};

public:
    std::wstring DisplayPathInfo(const path pathToDisplay)
    {
        // This path may or may not refer to an existing file. We are
        // examining this path string, not file system objects.
        // path pathToDisplay(L"E:\Documents\Datasets\Weather Data\Copernicus ");

        std::wostringstream wos;
        int i = 0;
        wos << L"Displaying path info for: " << pathToDisplay << std::endl;
        for (path::iterator itr = pathToDisplay.begin(); itr != pathToDisplay.end(); ++itr)
        {
            wos << L"path part: " << i++ << L" = " << *itr << std::endl;
        }

        wos << L"root_name() = " << pathToDisplay.root_name() << std::endl
            << L"root_path() = " << pathToDisplay.root_path() << std::endl
            << L"relative_path() = " << pathToDisplay.relative_path() << std::endl
            << L"parent_path() = " << pathToDisplay.parent_path() << std::endl
            << L"filename() = " << pathToDisplay.filename() << std::endl
            << L"stem() = " << pathToDisplay.stem() << std::endl
            << L"extension() = " << pathToDisplay.extension() << std::endl;

        if (pathToDisplay.extension() == extension) {
            crawlFile(pathToDisplay);
        }

        return wos.str();
    }

public:
    void iterate_files(const path curPath, const bool iter_depth) {

        for (const auto& entry : directory_iterator(curPath)) {

            // std::cout << entry.path() << std::endl;

            std::error_code ec;
            std::wstring pathInfo;

            if (is_directory(entry.path(), ec) && iter_depth == true) 
            {
                path newPath = entry.path();
                std::cout << newPath.string() << std::endl;
                iterate_files(newPath, true);
            }
            else if (is_regular_file(entry.path(), ec))
            {
                // std::size_t found = entry.path().string().find_last_of("/\\");
                // std::size_t second_to_last = entry.path().string().substr(0, found).find_last_of("/\\");
                // std::size_t ext_loc = entry.path().string().find_first_of(".");

                pathInfo = DisplayPathInfo(entry);
                // std::wcout << pathInfo << std::endl;

                /*
                numFiles += 1;
                filePaths.push_back(entry.path().string());
                fileNames.push_back(entry.path().string().substr(found + 1));
                containingFolder.push_back(entry.path().string().substr(second_to_last + 1, (found - second_to_last - 1)));
                extension.push_back(entry.path().string().substr(ext_loc));
                */
                /*
                std::cout << "path: " << entry.path().string().substr(0, found) << std::endl;
                std::cout << "file: " << entry.path().string().substr(found+1) << std::endl;
                */
            }
        }

    }

public:
    void crawlPath(bool iter_depth) {
        if (origPlaylistDir == "")
        {
            std::cout << "No path defined!" << std::endl;
            return;
        };

        iterate_files(origPlaylistDir, iter_depth);
    }

public:
    void crawlFile(const path filePath) {

        std::ifstream file (filePath);
        std::string str;
        std::vector<std::string> file_contents;
        int file_length = 0;

        while (std::getline(file, str)) {
            file_contents.push_back(str);
            file_length += 1;
        }

        if (file_length > 0) {
            std::cout << "Length: " << file_length << std::endl;
            std::cout << "First entry: " << file_contents[0] << std::endl;
            std::cout << "Edited entry: " << win_to_lin_path(file_contents[0]) << std::endl;
            
        }
        else {
            std::cout << "Error: " << filePath.string() << " empty!" << std::endl;
        }
    }

public:
    void write_file_list(const std::string outputName) {

        std::ofstream myfile;
        myfile.open(outputName);

        myfile << "path,folder,name,extension\n";

        for (int i = 0; i < numFiles; i++) {
            // myfile << filePaths[i] << ",";
            // myfile << containingFolder[i] << ",";
            // myfile << fileNames[i] << ",";
            myfile << extension[i] << ",";
            myfile << "\n";
        }

        myfile.close();

        std::cout << "File written to: " << outputName << std::endl;

    }


};

void edit_strings(const path music_path, std::string playlist_path) {

    std::cout << playlist_path << std::endl;
};

std::string win_to_lin_path(std::string playlist_path) {
/*
win_to_lin_path

Changes the slash direction and exit characters from Windows standard to Linux standard.
*/

    size_t start_pos = 0;

    while ((start_pos = playlist_path.find("\\", start_pos)) != std::string::npos) {
        playlist_path.replace(start_pos, 1, "/");
        start_pos += 1;
    }

    return playlist_path;
};



int main() {

    fileInformation file_dir;

    file_dir.origPlaylistDir = "/media/disc1/Music/Playlists/";
    file_dir.newPlaylistDir = "/home/shjewell/Music/Playlists/";
    file_dir.origMusicDir = "/home/shjewell/Music/Albums/";
    file_dir.newMusicDir = "/";
    file_dir.extension = ".m3u";

    // win_to_linux(file_dir.musicFolder, "E:\\Music\\Albums\\Ages and Ages\\Divisionary\\01-04- Big Idea.mp3");

    file_dir.crawlPath(false);
    
    //wcout << DisplayPathInfo(L"E:\Documents\Datasets\Weather Data\Copernicus ") << endl;
    // wcout << ComparePaths() << endl; // see following example
    // wcout << endl << L"Press Enter to exit" << endl;
    // wstring input;
    // getline(wcin, input);
}