#include "ColdStorage.h"

ColdStorage::ColdStorage() {}

bool ColdStorage::begin() {
    return SD.begin(SS);
}

bool ColdStorage::writeToRLFile(ConfigValues values) {
    if (SD.exists(getTargetRLFile())) {
        SD.remove(getTargetRLFile());
    }

    File file = SD.open(getTargetRLFile(), FILE_WRITE);
    if (!file) {
        return false;
    }

    // Agent params
    file.println("CONSERVATISM: " + String(values.conservatism));
    // Weights (array of 3)
    file.println("WEIGHT_0: " + String(values.weights[0]));
    file.println("WEIGHT_1: " + String(values.weights[1]));
    file.println("WEIGHT_2: " + String(values.weights[2]));

    file.println("ENERGY_PER_EP: " + String(values.energyPerEp));
    file.println("MS_PER_EP: " + String(values.msPerEp));

    // Agent parameters
    file.println("EPSILON: " + String(values.epsilon));
    file.println("GAMMA: " + String(values.gamma));
    file.println("LR: " + String(values.lr));
    file.println("NO_OF_ACTIONS: " + String(values.noOfActions));
    file.println("NO_OF_STATES: " + String(values.noOfStates));

    // Q-table: write size then values
    file.println("QTABLE_SIZE: " + String(values.qTable.size()));
    for (size_t i = 0; i < values.qTable.size(); ++i) {
        file.println("Q_" + String(i) + ": " + String(values.qTable[i]));
    }

    file.close();
    return true;
}

ConfigValues ColdStorage::readFromRLFile() {
    ConfigValues values{};
    values.weights.fill(0.0f);
    values.qTable.clear();

    File file = SD.open(getTargetRLFile(), FILE_READ);
    if (!file) {
        return values;
    }

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) {
            continue;
        }

        int separator = line.indexOf(':');
        if (separator < 0) {
            continue;
        }

        String key = line.substring(0, separator);
        String value = line.substring(separator + 1);
        key.trim();
        value.trim();

        if (key == "CONSERVATISM") {
            values.conservatism = value.toFloat();
        } else if (key == "WEIGHT_0") {
            values.weights[0] = value.toFloat();
        } else if (key == "WEIGHT_1") {
            values.weights[1] = value.toFloat();
        } else if (key == "WEIGHT_2") {
            values.weights[2] = value.toFloat();
        } else if (key == "ENERGY_PER_EP") {
            values.energyPerEp = value.toFloat();
        } else if (key == "MS_PER_EP") {
            values.msPerEp = value.toInt();
        } else if (key == "EPSILON") {
            values.epsilon = value.toFloat();
        } else if (key == "GAMMA") {
            values.gamma = value.toFloat();
        } else if (key == "LR") {
            values.lr = value.toFloat();
        } else if (key == "NO_OF_ACTIONS") {
            values.noOfActions = value.toFloat();
        } else if (key == "NO_OF_STATES") {
            values.noOfStates = value.toFloat();
        } else if (key == "QTABLE_SIZE") {
            int qTableSize = value.toInt();
            if (qTableSize > 0) {
                values.qTable.clear();
                values.qTable.resize(qTableSize);
            }
        } else if (key.startsWith("Q_")) {
            int index = key.substring(2).toInt();
            if (index >= 0) {
                if (index >= (int)values.qTable.size()) {
                    values.qTable.resize(index + 1);
                }
                values.qTable[index] = value.toFloat();
            }
        }
    }

    file.close();
    return values;
}
