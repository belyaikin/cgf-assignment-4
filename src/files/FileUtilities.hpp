//
// Created by Dmitriy on 18.10.26.
//

#pragma once
#include <string>

namespace assignment::files {
    class FileUtilities final
    {
    public:
        static std::string getFileContents(const char *filename);
    };
}
