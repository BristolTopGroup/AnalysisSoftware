/*
 * StringOperations.cpp
 *
 *  Created on: 11 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/StringOperations.h"
namespace BAT{
extern vector<string> getDirectoriesFromPath(string path) {

    vector<string> directories;
    if (path == "" || path == "/")
        return directories;

    if (path.find_last_of("/") == path.length() - 1) {
        path.erase(path.length() - 1);
    }

    if (path.find("/") == string::npos) {
        directories.push_back(path);
    } else {
        // Skip delimiters at beginning.
        string::size_type lastPos = path.find_first_not_of("/", 0);
        // Find first "non-delimiter".
        string::size_type pos = path.find_first_of("/", lastPos);

        while (string::npos != pos || string::npos != lastPos) {
            // Found a directory, add it to the vector.
            directories.push_back(path.substr(lastPos, pos - lastPos));
            // Skip delimiters.
            lastPos = path.find_first_not_of("/", pos);
            // Find next "non-delimiter"
            pos = path.find_first_of("/", lastPos);
        }
    }
    return directories;
}
}
