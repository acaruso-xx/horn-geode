#include "Manager.hpp"

#include <Geode/loader/Log.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <fmt/format.h>

#include <string>

namespace horn {

Manager* Manager::s_sharedManager = nullptr;

Manager* Manager::sharedManager() {
    if (s_sharedManager == nullptr) {
        s_sharedManager = new (std::nothrow) Manager();
        s_sharedManager->init();
    }

    return s_sharedManager;
}

bool Manager::init() {
    m_cache = geode::Mod::get()->getSavedValue<Cache>("cache");

    return true;
}

static std::string urlencode(std::string const& str) {
    using namespace geode::utils;
    
    // I see no problems with this.
    return string::replace(str, " ", "%20");
}

void Manager::refresh() {
    std::time_t now = std::time(nullptr);

    // Check every 7 days.
    if (m_cache.getTimestamp() + 604800 < now) {
        download([]{}, []{});
    }
}

void Manager::download(
    std::function<void()> finished,
    std::function<void()> failed
) {
    using namespace geode::utils;

    geode::log::info("Downloading level info");

    std::string sheet = "All levels in chronological order";
    std::string tqx = "out:csv";
    std::string tq = "select D, E, F, G";
    std::string query = fmt::format(
        "sheet={}&tqx={}&tq={}",
        sheet,
        tqx,
        tq
    );

    std::string url = fmt::format(
        "https://docs.google.com/spreadsheets/d/{}/gviz/tq?{}",
        getSheetID(),
        urlencode(query)
    );

    web::AsyncWebRequest()
        .fetch(url)
        .text()
        .then([=, this](std::string const& str) {
            cache(str);
            finished();
        })
        .expect([=](std::string const& error) {
            failed();
        });
}

std::string Manager::getSheetID() const {
    return "1-Abvx7zXRAqpGFVbdTXpn6g1TRYp9WuZqW2pHWE7Dr4";
}

void Manager::cache(std::string const& str) {
    m_cache = Cache(str);
    geode::Mod::get()->setSavedValue("cache", m_cache);
}

} // namespace horn

$on_mod(Loaded) {
    horn::Manager::sharedManager()->refresh();
}
