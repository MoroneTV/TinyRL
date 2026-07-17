#include <unity.h>
#include <ColdStorage.h>
#include <vector>

ColdStorage coldStorage;

bool qTablesEqual(const std::vector<std::vector<float>>& a,
                  const std::vector<std::vector<float>>& b,
                  float tolerance = 0.01f)
{
    if (a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); i++)
    {
        if (a[i].size() != b[i].size())
            return false;

        for (size_t j = 0; j < a[i].size(); j++)
        {
            if (abs(a[i][j] - b[i][j]) > tolerance)
            {
                Serial.print("Mismatch at [");
                Serial.print(i);
                Serial.print("][");
                Serial.print(j);
                Serial.print("]: ");
                Serial.print(a[i][j]);
                Serial.print(" vs ");
                Serial.println(b[i][j]);

                return false;
            }
        }
    }

    return true;
}


void test_SetupStatus()
{
    coldStorage.begin();
    TEST_ASSERT(coldStorage.getStatus() == true);
}

void test_QFileNameFormatting()
{
    TEST_ASSERT((coldStorage.getQFileName().indexOf("q_table") > 0));
}

void test_LogFileNameFormatting()
{
    TEST_ASSERT((coldStorage.getLogFileName().indexOf("log") > 0));
}

void test_writeAndReadQTable()
{
    int rows = random(1, 30);
    int cols = random(1, 30);

    std::vector<std::vector<float>> q(
        rows,
        std::vector<float>(cols));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            q[i][j] = random(1, 10) / (float)random(1, 10);
        }
    }

    // Write table to storage, check if it can be read back.
    coldStorage.writeQTable(q);
    std::vector<std::vector<float>> q_ = coldStorage.getLatestQTable();

    TEST_ASSERT(qTablesEqual(q,q_));
}

void writeLog()
{
}

void setup()
{
    Serial.begin(115200);
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_SetupStatus);
    RUN_TEST(test_QFileNameFormatting);
    RUN_TEST(test_LogFileNameFormatting);
    RUN_TEST(test_writeAndReadQTable);
    UNITY_END();
}

void loop()
{
}