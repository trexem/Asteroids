#include "PackReader.h"
#include "ObfuscateReader.h"

#include <iostream>

PackReader& PackReader::instance() {
    static PackReader instance;
    return instance;
}

bool PackReader::init(const std::string& archivePath, void* assetMgr) {
    if (initialized) return true;
    if (!ObfuscatedReader::load(archivePath, raw)) {
        std::cerr << "Failed to read or decode pack file!" << std::endl;
        return false;
    }
    if (!mz_zip_reader_init_mem(&archive, raw.data(), raw.size(), 0)) {
        std::cerr << "Failed to init ZIP reader from memory!" << std::endl;
        return false;
    }
    initialized = true;
    return true;
}

std::vector<uint8_t> PackReader::readFile(const std::string& virtualPath) {
    size_t size = 0;
    void* data = mz_zip_reader_extract_file_to_heap(&archive, virtualPath.c_str(), &size, 0);
    if (!data || size == 0) {
        std::cerr << "Failed to read file from pack: " << virtualPath << std::endl;
        return {};
    }

    std::vector<uint8_t> buffer(static_cast<uint8_t*>(data), static_cast<uint8_t*>(data) + size);
    mz_free(data);
    return buffer;
}

std::vector<std::string> PackReader::listFiles(const std::string& prefix) {
    std::vector<std::string> list;
    for (int i = 0; i < (int)mz_zip_reader_get_num_files(&archive); i++) {
        mz_zip_archive_file_stat file;
        if (!mz_zip_reader_file_stat(&archive, i, &file)) {
            std::cerr << "Failed to read file in pack" << std::endl;
            continue;
        }

        if (!mz_zip_reader_is_file_a_directory(&archive, i)) {
            std::string name = file.m_filename;
            if (prefix.empty() || name.starts_with(prefix)) {
               list.emplace_back(file.m_filename);
            }
        }
    }
    return list;
}

void PackReader::close() {
    mz_zip_reader_end(&archive);
    initialized = false;
}
