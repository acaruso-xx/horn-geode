#include "LevelInfo.hpp"

namespace horn {

LevelInfo::LevelInfo(std::vector<std::string> const& vec) {
    m_tier = std::stoi(vec[1]);
    m_skillset = vec[2];
    m_description = vec[3];
}

LevelInfo::LevelInfo(matjson::Value const& json) {
    m_tier = json["tier"].as_int();
    m_skillset = json["skillset"].as_string();
    m_description = json["description"].as_string();
}

matjson::Value LevelInfo::json() const {
    matjson::Value json;

    json["tier"] = m_tier;
    json["skillset"] = m_skillset;
    json["description"] = m_description;

    return json;
}

} // namespace horn

