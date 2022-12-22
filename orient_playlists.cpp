// orient_playlists.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <vector>
#include <algorithm>

// using namespace std;
namespace fs = std::filesystem;

std::string win_to_lin_path(std::string old_path) {
/*
win_to_lin_path
Changes the slash direction and exit characters from Windows standard to Linux standard.

Also, this is redundant with DisplayPathInfo for our purposes, but is more general
*/

    size_t start_pos = 0;

    /*
    if (old_path.find(":", 0) != std::end(old_path)) {
        old_path = old_path.substr()
    }
    */

    while ((start_pos = old_path.find("\\", start_pos)) != std::string::npos) {
        old_path.replace(start_pos, 1, "/");
        start_pos += 1;
    }

    return old_path;
};


class dirObj
{
public:
    int numFiles = 0;
    int depthLimitFlag = false;
    int depthLimit = 0;
    int curDepth = 0;
    fs::path baseDir {};
    std::vector<std::string> files {};
    std::vector<std::string> dicts {};

public:
    void iterate_files(const fs::path curPath) {

        for (const auto& entry : fs::directory_iterator(curPath)) {

            std::error_code ec;

            if (fs::is_directory(entry.path(), ec) && depthLimitFlag == false) 
            {
                fs::path newPath = entry.path();
                dicts.push_back(newPath.string());
                iterate_files(newPath);
            }
            else if (is_directory(entry.path(), ec) && depthLimitFlag == true) 
            {
                fs::path newPath = entry.path();
                dicts.push_back(newPath.string());
            }
            /*
            else if (is_directory(entry.path(), ec) && depthLimitFlag == true && depthLimit =< 1)
            {
                path newPath = entry.path();
                std::cout << newPath.string() << std::endl;
            } 
            else if (is_directory(entry.path(), ec) && depthLimitFlag == true && depthLimit =< curDepth)
            {
                path newPath = entry.path();
                std::cout << newPath.string() << std::endl;
                iterate_files(newPath);
            }
            */
            else if (is_regular_file(entry.path(), ec))
            {
                numFiles++;
                files.push_back(entry.path().string());
            }
        }

    }

public:
    void startDirCrawl(bool iter_depth) {
        if (baseDir.string() == "")
        {
            std::cout << "No path defined!" << std::endl;
            return;
        };

        iterate_files(baseDir);
    }

public:
    std::vector<std::string> selectByExt(const std::string ext) {

        std::vector<std::string> filtered_files;
        
        for (auto filePathStr: files) {  

            if (fs::path(filePathStr).extension() == ext) {
                filtered_files.push_back(filePathStr);
            }
        }
    
        return filtered_files;
    }
};


class playlistObj
{
public:
    fs::path filePath {};
    fs::path musicLib {};
    std::string fileExt {};
    size_t numEntries {};
    std::vector<std::string> playlistContent {};

public:
    void crawlFile(const fs::path newPath) {

        filePath = newPath;
        fileExt = newPath.extension();

        std::ifstream file (filePath);
        std::string str;

        while (std::getline(file, str)) {
            playlistContent.push_back(str);
            numEntries++;
        };

        if (numEntries > 0) {
             
            std::cout << "Length: " << numEntries << std::endl;
            std::cout << "First entry: " << playlistContent[0] << std::endl;
        }

        else {
            std::cout << "Error: " << filePath.string() << " empty!" << std::endl;
        }
    }

public:
    std::string changeInternalPath(std::string oldPath) {
        /*
        This is the one that will need the most care in the future, as it's based on my personal structure:
        Albums/[aritst]/[album]/[tracks]
        Which is inconsistent within my library (there are single tracks in the artist folder) and may not reflect other's library file structure
        */

        size_t album_idx = oldPath.find("Albums", oldPath.length());
        fs::path new_path = musicLib / oldPath.substr(album_idx);

        return win_to_lin_path(new_path.string());
    }

public:
    void gen_new_playlist(const fs::path newPath, std::string newName, std::string newExt) {



    }
};


int main() {

    playlistObj pl;
    pl.musicLib = "/media/disc1/Music/Albums";

    // pl.crawlFile("/media/disc1/Music/Playlists/New.m3u");
    std::string newpath = pl.changeInternalPath("E:\\Music\\Albums\\Andrew Bird\\Inside Problems\\01 - Underlands.mp3");

    std::cout << newpath << std::endl;

    /*
    dirObj file_dir;
    file_dir.baseDir = "/media/disc1/Music/Playlists/";
    file_dir.depthLimitFlag = true;

    file_dir.startDirCrawl(0);

    for (std::string fileStr: file_dir.files) {
        std::cout << fileStr << std::endl;
    }

    std::cout << "Filtered files: " << std::endl;
    std::vector<std::string> playlists = file_dir.selectByExt(".m3u");

    for (std::string fileStr: playlists) {
        std::cout << fileStr << std::endl;
    }

    */

    /*
    file_dir.origPlaylistDir = "/media/disc1/Music/Playlists/";
    file_dir.newPlaylistDir = "/home/shjewell/Music/Playlists/";
    file_dir.origMusicDir = "/home/shjewell/Music/Albums/";
    file_dir.newMusicDir = "/";
    file_dir.extension = ".m3u";
    */
}