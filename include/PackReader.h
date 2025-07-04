#pragma once

#include <string>
#include <vector>

#include "miniz.h"

class PackReader {
public:
    static PackReader& instance();

    bool init(const std::string& archivePath, void* assetMgr = nullptr);
    std::vector<uint8_t> readFile(const std::string& virtualPath);
    std::vector<std::string> listFiles(const std::string& prefix = "");

    void close();

private:
    PackReader() = default;
    mz_zip_archive archive;
    std::vector<uint8_t> raw;
    bool initialized {false};
};
