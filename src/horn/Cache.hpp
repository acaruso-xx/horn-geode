#pragma once

#include <cocos2d.h>
#include <matjson.hpp>

#include <ctime>
#include <optional>
#include <string>
#include <unordered_map>

#include "LevelInfo.hpp"

namespace horn {

//! @brief Cached spreadsheet data.
class Cache final {
public:
    Cache() : m_timestamp(0) {}

    //! @brief Construct cache from unparsed csv.
    //! @param csv Unparsed CSV data.
    Cache(std::string const& str);

    //! @brief Construct cache from JSON.
    //! @param json JSON data.
    Cache(matjson::Value const& json);

    //! @brief Serialize into JSON.
    //! @return JSON value.
    matjson::Value json() const;

    //! @brief Get level info for level.
    //! @param levelID Level ID.
    //! @return Level info.
    std::optional<LevelInfo> getLevelInfo(int levelID);

    std::time_t getTimestamp() const { return m_timestamp; }

private:
    std::time_t m_timestamp;
    std::unordered_map<int, LevelInfo> m_levels;
};

} // namespace horn

template<>
struct matjson::Serialize<horn::Cache> {
    static horn::Cache from_json(matjson::Value const& json) {
        return horn::Cache(json);
    }

    static matjson::Value to_json(horn::Cache const& cache) {
        return cache.json();
    }

    static bool is_json(horn::Cache const& cache) {
        return true;
    }
};
