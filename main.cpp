#include "argParse.h"

using namespace std;
using namespace ap;


int main(int argc, char* argv[]) {
    ArgParse ap(L"TEST ARG PARSE");

    ap.addArg(std::vector<std::wstring>{L"-i", L"--int"}, PayloadType::TYPE_INT, [&ap]() {
        auto res = ap.getPayload(L"--int");
        cout << std::get<int>(res.value());
    }, 13, L"int val");

    ap.addArg(std::vector<std::wstring>{L"-f", L"--float"}, PayloadType::TYPE_FLOAT, [&ap]() {
        auto res = ap.getPayload(L"--float");
        cout << std::get<float>(res.value());
        }, None{}, L"float val");
    ap.addArg(std::vector<std::wstring>{L"-b", L"--bool"}, PayloadType::TYPE_BOOL, [&ap]() {
        auto res = ap.getPayload(L"--bool");
        cout << std::get<bool>(res.value());
    }, None{}, L"bool val");

    ap.addArg(std::vector<std::wstring>{L"-s", L"--str"}, PayloadType::TYPE_WSTRING, [&ap]() {
        auto res = ap.getPayload(L"--str");
        wcout << std::get<std::wstring>(res.value());
    }, L"!!!", L"str val");

    ap.parse(argc, argv);
    return 0;
}
