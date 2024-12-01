#include "entry.h"
#include "days/aoc_days.h"

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
    
    #define INVOKE_DAY(number) case number: day##number("inputs/day"#number".txt"); break;

    switch (dayToRun)
    {
        INVOKE_DAY(1);
        INVOKE_DAY(2);
        INVOKE_DAY(3);
        INVOKE_DAY(4);
        INVOKE_DAY(5);
        INVOKE_DAY(6);
        INVOKE_DAY(7);
        INVOKE_DAY(8);
        INVOKE_DAY(9);
        INVOKE_DAY(10);
        INVOKE_DAY(11);
        INVOKE_DAY(12);
        INVOKE_DAY(13);
        INVOKE_DAY(14);
        INVOKE_DAY(15);
        INVOKE_DAY(16);
        INVOKE_DAY(17);
        INVOKE_DAY(18);
        INVOKE_DAY(19);
        INVOKE_DAY(20);
        INVOKE_DAY(21);
        INVOKE_DAY(22);
        INVOKE_DAY(23);
        INVOKE_DAY(24);
        INVOKE_DAY(25);
            
        default:
            std::cout << "Invalid day\n";
            break;
    }

    std::cout << std::endl << std::endl;
}

}
