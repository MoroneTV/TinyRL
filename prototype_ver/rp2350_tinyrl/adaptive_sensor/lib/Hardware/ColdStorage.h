#ifndef COLDSTORAGE_H
#define COLDSTORAGE_H

#include <cstddef>
#include <Arduino.h>
#include <vector>
#include <array>
#include <SD.h>
#include <ConfigValues.h>


class ColdStorage {
public:
    ColdStorage();

    bool begin();

    void setTargetRLFile(String filename) {targetRLFile = filename;}
    String getTargetRLFile() { return targetRLFile; }

    bool writeToRLFile(ConfigValues values);
    ConfigValues readFromRLFile();

private:
    String targetRLFile;
};

#endif // COLDSTORAGE_H
