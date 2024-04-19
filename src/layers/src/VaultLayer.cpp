#include "../headers/VaultLayer.h"
#include <algorithm>
#include <cctype>
#include <string>

bool VaultLayer::init() {
    if(!CCLayer::init())
        return false;

    auto director = CCDirector::get();
    auto winSize = director->getWinSize();

    m_bg = CCSprite::create("GJL_lunarBG_001.png"_spr);
    m_bg->setAnchorPoint({0.f, 0.f});
    addChild(m_bg, -2);

    m_bg->setScaleX((winSize.width + 10.f) / m_bg->getTextureRect().size.width);
    m_bg->setScaleY((winSize.height + 10.f) / m_bg->getTextureRect().size.height);
    m_bg->setPosition(ccp(-5, -5));

    m_title = CCLabelBMFont::create("Lunar Hideout", "goldFont.fnt");
    addChildAtPosition(m_title, Anchor::Top, ccp(0, -22), false);

    m_textInput = TextInput::create(180, "...");
    m_textInput->setFilter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
    m_textInput->setMaxCharCount(20);

    addChildAtPosition(m_textInput, Anchor::Center, ccp(0, 65), false);

    m_response = CCLabelBMFont::create("", "bigFont.fnt");
    updateMessageLabel("It's you again...");

    addChildAtPosition(m_response, Anchor::Center, ccp(0, 105), false);

    m_lunarMasterSpr = CCSprite::createWithSpriteFrameName("GJL_lunarMaster_001.png"_spr);
    m_lunarMasterSpr->setScale(1.15);

    m_lunarMasterBtn = CCMenuItemSpriteExtra::create(m_lunarMasterSpr, this, menu_selector(VaultLayer::onSubmit));

    auto lunarMasterMenu = CCMenu::create();
    lunarMasterMenu->addChild(m_lunarMasterBtn);

    addChildAtPosition(lunarMasterMenu, Anchor::Center, ccp(0, -35), false);

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(VaultLayer::onClose));

    bool hasSeenAnim = Mod::get()->getSavedValue<bool>("has-seen-anim");
    m_playSpr = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");
    m_playSpr->setScale(0.5f);

    m_playBtn = CCMenuItemSpriteExtra::create(m_playSpr, this, nullptr);

    m_playMenu = CCMenu::create();
    m_playMenu->addChild(m_playBtn);

    if(!hasSeenAnim) {
        m_playSpr->setScale(1.f);
        auto scaleTo = CCScaleTo::create(0.5f, 0.5f);
        auto easeInOut = CCEaseInOut::create(scaleTo, 2.0f);
        m_playSpr->runAction(easeInOut);
    }

    auto backMenu = CCMenu::create();
    backMenu->addChild(backBtn);
    
    addChildAtPosition(backMenu, Anchor::TopLeft, ccp(25, -22), false);
    addChildAtPosition(m_playMenu, Anchor::BottomRight, ccp(-50, 50), false);

    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    GameManager::sharedState()->fadeInMusic("secretLoop.mp3"_spr);
    return true;
}

void VaultLayer::updateMessageLabel(std::string message) {
    m_response->setString(message.c_str());
    m_response->limitLabelWidth(320, 0.6f, 0.f);
}

void VaultLayer::onSubmit(CCObject*) {
    std::string response = getMessage();
    std::string lower;

    auto GM = GameManager::sharedState();
    auto AM = AchievementManager::sharedState();

    for(auto elem : m_textInput->getString())
       lower += std::tolower(elem);

    m_textInput->setString("");

    if(std::string_view(lower) == "throne room") {

    }

    if(std::string_view(lower) == std::string_view("attention") && !AM->isAchievementEarned("geometry.ach.lunar.vault01")) {
        response = "Hmmm..?";
        GM->reportAchievementWithID("geometry.ach.lunar.vault01", 100, false);
        return updateMessageLabel(response);
    }

    if(std::string_view(lower) == std::string_view("lucky") && !AM->isAchievementEarned("geometry.ach.lunar.vault02")) {
        response = "It\'s your lucky day!";
        GM->reportAchievementWithID("geometry.ach.lunar.vault02", 100, false);
        return updateMessageLabel(response);
    }

    if(std::string_view(lower) == std::string_view("stellar") && !AM->isAchievementEarned("geometry.ach.lunar.vault03")) {
        response = "Look into the stars...";
        GM->reportAchievementWithID("geometry.ach.lunar.vault03", 100, false);
        return updateMessageLabel(response);
    }

    updateMessageLabel(response);
}

std::string VaultLayer::getMessage() {
    int rand = std::rand() % 33 + 1;
    if(m_messageID == 0) {
        m_messageID = rand;
        m_messageIndex = 0;
        std::string message = getThreadMessage(m_messageIndex, m_messageID);
        if(message == "") message = getBasicMessage();
        m_messageIndex++;
        return message;
    } else {
        return getThreadMessage(m_messageIndex++, m_messageID);
    }
}

std::string VaultLayer::getThreadMessage(int index, int messageID) {
    if(messageID == 1) {
        switch(index) {
            case 0: return "Gah! You have too much free time";
            case 1: return "You have, like, so much levels available";
            case 2: return "Why won't you go play them?";
            case 3: return "So i can rest...";
            case 4: return "...";
            case 5: m_messageID = 0; m_messageIndex = 0; break;
        }
    }
    if(messageID == 2) {
        switch(index) {
            case 0: return "I'm going to sleep";
            case 1: return "...";
            case 2: return ".....";
            case 3: return "......!";
            case 4: return "Gah! I had a nightmare!";
            case 5: return "It was about you, how ironic";
            case 6: m_messageID = 0; m_messageIndex = 0; break;
        }
    }
    m_messageID = 0;
    m_messageIndex = 0;
    return "";
}

std::string VaultLayer::getBasicMessage() {
    int rand = std::rand() % 14 + 1;
    switch(rand) {
        case 1: return "Its you again...";
        case 2: return "Have you heard of personal space?!";
        case 3: return "...";
        case 4: return "Gah! You have too much free time";
        case 5: return "I'll stay here until the end of my life...";
        case 6: return "Go home";
        case 7: return "You test my patience";
        case 8: return "Stop bothering me";
        case 9: return "Leave me alone!";
        case 10: return "What are you doing?!";
        case 11: return "Go bother someone else.";
        case 12: return "Whats the point of this? What are you going to achieve?!";
        case 13: return "Who even let you in?!";
        case 14: return "Can you even read?!";
        default: return "";
    }
    return "";
}

void VaultLayer::onClose(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, LevelSelectLayer::scene(10)));
    GameManager::sharedState()->fadeInMenuMusic();
}

void VaultLayer::keyBackClicked() {
    onClose(nullptr);
}

VaultLayer* VaultLayer::create() {
    auto ret = new VaultLayer();
    if(ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCScene* VaultLayer::scene() {
    auto ret = CCScene::create();
    ret->addChild(VaultLayer::create());
    return ret;
}