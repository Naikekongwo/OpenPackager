#include "OpenPackager/OpenPackager.hpp"
#include "Asset/PackageManager.hpp"
#include "OpenCore.hpp"
#include "OpenPackager/Stage/PreloadStage.hpp"
#include <exception>
#include <iostream>
#include <memory>

void OpenPackager::StartUp()
{
    // 初始化OpenPackager
    (void)OpenEngine::getInstance();

    auto &OCore = OpenEngine::getInstance();

    auto preloadStage = std::make_unique<PreloadStage>();

    GraphicsInfo gInfo;
    gInfo.nearestScaling   = false;
    gInfo.resolutionWidth  = 3840;
    gInfo.resolutionHeight = 2160;
    gInfo.targetFrameRate  = 30;
    gInfo.keepRatio        = true;
    gInfo.resizable        = false;

    ResourceInfo rInfo;
    rInfo.packageOnly                = false;
    rInfo.keepStructureWhenPackaging = true;
    rInfo.textureResampleFactor      = 1.0f;

    auto OpenPackagerInfo           = std::make_unique<GameInfo>();
    OpenPackagerInfo->_graphicsInfo = gInfo;
    OpenPackagerInfo->_resourceInfo = rInfo;
    OpenPackagerInfo->beta          = true;
    OpenPackagerInfo->version_major = 0;
    OpenPackagerInfo->version_minor = 1;
    OpenPackagerInfo->entranceStage = std::move(preloadStage);
    OpenPackagerInfo->gameName      = "OpenPackager 打包工具";

    OCore.GameRegistry(std::move(OpenPackagerInfo));

    if (!OCore.Initialize())
    {
        LOG("OpenCore 引擎在初始化时出现问题");
        return;
    }

    auto Package = OCore.getPackageManager();

    Package->registerResource(RscFont, "Font_Eng",
                              "assets/ui/OpenCoreFont.ttf");
    Package->registerResource(RscTexture, "CORE_LOGO",
                              "assets/ui/opencore_logo.png");
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