#include "Cache.hpp"

#include <Geode/loader/Log.hpp>
#include <Geode/utils/string.hpp>

#include "CSV.hpp"

namespace horn {

Cache::Cache(std::string const& str) {
    using namespace geode::utils;
    
    m_timestamp = std::time(nullptr);
    
    CSV csv(str);
    auto rows = csv.rows();

    // First row is the header. Skip it.
    for (int i = 1; i < rows.size(); i++) {
        auto row = rows[i];

        int levelID = std::stoi(row[0]);
        m_levels.emplace(levelID, row);
    }
}

Cache::Cache(matjson::Value const& json) {
    m_timestamp = json["timestamp"].as_int();

    auto levels = json["levels"].as_object();
    for (auto& [k, v] : levels) {
        int levelID = std::stoi(k);
        auto info = horn::LevelInfo(v);

        m_levels[levelID] = info;
    }
}

matjson::Value Cache::json() const {
    matjson::Object levels;
    for (auto [levelID, info] : m_levels) {
        std::string k = std::to_string(levelID);

        levels[k] = info;
    }

    matjson::Object res;
    res["timestamp"] = m_timestamp;
    res["levels"] = levels;

    return res;
}

std::optional<LevelInfo> Cache::getLevelInfo(int levelID) {
    if (m_levels.count(levelID) == 0) {
        return std::nullopt;
    }

    return m_levels[levelID];
}

} // namespace horn
