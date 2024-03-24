#pragma once

#include <Geode/loader/Setting.hpp>
#include <matjson.hpp>

#include "RefreshSettingNode.hpp"

namespace horn {

//! @brief Monostate setting value for refresh button.
class RefreshSettingValue : public geode::SettingValue {
public:
    RefreshSettingValue(
        std::string const& key,
        std::string const& modID,
        int dummy
    ) : geode::SettingValue(key, modID) {}

    RefreshSettingNode* createNode(float width) override {
        return RefreshSettingNode::create(this, width);
    }

    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
};

} // namespace horn