#include "argParse.h"

using namespace std;
using namespace ap;

int main(int argc, char* argv[]) {
	ArgParse ap("APP NAME", "APP DESCRIPTION");

	ap.addArg<int>({"-i", "--int"}, [](const auto& val) {
		cout << val << endl;
	}, 13, "int val");
	ap.addArg<float>({"-f", "--float"}, [](const auto& val) {
		cout << val << endl;
	}, 0.0f, "float val");
	ap.addArg<bool>({"-b", "--bool"}, [](const auto& val) {
		cout << val << endl;
	}, 0.0f, "bool val");
	ap.addArg<std::string>({"-s"}, [](const auto& val) {
		cout << val << endl;
	}, "", "string val");

	ap.parse(argc, argv);
	return 0;
}