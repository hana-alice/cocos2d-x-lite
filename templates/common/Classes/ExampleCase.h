#include "GameAgent.h"
#include <vector>
class ExampleCase {
public:
    void init();

    void update();

private:
    std::vector<cc::CMIData> _simulateDatas;
};