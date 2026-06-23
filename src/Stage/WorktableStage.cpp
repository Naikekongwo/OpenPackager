#include "OpenPackager/Stage/WorktableStage.hpp"
#include "OpenCore.hpp"
#include "Runtime/Animation/IAnimation.hpp"
#include "Runtime/Graphics/Manager/ElementManager.hpp"
#include "Runtime/Graphics/UI/ToolMenu.hpp"
#include "World/Stage/BaseStage.hpp"
#include "World/Stage/StageManager.hpp"

WorktableStage::WorktableStage() {}

void WorktableStage::onEnter() { initializeComponents(); }

void WorktableStage::onExit()
{
    Elements->onDestroy();
    LOG("WorktableStage: onExit - cleared elements");
}

void WorktableStage::onDestroy() { LOG("WorktableStage: onDestroy"); }

bool WorktableStage::parseEvents(Event *event)
{
    Elements->parseEvents(event, timer->getTotalTime());
    return true;
}

void WorktableStage::onUpdate()
{
    Elements->onUpdate(timer->getTotalTime());
    pipeline.onUpdate(timer->getTotalTime());
}

void WorktableStage::onRender() { Elements->onRender(); }

void WorktableStage::initializeComponents()
{
    auto toolmenu = std::make_unique<ToolMenu>("toolmenu", 50);

    toolmenu->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::TopLeft)
        .Scale(1.0f, 120.0f)
        .Posite(0.0f, 0.0f);

    toolmenu->setBackgroundColor({0.5f, 1.0f, 1.0f, 1.0f});

    Elements->PushElement(std::move(toolmenu));

    auto text = std::make_unique<TextArea>("text", 99, "Font_Eng");

    text->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::Center)
        .Scale(1.0f, 0.2f)
        .Posite(0.5f, 0.5f);

    text->setFontSize(100);
    text->setText("We are the world!");
    text->alignCenter(true);

    Elements->PushElement(std::move(text));
}