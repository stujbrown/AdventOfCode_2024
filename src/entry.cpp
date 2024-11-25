#include "entry.h"
#include "days/days.h"

#include <stdlib.h>
#include <fstream>

namespace aoc
{

Args parseArgs(int numArgs, const char* args[])
{
    int dayToRun = -1;
    if (numArgs > 1)
    {
        dayToRun = atoi(args[1]);
    }
    return Args { .dayToRun = dayToRun };
}

void entry(const Args& args)
{
    int dayToRun = args.dayToRun;
    if (dayToRun <= 0)
    {
        std::cout << "Run day: ";
        std::cin >> dayToRun;
        std::cout << std::endl;
    }
    
    #define DAY(number) case number: day##number(); break;
    switch (dayToRun)
    {
        DAY(1);
        DAY(2);
        DAY(3);
        DAY(4);
        DAY(5);
        DAY(6);
        DAY(7);
        DAY(8);
        DAY(9);
        DAY(10);
        DAY(11);
        DAY(12);
        DAY(13);
        DAY(14);
        DAY(15);
        DAY(16);
        DAY(17);
        DAY(18);
        DAY(19);
        DAY(20);
        DAY(21);
        DAY(22);
        DAY(23);
        DAY(24);
        DAY(25);
            
        default:
            std::cout << "Invalid day\n";
            break;
    }
}

}
