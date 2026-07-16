#include <unity.h>
#include <ColdStorage.h>
#include <vector>

ColdStorage coldStorage;

void test_SetupStatus()
{
    TEST_ASSERT(coldStorage.getStatus() == true)
}

void test_QFileNameFormatting()
{
    TEST_ASSERT((coldStorage.getQFileName().indexOf("QTables") > 0))
}

void test_LogFileNameFormatting()
{
    TEST_ASSERT((coldStorage.getLogFileName().indexOf("Log") > 0))
}

void writeAndReadQTable()
{   
    // initialise random Qtable stand-in
    std::vector<std::vector<float>> q;
    for(int i = 0; i < random(30); i++) {
        for(int j = 0; j < random(30); j++) {
            q[i][j] = random(1, 10) / random(1, 10);
        }
    }

    // Write table to storage, check if it can be read back.
    coldStorage.writeQTable(q);
    TEST_ASSERT(coldStorage.getLatestQTable() == q);
}

void writeLog()
{}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(fuelGuage_is_initially_empty);
    return UNITY_END();
}
