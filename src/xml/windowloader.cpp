#include <SDL.h>

#include "tinyxml2.h"
#include "windowloader.h"
#include "util/log.h"

using namespace ldjam;

WindowConfig ldjam::config::LoadWindow(const char* filename)
{
    LD_LOG_DEBUG("Loading window config from %s", filename)
    tinyxml2::XMLDocument doc;
    
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS)
    {
        LD_LOG_INFO("File %s not found. Using default window config", filename);

        // Return default config
        WindowConfig defaultCfg;
        defaultCfg.x = SDL_WINDOWPOS_CENTERED;
        defaultCfg.y = SDL_WINDOWPOS_CENTERED;
        defaultCfg.width = 800;
        defaultCfg.height = 600;
        return defaultCfg;
    }

    WindowConfig cfg;
    auto windowElement = doc.FirstChildElement("Window");

    cfg.x       = windowElement->IntAttribute("x");
    cfg.y       = windowElement->IntAttribute("y");
    cfg.width   = windowElement->IntAttribute("width");
    cfg.height  = windowElement->IntAttribute("height");
    
    return cfg;
}

void ldjam::config::WriteWindow(WindowConfig cfg, FILE* fp)
{
    LD_LOG_DEBUG("Saving current window config");

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLPrinter printer(fp);
    
    printer.OpenElement("Window");

    printer.PushAttribute("x", cfg.x);
    printer.PushAttribute("y", cfg.y);
    printer.PushAttribute("width", cfg.width);
    printer.PushAttribute("height", cfg.height);
    
    printer.CloseElement();
    doc.Print(&printer);
}
