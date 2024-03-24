#include "RefreshSettingValue.hpp"

#include <Geode/loader/Mod.hpp>

$on_mod(Loaded) {
    geode::Mod::get()->addCustomSetting<horn::RefreshSettingValue>(
        "refresh",
        0
    );
}