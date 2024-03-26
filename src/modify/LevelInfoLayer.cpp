#include <Geode/modify/LevelInfoLayer.hpp>

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/binding/GJDifficultySprite.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Mod.hpp>

#include "../horn/Manager.hpp"

class $modify(CustomLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool p1) {
        if (!LevelInfoLayer::init(level, p1)) {
            return false;
        }

        auto cache = horn::Manager::sharedManager()->getCache();
        m_fields->m_levelInfo = cache.getLevelInfo(level->m_levelID);

        if (m_fields->m_levelInfo) {
            addHintArt();

            addButton();
            updateButton();
        }

        return true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();

        if (m_fields->m_levelInfo) {
            updateButton();
        }
    }

    //! @brief Add hint art if not already shown.
    void addHintArt() {
        auto* mgr = horn::Manager::sharedManager();
        if (mgr->getHintShown()) {
            return;
        }

        cocos2d::CCPoint position = {
            m_difficultySprite->getPositionX() - 47.0f,
            m_difficultySprite->getPositionY() + 12.0f
        };

        auto* hint = cocos2d::CCSprite::createWithSpriteFrameName(
            "hint_001.png"_spr
        );
        hint->setPosition(position);

        addChild(hint);
    }

    //! @brief Add difficulty button.
    //! @param levelInfo Level info.
    void addButton() {
        auto* menu = cocos2d::CCMenu::create();
        menu->ignoreAnchorPointForPosition(false);
        menu->setContentSize({ 80.0f, 80.0f });
        menu->setPosition(m_difficultySprite->getPosition());

        m_fields->m_difficultyButton = CCMenuItemSpriteExtra::create(
            m_difficultySprite,
            this,
            menu_selector(CustomLevelInfoLayer::onDifficulty)
        );
        m_fields->m_difficultyButton->setPosition({
            menu->getContentSize() / 2.0f
        });

        menu->addChild(m_fields->m_difficultyButton);
        addChild(menu);

        removeChild(m_difficultySprite);
    }

    //! @brief Unfuck difficulty button position.
    void updateButton() {
        m_difficultySprite->setPosition({
            m_fields->m_difficultyButton->getContentSize() / 2.0f
        });
    }

    //! @brief Difficulty button clicked callback.
    void onDifficulty(cocos2d::CCObject* sender) {
        horn::Manager::sharedManager()->setHintShown(true);

        std::string body = fmt::format(
            "<cr>Tier</c>: {}" "\n"
            "<cl>Skillset</c>: {}" "\n"
            "\n"
            "{}",
            m_fields->m_levelInfo->tier(),
            m_fields->m_levelInfo->skillset(),
            m_fields->m_levelInfo->description()
        );

        FLAlertLayer::create(
            nullptr,
            m_level->m_levelName.c_str(),
            body,
            "OK",
            nullptr,
            400.0f
        )->show();
    }

    std::optional<horn::LevelInfo> m_levelInfo;
    CCMenuItemSpriteExtra* m_difficultyButton;
};
