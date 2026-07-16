#include "ColdStorage.h"

bool ColdStorage::begin(String customQName = "", String customLogName = "")
{

    if (!SD.begin(SS))
    {
        D_println("SD Card Failure. Check that it's connected.");
        return 0;
    }

    if (!customQName.isEmpty())
        setQFileName(findFileNo());
    else
    {
        setQFileName(String("/q_files/" + customQName));
    }

    if (!customLogName.isEmpty())
        setQFileName(findLogName());
    else
    {
        setQFileName(String("/q_files/" + customLogName));
    }

    return 1;
}

String ColdStorage::findFileNo()
{
    int num = 1;
    String name = String("/q_files/q_tables") + String(num);

    while (SD.exists(name))
    {
        num++;
        name = String("/q_files/q_tables") + String(num);
    }

    return name;
}

String ColdStorage::findLogName()
{
    int num = 1;
    String name = String("/logs/log") + String(num);

    while (SD.exists(name))
    {
        num++;
        name = String("/logs/log") + String(num);
    }

    return name;
}
