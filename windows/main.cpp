#include "httplib.h"
#include "nlohmann/json.hpp"
#include "pmm.hpp"

using json = nlohmann::json;

int main() {
  httplib::Server svr;
  ProcessMemoryManager pmm;
  svr.Post("/",
           [&](const httplib::Request &req, httplib::Response &res) {
             try {
               json data = json::parse(req.body);
               if (data.contains("processId") && data.contains("type") && data.contains("address")) {
                 int processId = data["processId"].get<int>();
                 std::string type = data["type"].get<std::string>();
                 int address = data["address"].get<int>();

                 bool success = false;
                 json result_data = nullptr;

                 if (type == "uint8") {
                   uint8_t value = 0;
                   success = pmm.Read(processId, address, value);
                   if (success) result_data = value;
                 } else if (type == "uint16") {
                   uint16_t value = 0;
                   success = pmm.Read(processId, address, value);
                   if (success) result_data = value;
                 } else if (type == "uint32") {
                   uint32_t value = 0;
                   success = pmm.Read(processId, address, value);
                   if (success) result_data = value;
                 } else if (type == "uint64") {
                   uint64_t value = 0;
                   success = pmm.Read(processId, address, value);
                   if (success) result_data = value;
                 } else if (type == "bytes" && data.contains("size")) {
                    int size = data["size"].get<int>();
                    if (size > 0) {
                      std::vector<uint8_t> buffer(size);
                      success = pmm.Read(processId, address, buffer);
                      if (success) result_data = buffer;
                    }
                 } else if (type == "utf16") {
                    std::wstring value;
                    success = pmm.Read(processId, address, value);
                    if (success) {
                        std::vector<int> charCodes;
                        for (wchar_t wc : value) {
                            charCodes.push_back(static_cast<int>(wc));
                        }
                        result_data = charCodes;
                    }
                 }

                 res.set_content(json{{"data", result_data}, {"success", success}}.dump(), "application/json");

               } else {
                 res.set_content(json{{"data", nullptr}, {"success", false}}.dump(), "application/json");
               }
             } catch (const nlohmann::json::parse_error &e) {
               res.set_content(json{{"data", nullptr}, {"success", false}}.dump(), "application/json");
             }
           }
  );
  svr.listen("127.0.0.1", 20258);
}
