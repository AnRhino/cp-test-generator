#include <bits/stdc++.h>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

struct TestRuntime {
    string testName;
    long long runtimeMs;
    int exitCode;
};

int extractTestId(const string& fileName) {
    static const regex pattern(R"(test(\d+)\.INP)", regex_constants::icase);
    smatch match;
    if (regex_match(fileName, match, pattern)) {
        return stoi(match[1].str());
    }
    return INT_MAX;
}

signed main() {
    const string sourceFile = "code.cpp";
#ifdef _WIN32
    const string executable = "code.exe";
    const string nullDevice = "NUL";
#else
    const string executable = "./code";
    const string nullDevice = "/dev/null";
#endif

    const string compileCommand = "g++ -std=c++17 -O2 \"" + sourceFile + "\" -o \"" + executable + "\"";
    const int compileExit = system(compileCommand.c_str());

    if (compileExit != 0) {
        cerr << "Compile failed with exit code: " << compileExit << "\n";
        return 1;
    }

    const fs::path testDir = "TEST";
    if (!fs::exists(testDir) || !fs::is_directory(testDir)) {
        cerr << "Cannot find TEST directory." << "\n";
        return 1;
    }

    vector<fs::path> inputFiles;
    for (const auto& entry : fs::directory_iterator(testDir)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        const fs::path filePath = entry.path();
        if (filePath.extension() == ".INP") {
            inputFiles.push_back(filePath);
        }
    }

    if (inputFiles.empty()) {
        cout << "No .INP test files found in TEST/." << "\n";
        return 0;
    }

    sort(inputFiles.begin(), inputFiles.end(), [](const fs::path& a, const fs::path& b) {
        const string nameA = a.filename().string();
        const string nameB = b.filename().string();

        const int idA = extractTestId(nameA);
        const int idB = extractTestId(nameB);

        if (idA != idB) {
            return idA < idB;
        }
        return nameA < nameB;
    });

    vector<TestRuntime> runtimes;
    runtimes.reserve(inputFiles.size());

    long long maxRuntimeMs = -1;
    string slowestTest;

    for (const auto& inputPath : inputFiles) {
        const string inputPathStr = inputPath.string();
        const string runCommand = "\"" + executable + "\" < \"" + inputPathStr + "\" > " + nullDevice;

        const auto start = chrono::steady_clock::now();
        const int runExit = system(runCommand.c_str());
        const auto finish = chrono::steady_clock::now();

        const long long elapsedMs = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
        const string fileName = inputPath.filename().string();

        runtimes.push_back({fileName, elapsedMs, runExit});

        if (elapsedMs > maxRuntimeMs) {
            maxRuntimeMs = elapsedMs;
            slowestTest = fileName;
        }
    }

    ofstream report("runtime_report.txt");
    if (!report.is_open()) {
        cerr << "Cannot write runtime_report.txt" << "\n";
        return 1;
    }

    cout << "Runtime (ms) of each test case:" << "\n";
    report << "Runtime (ms) of each test case:" << "\n";

    for (const auto& item : runtimes) {
        cout << item.testName << ": " << item.runtimeMs << " ms";
        report << item.testName << ": " << item.runtimeMs << " ms";

        if (item.exitCode != 0) {
            cout << " (exit code " << item.exitCode << ")";
            report << " (exit code " << item.exitCode << ")";
        }

        cout << "\n";
        report << "\n";
    }

    cout << "\nSlowest test case: " << slowestTest << " - " << maxRuntimeMs << " ms" << "\n";
    report << "\nSlowest test case: " << slowestTest << " - " << maxRuntimeMs << " ms" << "\n";

    report.close();

    cout << "Saved report to runtime_report.txt" << "\n";
    return 0;
}
