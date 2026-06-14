#include "OpenPackager/OpenPackager.hpp"
#include "OpenCore.hpp"
#include "OpenPackager/Stage/PreloadStage.hpp"
#include <exception>
#include <memory>

void OpenPackager::StartUp()
{
    // 初始化OpenPackager
    (void)OpenEngine::getInstance();

    auto &OCore = OpenEngine::getInstance();

    if (!OCore.Initialize())
    {
        LOG("OpenCore 引擎在初始化时出现问题");
        return;
    }

    auto timer = OCore.getTimer();
    auto sCon = OCore.getStageController();

    auto preloadStage = std::make_unique<PreloadStage>(timer, sCon);

    GraphicsInfo gInfo;
    gInfo.nearestScaling = false;
    gInfo.resolutionWidth = 3840;
    gInfo.resolutionHeight = 2160;
    gInfo.targetFrameRate = 144;

    auto OpenPackagerInfo = std::make_unique<GameInfo>();
    OpenPackagerInfo->_graphicsInfo = gInfo;
    OpenPackagerInfo->beta = true;
    OpenPackagerInfo->version_major = 0;
    OpenPackagerInfo->version_minor = 1;
    OpenPackagerInfo->entranceStage = std::move(preloadStage);
    OpenPackagerInfo->gameName = "OpenPackager 脚本设计器";

    OCore.GameRegistry(std::move(OpenPackagerInfo));

    try
    {
        OCore.MainLoop();
        OCore.CleanUp();
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        return;
    }
}