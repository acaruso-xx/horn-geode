#include "RefreshSettingNode.hpp"

#include <Geode/binding/FLAlertLayer.hpp>
#include "RefreshSettingValue.hpp"
#include "Manager.hpp"

namespace horn {

RefreshSettingNode::RefreshSettingNode() {
    m_loadingCircle = nullptr;
}

RefreshSettingNode* RefreshSettingNode::create(
    RefreshSettingValue* value,
    float width
) {
    auto* ret = new (std::nothrow) RefreshSettingNode();

    if (ret && ret->init(value, width)) {
        ret->autorelease();
    } else {
        CC_SAFE_RELEASE(ret);
    }

    return ret;
}

bool RefreshSettingNode::init(RefreshSettingValue* value, float width) {
    if (!SettingNode::init(value)) {
        return false;
    }

    this->setContentSize({ width, 40.0f });

    auto* nameLabel = cocos2d::CCLabelBMFont::create(
        "Refresh",
        "bigFont.fnt"
    );
    nameLabel->setAnchorPoint({ 0.0f, 0.5f });
    nameLabel->setPosition({ 20.0f, 20.0f });
    nameLabel->setScale(0.5f);
    addChild(nameLabel);

    m_menu = cocos2d::CCMenu::create();
    m_menu->ignoreAnchorPointForPosition(false);
    m_menu->setAnchorPoint({ 0.5f, 0.5f });
    m_menu->setContentSize({35.0f, 35.0f});
    m_menu->setPosition({
        width - (35.0f / 2.0f) - 20.0f,
        20.0f
    });
    addChild(m_menu);

    auto* sprite = cocos2d::CCSprite::createWithSpriteFrameName(
        "GJ_replayBtn_001.png"
    );
    sprite->setScale(0.5f);

    m_button = CCMenuItemSpriteExtra::create(
        sprite,
        this,
        menu_selector(RefreshSettingNode::onRefresh)
    );
    m_button->setPosition(m_menu->getContentSize() / 2.0f);
    m_menu->addChild(m_button);

    return true;
}

void RefreshSettingNode::onRefresh(cocos2d::CCObject* sender) {
    auto* director = cocos2d::CCDirector::sharedDirector();
    auto* mgr = Manager::sharedManager();

    cocos2d::CCSize offset = director->getWinSize() / 2.0f;
    m_loadingCircle = LoadingCircle::create();
    m_loadingCircle->setPosition((m_menu->getContentSize() / 2.0f) - offset);
    m_loadingCircle->setScale(0.45f);

    // What could possibly go wrong?
    m_loadingCircle->setParentLayer(
        reinterpret_cast<cocos2d::CCLayer*>(m_menu)
    );

    m_loadingCircle->show();
    m_button->setVisible(false);

    mgr->download(
        [this] {
            m_loadingCircle->fadeAndRemove();
            m_button->setVisible(true);
        },
        [this] {
            m_loadingCircle->fadeAndRemove();
            m_button->setVisible(true);

            FLAlertLayer::create(
                "Error",
                "Download failed. Please try again later.",
                "OK"
            )->show();
        }
    );
}

} // namespace horn
