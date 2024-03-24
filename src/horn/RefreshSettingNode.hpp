#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/loader/SettingNode.hpp>

namespace horn {

class RefreshSettingValue;

//! @brief Refresh setting node.
class RefreshSettingNode : public geode::SettingNode {
public:
    RefreshSettingNode();

    //! @brief Create refresh setting node.
    //! @param value Setting value.
    //! @param width Width of settings layer.
    //! @return Refresh button.
    static RefreshSettingNode* create(
        RefreshSettingValue* value, 
        float width
    );

    //! @brief Initialize refresh setting node.
    //! @param value Setting value.
    //! @param width Width of settings layer.
    //! @return Whether the initialization was successful.
    bool init(RefreshSettingValue* value, float width);

    void commit() override {}
    bool hasUncommittedChanges() override { return false; }
    bool hasNonDefaultValue() override { return false; }
    void resetToDefault() override {}

private:
    //! @brief Refresh callback.
    //! @param sender Sender.
    void onRefresh(cocos2d::CCObject* sender);

    cocos2d::CCMenu* m_menu;
    CCMenuItemSpriteExtra* m_button;
    LoadingCircle* m_loadingCircle;
};

} // namespace horn
