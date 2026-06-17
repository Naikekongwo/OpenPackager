#include "Core/Macros.hpp"
#include "OpenCore.hpp"
#include "OpenPackager/OpenPackager.hpp"
#include "Runtime/Animation/IAnimation.hpp"
#include "Runtime/Graphics/UI/ImageBoard.hpp"
#include <cstddef>
#include <memory>

PreloadStage::PreloadStage() {}

void PreloadStage::onEnter() { initializeComponents(); }

void PreloadStage::onExit()
{
    Elements->onDestroy();
    LOG("PreloadStage: onExit - cleared elements");
}

void PreloadStage::onDestroy() { LOG("PreloadStage: onDestroy"); }

bool PreloadStage::parseEvents(Event *event)
{
    Elements->parseEvents(event, timer->getTotalTime());
    return true;
}

void PreloadStage::onUpdate()
{
    Elements->onUpdate(timer->getTotalTime());
    pipeline.onUpdate(timer->getTotalTime());
}

void PreloadStage::onRender() { Elements->onRender(); }

void PreloadStage::initializeComponents()
{

    auto frameCounter = UI<FrameCounter>("frameCounter", 100, 0, 0, 0);
    frameCounter->Configure().Sequence(true);
    frameCounter->setFontName("Font_Eng");
    frameCounter->setFontSize(36);
    frameCounter->Animate().Timer(6.0f).Commit();
    Elements->PushElement(std::move(frameCounter));

    auto logo = UI<ImageBoard>("core_logo", 5, 0, 0, 0);
    logo->Configure()
        .Parent(nullptr)
        .Anchor(AnchorPoint::Center)
        .Posite(0.5f, 0.5f)
        .Scale(0.5f, 0.25f * widthheight)
        .Alpha(1.0f);

    logo->changeTexture(MakeTextureFromPkg(1, 1, "CORE_LOGO"));

    Elements->PushElement(std::move(logo));

    pipeline.next([this]() -> bool { return true; })
        .next(
            [this]() -> bool
            {
                auto *framecounter = Elements->find("frameCounter");
                return framecounter && framecounter->isAnimeFinished();
            })
        .next(
            [this]() -> bool
            {
                sController->removeStage(StageType::overlayStage);

                auto workstage = std::make_unique<WorktableStage>();
                sController->changeStage(std::move(workstage));
                return true;
            });
}