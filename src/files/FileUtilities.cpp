//
// Created by Dmitriy on 18.10.26.
//

#include "FileUtilities.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace assignment::files {
    std::string FileUtilities::getFileContents(const char *filename) {
        if (std::ifstream in(filename, std::ios::binary); in) {
            std::cout << "[FileUtilities]: Found file at " << filename << "\n";

            std::string contents;

            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();

            return contents;
        }

        throw(errno);
    }
}
