#pragma once

#include <matjson.hpp>
#include <string>
#include <vector>

namespace horn {

//! @brief Info for individual levels.
class LevelInfo final {
public:
    LevelInfo() {}

    //! @brief Construct level info from vector.
    LevelInfo(std::vector<std::string> const& vec);

    //! @brief Construct level info from JSON.
    LevelInfo(matjson::Value const& json);

    //! @brief Serialize into JSON.
    //! @return JSON value.
    matjson::Value json() const;

    int tier() const { return m_tier; }
    std::string skillset() const { return m_skillset; }
    std::string description() const { return m_description; }

private:
    int m_tier;
    std::string m_skillset;
    std::string m_description;
};

} // namespace horn

template<>
struct matjson::Serialize<horn::LevelInfo> {
    static horn::LevelInfo from_json(matjson::Value const& json) {
        return horn::LevelInfo(json);
    }

    static matjson::Value to_json(horn::LevelInfo const& levelInfo) {
        return levelInfo.json();
    }

    static bool is_json(horn::LevelInfo const& json) {
        return true;
    }
};
