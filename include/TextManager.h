#pragma once

#include <format>
#include <string>
#include <unordered_map>

class TextManager {
public:
    static TextManager& instance();
    bool loadLanguage(const std::string& langCode);
    const std::string& get(const std::string& key);
    
    template <typename... Args>
    std::string format(const std::string& key, Args&&... args) const {
        auto it = translations.find(key);
        if (it != translations.end()) {
            return std::vformat(it->second, std::make_format_args(std::forward<Args>(args)...));
        } else {
            return "[[" + key + "]]";
        }
    }

private:
    TextManager() = default;
    std::unordered_map<std::string, std::string> translations;
    std::string currentLang;
};
