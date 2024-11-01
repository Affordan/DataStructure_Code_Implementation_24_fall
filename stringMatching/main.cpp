#include "./include/String.h"
#include "./include/Timer.h"

string generateRandomString(size_t len, const string &charset) {
    string result;
    result.reserve(len);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> dis(0, charset.length() - 1);

    for (size_t i = 0; i < len; i++) {
        result += charset[dis(gen)];
    }
    return result;
}

void RandomStringTest() {
    // 字符集定义 - 只保留两种最具代表性的
    const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string ALL_CHARS = LETTERS + "0123456789!@#$%^&*()";
    
    vector<pair<string, string>> charsets = {
        {"Letters", LETTERS},
        {"All Chars", ALL_CHARS}
    };

    const size_t TEXT_LENGTH = 200000;
    const vector<size_t> PATTERN_LENGTHS = {
        80,     // 较短
        150,    // 中短
        300,    // 中等
        500,    // 中长
        1000    // 较长
    };
    
    vector<TestResult> allResults;
    cout << "Starting tests...\n";

    for (const auto& [charset_name, charset] : charsets) {
        cout << "\nTesting " << charset_name << " charset:\n";
        
        // 生成主串
        string text = generateRandomString(TEXT_LENGTH, charset);
        
        for (size_t patLen : PATTERN_LENGTHS) {
            cout << "Pattern length " << patLen << ": ";
            
            vector<pair<string, string>> patterns = {
                {"Front", text.substr(0, patLen)},
                {"Middle", text.substr(TEXT_LENGTH/2 - patLen/2, patLen)},
                {"Back", text.substr(TEXT_LENGTH - patLen, patLen)}
            };

            for (const auto& [position, pattern] : patterns) {
                Timer<> buildKMP, buildBM, runKMP, runBM;
                int kmpCount = 0, bmCount = 0;
                
                // KMP测试
                buildKMP.start();
                kmpString k(pattern);
                buildKMP.stop();
                
                runKMP.start();
                k.DFAsearchIn(text, kmpCount);
                runKMP.stop();
                
                // BM测试
                buildBM.start();
                bmString b(pattern);
                buildBM.stop();
                
                runBM.start();
                b.BMsearchIn(text, bmCount);
                runBM.stop();

                // 收集结果，时间转换为微秒(us)
                TestResult result;
                result.charset = charset_name;
                result.patternLength = patLen;
                result.position = position;
                result.kmpComparisons = kmpCount;
                result.bmComparisons = bmCount;
                result.kmpBuildTime = chrono::duration_cast<chrono::microseconds>(buildKMP.getDuration()).count();
                result.kmpSearchTime = chrono::duration_cast<chrono::microseconds>(runKMP.getDuration()).count();
                result.bmBuildTime = chrono::duration_cast<chrono::microseconds>(buildBM.getDuration()).count();
                result.bmSearchTime = chrono::duration_cast<chrono::microseconds>(runBM.getDuration()).count();
                
                allResults.push_back(result);
                cout << "." << flush;
            }
            cout << " done\n";
        }
    }

    // 打印结果表格
    cout << "\n\nResults Summary:\n";
    printResultTable(allResults);
}

int main() {
    cout << "Starting String Matching Algorithm Performance Test...\n\n";
    
    auto start = chrono::steady_clock::now();
    RandomStringTest();
    auto end = chrono::steady_clock::now();
    
    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout << "\nTotal test time: " << duration.count() << " seconds\n";
    
    cout << "\nPress Enter to exit...";
    cin.get();
    return 0;
}