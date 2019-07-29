#include "argParse.h"

using namespace std;
using namespace ap;


int main(int argc, char* argv[]) {
    ArgParse ap(L"TEST ARG PARSE");

    ap.addArg(std::vector<std::wstring>{L"-i", L"--int"}, PayloadType::TYPE_INT, [&ap]() {
        auto res = ap.payloadMap.find(L"--int");
        cout << (*(*res).second).intVal;
    }, new Payload(13), L"int val");

    ap.addArg(std::vector<std::wstring>{L"-f", L"--float"}, PayloadType::TYPE_FLOAT, [&ap]() {
        auto res = ap.payloadMap.find(L"--float");
        cout << (*(*res).second).floatVal;
    }, nullptr, L"float val");
    ap.addArg(std::vector<std::wstring>{L"-b", L"--bool"}, PayloadType::TYPE_BOOL, [&ap]() {
        auto res = ap.payloadMap.find(L"--bool");
        cout << (*(*res).second).boolVal;
    }, nullptr, L"bool val");

    ap.addArg(std::vector<std::wstring>{L"-s", L"--str"}, PayloadType::TYPE_WSTRING, [&ap]() {
        auto res = ap.payloadMap.find(L"--str");
        wcout << (*(*res).second).wstrVal;
    }, new Payload("!!!"), L"str val");

    ap.parse(argc, argv);
    return 0;
}
