#pragma once

#include <any>
#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <optional>
#include <string>

//#define USE_THROW

namespace ap {
	class ArgParse {
	public:
		using HelpPrinter = std::function<void(const std::string&, const std::string& epilog)>;
		using ArgStr = std::string;
		using TypeId = std::string;
		using Payload = std::optional<std::any>;
		using PayloadPtr = std::shared_ptr<Payload>;
		using ArgCallback = std::function<void(const std::any&)>;
		using ArgCallbackPtr = std::shared_ptr<ArgCallback>;
		using ArgParseBeh = std::function<void(const ArgStr&, std::map<ArgStr, PayloadPtr>&, int&, int, char* argv[])>;
	
		ArgParse(const std::string& name = "", const std::string& description = "", const std::string& epilog = ""):
			_allDescription(name + "\n\n" + description + "\n\n"), _epilog(epilog) {
			init();
		}
	
		~ArgParse() = default;
	
		template<class T>
		[[nodiscard]] std::optional<T> getPayload(const ArgStr& key) const {
			if (!_payloadMap.contains(key)) {
				printErrorOrThrow("Error: getPayload - wrong key");
				return std::nullopt;
			}
			if (_argsDataTypeName.at(key) != typeid(baseType<T>).name()) {
				printErrorOrThrow("Error: getPayload - wrong type");
				return std::nullopt;
			}
			return std::any_cast<T>(_payloadMap.at(key));
		}
		
		[[nodiscard]] const TypeId& getPayloadTypeStr(const ArgStr& key) const {
			if (!_argsDataTypeName.contains(key)) {
				printErrorOrThrow("Error: getPayloadTypeStr - wrong key");
				static const std::string def;
				return def;
			}
			return _argsDataTypeName.at(key);
		}
	
		template<class T>
		void addArg(const std::vector<ArgStr>& keys, std::function<void(const T&)> fun = nullptr, const std::optional<T>& defPayload = std::nullopt, const std::string& description = "") {
			auto wrap = [fun=std::move(fun)](const std::any& wrappedVal) {
				if (fun) {
					fun(std::any_cast<T>(wrappedVal));
				}
			};
			auto wrapPtr = std::make_shared<ArgCallback>(wrap);
			const auto typeStr = typeid(baseType<T>).name();
			std::string argDescription = "";
			int i = 0;
			for (const auto& k : keys) {
				_argsCallbackMap[k] = wrapPtr;
				_argsDataTypeName[k] = typeStr;
				argDescription += k + ((i == keys.size() - 1) ? "" : ", ");
				_payloadMap[k] = std::make_shared<Payload>(defPayload);
				++i;
			}
			_allDescription += argDescription + " - ";
			_allDescription += description + "\n";
		}
	
		void parse(int argc, char* argv[], bool needRun = true) {
			if (argc <= 1) {
				return;
			}
			auto i = 1;
			while (i < argc) {
				std::string cmd(argv[i]);
				
				if (!_argsDataTypeName.contains(cmd)) {
					printErrorOrThrow("Error: bad args key: " + cmd);
					return;
				}
	
				if (cmd == "-h" || cmd == "--help") {
					if (_helpPrinter) {
						_helpPrinter(_allDescription, _epilog);
					}
					// I think it is normal behaviour
					return;
				}
	
				if (!_typeBehMap.contains(_argsDataTypeName.at(cmd))) {
					printErrorOrThrow("Error: parse try parse not supported type for command: " + cmd);
					//return, because all next strings could be corrupted
					return;
				}
	
				_typeBehMap.at(_argsDataTypeName.at(cmd))(cmd, _payloadMap, i, argc, argv);
	
				if (needRun) {
					run(cmd);
				}
				i++;
			}
		}
	
		void run(const std::string& key) const {
			const auto res = _argsCallbackMap.find(key);
			if (_argsCallbackMap.contains(key)) {
				auto& callback = _argsCallbackMap.at(key);
				if (callback) {
					if (const auto& valPtr = _payloadMap.at(key)) {
						if (valPtr && valPtr->has_value()) {
							(*callback)(valPtr->value());
						}
					}
				}
			}
		}
	
		[[nodiscard]] const std::string& getHelp() const {
			return _allDescription;
		}
	
		void setCustomHelpPrinter(HelpPrinter printer) {
			_helpPrinter = std::move(printer);
		}
	
		template<class T>
		void addTypeParser(ArgParseBeh beh) {
			_typeBehMap[typeid(baseType<T>).name()] = std::move(beh);
		}
	
	private:
		void init() {
			//add -h --help info key
			_allDescription += "--h -help : print information about program arguments\n";

			setCustomHelpPrinter([](const std::string& data, const std::string& epilog) {
				std::cout << data << "\n\n" << epilog << std::endl;
			});
	
			addTypeParser<bool>([](const ArgStr& cmd, std::map<ArgStr, PayloadPtr>& payloadMap, int&i, int argc, char* argv[]) {
				payloadMap[cmd] = std::make_shared<Payload>(true);
			});
	
			addTypeParser<int>([](const ArgStr& cmd, std::map<ArgStr, PayloadPtr>& payloadMap, int& i, int argc, char* argv[]) {
				i++;
				if (i < argc) {
					payloadMap[cmd] = std::make_shared<Payload>(std::stoi(argv[i]));
				}
				else {
					printErrorOrThrow("Error: addTypeParser<int> bad int param");
				}
			});
	
			addTypeParser<float>([](const ArgStr& cmd, std::map<ArgStr, PayloadPtr>& payloadMap, int& i, int argc, char* argv[]) {
				i++;
				if (i < argc) {
					payloadMap[cmd] = std::make_shared<Payload>(std::stof(argv[i]));
				}
				else {
					printErrorOrThrow("Error: addTypeParser<float> bad float param");
				}
			});
	
			addTypeParser<std::string>([](const ArgStr& cmd, std::map<ArgStr, PayloadPtr>& payloadMap, int& i, int argc, char* argv[]) {
				i++;
				if (i < argc) {
					payloadMap[cmd] = std::make_shared<Payload>(std::string(argv[i]));
				} else {
					std::cerr << "Err: addTypeParser<std::string> bad string param";
				}
			});
		}
	
		inline static void printErrorOrThrow(const std::string& errorStr) {
#ifdef USE_THROW
			throw std::runtime_error(errorStr);
#else
			std::cerr << errorStr;
#endif
		}
	
		template<typename T>
		using baseType = std::remove_cv_t<std::remove_reference_t<T>>;
	
		HelpPrinter _helpPrinter;
		std::string _allDescription;
		std::string _epilog;
	
		std::map<TypeId, ArgParseBeh> _typeBehMap;
	
		std::map<ArgStr, ArgCallbackPtr> _argsCallbackMap;
		std::map<ArgStr, TypeId> _argsDataTypeName;
		std::map<ArgStr, PayloadPtr> _payloadMap;
	};
};
	