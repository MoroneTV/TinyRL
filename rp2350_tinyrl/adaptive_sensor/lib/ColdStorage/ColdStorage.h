#ifndef COLD_STORAGE_H
#define COLD_STORAGE_H

#include <Arduino.h>
#include <SD.h>
#include <vector>
#include "config.h"

class ColdStorage {
    public:
        ColdStorage();

        // Setup SD card and find names for the files. Optional custom names
        // But use the sequential naming scheme if not. use format ("NAME.txt")
        bool begin(String customQName = "", String customLogName = "");   

        // Read most recent Q table entry in SD card (if one exists)
        std::vector<std::vector<float>> getLatestQTable();
        // Write a Q table to the available Q file
        bool writeQTable(std::vector<std::vector<float>> q);

        bool getStatus()            { return status; }
        String getLogFileName()     { return log; }
        String getqFileName()       { return q; }

    private:
        bool status = false;
        String log = "null";    // Log file name
        String q = "null";      // Q table file name

        String findFileNo();

        void setStatus(bool s)              { status = s; }
        void setLogFileName(String name)    { log = name; }
        void setQFileName(String name)      { q = name; }
}

#endif