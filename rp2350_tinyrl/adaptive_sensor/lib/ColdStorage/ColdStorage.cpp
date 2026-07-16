#include "ColdStorage.h"

bool ColdStorage::begin(String customQName, String customLogName)
{

    // Start SD
    if (!SD.begin(SS))
    {
        D_println("SD Card Failure. Check that it's connected.");
        return false;
    }

    // Make required folders for q tables and logs
    if (!SD.exists("/q_files"))
        SD.mkdir("/q_files");
    if (!SD.exists("/logs"))
        SD.mkdir("/logs");

    // If custom file names have been provided, use those. If not, use sequential naming
    if (customQName.isEmpty())
        setQFileName(findFileNo("/q_files/q_tables"));
    else
        setQFileName(String("/q_files/" + customQName + ".txt"));

    if (customLogName.isEmpty())
        setLogFileName(findFileNo("/logs/log"));
    else
        setLogFileName(String("/logs/" + customLogName + ".txt"));

    setStatus(true);
    return true;
}

String ColdStorage::findFileNo(String dir)
{
    int num = 1;
    String name = dir + String(num) + ".txt";

    while (SD.exists(name))
    {
        num++;
        name = dir + String(num) + ".txt";
    }

    return name;
}

bool ColdStorage::writeQTable(std::vector<std::vector<float>> &q)
{

    if (SD.exists(getQFileName()))
        SD.remove(getQFileName());

    File f = SD.open(getQFileName(), FILE_WRITE);
    if (!f)
    {
        D_println("Failed to open q table file: " + getQFileName());
        return false;
    }

    for (std::vector<float> line : q)
    {
        for (float weight : line)
        {
            f.print(weight);
            f.print(',');
        }
        f.print("\n");
    }

    f.close();
    return true;
}

std::vector<std::vector<float>> ColdStorage::getLatestQTable()
{
    Serial.println("KYS");
    std::vector<std::vector<float>> q;

    int num = 1;
    int latest = -1;
    String fileName = String("/q_files/q_tables") + num + ".txt";

    // Find Q file with the highest iterator that has content saved to it/
    while (true)
    {
        fileName = String("/q_files/q_tables") + num + ".txt";

        if (!SD.exists(fileName))
            break;

        File f = SD.open(fileName, FILE_READ);

        if (f)
        {
            if (f.size() > 0)
            {
                latest = num;
                f.close();
                num++;
                continue;
            }

            f.close();
        }

        break;
    }

    if (latest == -1)
    {
        // No valid Q-table found, return empty table
        Serial.println("Failed to open Q-table");
        return q;
    }

    fileName = String("/q_files/q_tables") + latest + ".txt";
    File f = SD.open(fileName, FILE_READ);

    if (!f)
    {
        Serial.println("Failed to open Q-table");
        return q;
    }

    Serial.println("READING FROM FILE: " + fileName);

    while (f.available())
    {
        String line = f.readStringUntil('\n');

        std::vector<float> row;

        int start = 0;
        int comma;

        while ((comma = line.indexOf(',', start)) != -1)
        {
            row.push_back(line.substring(start, comma).toFloat());
            start = comma + 1;
        }

        if (!row.empty())
            q.push_back(row);
    }

    f.close();

    return q;
}
