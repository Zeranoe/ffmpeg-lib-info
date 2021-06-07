// Copyright (c) 2021 Kyle Schwarz <zeranoe@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <string>
#include <filesystem>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#undef UNICODE // TODO: Add Unicode support if someone needs it
#include <windows.h>

class LibInfo {
public:
    LibInfo(const std::string& filename)
    {
        // There may be dependent dlls in the same directory as the querying dll
        std::string parent = std::filesystem::path(filename).parent_path().string();
        if (SetDllDirectory(parent.c_str())) {
            mLib = LoadLibrary(filename.c_str());
        }
    }
    ~LibInfo() { FreeLibrary(mLib); }
    explicit operator bool() const { return mLib != nullptr; }

    std::unordered_map<std::string, const char*> licenses() const
    {
        return execute<const char*>("license");
    }

    std::unordered_map<std::string, unsigned> versions() const
    {
        return execute<unsigned>("version");
    }

    std::unordered_map<std::string, const char*> configurations() const
    {
        return execute<const char*>("configuration");
    }

private:
    HMODULE mLib;

    template <typename T>
    std::unordered_map<std::string, T> execute(const std::string& suffix) const
    {
        static const auto libs = {
            "avcodec",
            "avdevice",
            "avfilter",
            "avformat",
            "avutil",
            "postproc",
            "swresample",
            "swscale"
        };
        std::unordered_map<std::string, T> outputs; // <function, output>
        std::for_each(libs.begin(), libs.end(), [&](const auto& lib) {
            std::string name = std::string(lib) + "_" + suffix;
            if (const auto function = GetProcAddress(mLib, name.c_str())) {
                 outputs[name] = reinterpret_cast<T (*)(void)>(function)();
            }
        });
        return outputs;
    }
};

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <dll path>\n";
        return 1;
    }

    if (const auto info = LibInfo(argv[1])) {
        for (const auto& license : info.licenses()) {
            std::cout << license.first << ":\t" << license.second << std::endl;
        }
        for (const auto& version : info.versions()) {
            std::cout << version.first << ":\t" << std::to_string(version.second) << std::endl;
        }
        for (const auto& configuration : info.configurations()) {
            std::cout << configuration.first << ":\t" << configuration.second << std::endl;
        }
    } else {
        std::cerr << "error: unable to load library '" << argv[1] << "'" << std::endl;
        return 1;
    }

    return 0;
}