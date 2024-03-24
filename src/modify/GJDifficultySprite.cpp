#include <Geode/modify/GJDifficultySprite.hpp>

#include <Geode/binding/GJGameLevel.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <fmt/format.h>

#include "../horn/Manager.hpp"

class $modify(GJDifficultySprite) {
    //! @brief This is a perfect place to hook since it takes a level as a
    //! parameter and is layer-agnostic. Praying this isn't inlined on mac.
    void updateFeatureStateFromLevel(GJGameLevel* level) {
        GJDifficultySprite::updateFeatureStateFromLevel(level);

        auto cache = horn::Manager::sharedManager()->getCache();
        auto levelInfo = cache.getLevelInfo(level->m_levelID);

        if (!levelInfo) {
            return;
        }

        int tier = levelInfo->tier();
        updateHorns(tier);

        if (tier == 6) {
            updateTakayama();
        }
    }

    //! @brief Add horns.
    void updateHorns(int tier) {
        if (m_fields->m_hornsCreated) {
            return;
        }

        std::string frame = fmt::format("horn{:02}_001.png"_spr, tier);
        auto* horns = cocos2d::CCSprite::createWithSpriteFrameName(
            frame.c_str()
        );
        horns->setPosition(getContentSize() / 2.0f);
        addChild(horns, -1);

        m_fields->m_hornsCreated = true;
    }

    //! @brief Add Super Mario Bros 2D easter egg.
    void updateTakayama() {
        if (m_fields->m_takayamaCreated) {
            return;
        }

        auto* eyes = cocos2d::CCSprite::createWithSpriteFrameName(
            "takayama_001.png"_spr
        );
        eyes->setPosition(getContentSize() / 2.0f);
        eyes->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
        addChild(eyes, 2);

        m_fields->m_takayamaCreated = true;
    }
    
    bool m_hornsCreated;
    bool m_takayamaCreated;
};
