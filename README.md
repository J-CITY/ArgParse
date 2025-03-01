# ArgParse

Simple arg parser for C++


## Base example
```c++
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
```

## Parse argument by key
```c++
#include "argParse.h"

using namespace std;
using namespace ap;

int main(int argc, char* argv[]) {
	ArgParse ap("APP NAME", "APP DESCRIPTION", "APP EPILOG");
	ap.addArg<int>({"-i", "--int"}, [](const auto& val) {
		cout << val << endl;
	}, 13, "int val");

	// set 'false' flag for don`t call callbacks when args parse
	ap.parse(argc, argv, false);
	...
	// get argument peyload by key
	const auto& payload = ap.getPayload("-i");

	// get argument payload type string by key
	const auto& typeStr = ap.getPayloadTypeStr("-i");

	// run argument callback by key
	ap.run("-i");

	return 0;
}
```


## Other features
```c++
#include "argParse.h"

using namespace std;
using namespace ap;

int main(int argc, char* argv[]) {
	ArgParse ap("APP NAME", "APP DESCRIPTION", "APP EPILOG");
	ap.addArg<int>({"-i", "--int"}, [](const auto& val) {
		cout << val << endl;
	}, 13, "int val");

	// set custom print app information, when call with '-h' or '--help' argument 
	setCustomHelpPrinter([](const std::string& data, const std::string& epilog) {
		std::cout << data << "\n\n" << epilog << std::endl;
	});

	// set custom type for parse
	addTypeParser<std::vector<int>>([](
		const ArgStr& cmd, 
		std::map<ArgStr, PayloadPtr>& payloadMap, 
		int&i, int argc, char* argv[])\
	{
		// some code
	});

	ap.parse(argc, argv, );

	return 0;
}
```
