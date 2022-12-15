// orient_playlists.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <vector>

// using namespace std;
using namespace std::filesystem;


class fileInformation
{
public:
    int numFiles {};
    path newPlaylistDir {};
    path origPlaylistDir {};
    path musicFolder {};
    std::string extension {};
    std::vector<std::string> newPlaylists {};
    std::vector<std::string> origPlaylists {};

public:
    std::wstring DisplayPathInfo(path pathToDisplay)
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

        return wos.str();
    }

public:
    void iterate_files(path curPath, bool iter_depth) {

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
                std::size_t found = entry.path().string().find_last_of("/\\");
                std::size_t second_to_last = entry.path().string().substr(0, found).find_last_of("/\\");
                std::size_t ext_loc = entry.path().string().find_first_of(".");

                pathInfo = DisplayPathInfo(entry);
                std::wcout << pathInfo << std::endl;

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

/*
public:
    void write_file_list(std::string outputName) {

        std::ofstream myfile;
        myfile.open(outputName);

        myfile << "path,folder,name,extension\n";

        for (int i = 0; i < numFiles; i++) {
            myfile << filePaths[i] << ",";
            myfile << containingFolder[i] << ",";
            myfile << fileNames[i] << ",";
            myfile << extension[i] << ",";
            myfile << "\n";
        }

        myfile.close();

        std::cout << "File written to: " << outputName << std::endl;

    }
*/

};


int main() {

    fileInformation file_dir;

    file_dir.origPlaylistDir = "/media/disc1/Music/Playlists/";
    file_dir.newPlaylistDir = "/home/shjewell/Music/Playlists/";
    file_dir.musicFolder = "/home/shjewell/Music/Albums/";
    file_dir.extension = ".m3u";

    file_dir.crawlPath(false);
    
    //wcout << DisplayPathInfo(L"E:\Documents\Datasets\Weather Data\Copernicus ") << endl;
    // wcout << ComparePaths() << endl; // see following example
    // wcout << endl << L"Press Enter to exit" << endl;
    // wstring input;
    // getline(wcin, input);
}