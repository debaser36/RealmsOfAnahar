#include "pchGE.h"
#include "GeneralUtil.h"


int main()
{
    al_init();
    auto allResolutions = ROA::UTIL::getAllResolutions();
    for(auto res : allResolutions)
    {
        std::cout << "w: " << res.width 
            << " h: " << res.height
            << " bbP: " << res.bbP
            << " freq: " << res.frequenzy <<
            std::endl;
    }
    std::cout << std::endl;

    auto res = ROA::UTIL::getCurrentResolution();
    std::cout << "w: " << res.width 
            << " h: " << res.height
            << " bbP: " << res.bbP
            << " freq: " << res.frequenzy <<
            std::endl;
}
