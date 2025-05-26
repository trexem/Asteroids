#include "TextManager.h"
#include "PackReader.h"
#include "json.hpp"
#include "utils.hpp"

using json = nlohmann::json;

TextManager& TextManager::instance() {
    static TextManager instance;
    return instance;
}

bool TextManager::loadLanguage(const std::string& langCode) {
    std::string path = "lang/" + langCode + ".json";
    auto data = PackReader::instance().readFile(normalizePath(path));
    if (data.empty()) return false;

    try {
        json j = json::parse(data.begin(), data.end());
        translations.clear();
        for (auto& [k, v] : j.items()) {
            translations[k] = v.get<std::string>();
        }
        currentLang = langCode;
        return true;
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "JSON parse error in %s: %s", path.c_str(), e.what());
        return false;
    }
}

const std::string& TextManager::get(const std::string& key) {
    auto it = translations.find(key);
    if (it != translations.end()) return it->second;
    static std::string missing = "[[" + key + "]]";
    return missing;
}
