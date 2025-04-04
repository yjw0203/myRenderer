#include "Engine/Engine/Public/Asset/Mesh.h"

namespace yjw
{
    std::string base64_encode(const std::vector<uint8_t>& data) {
        static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        std::string result;
        int val = 0, valb = -6;

        for (uint8_t c : data) {
            val = (val << 8) + c;
            valb += 8;

            while (valb >= 0) {
                result.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) {
            result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        }
        while (result.size() % 4) {
            result.push_back('=');
        }
        return result;
    }

    void base64_decode(const std::string& encoded, std::vector<uint8_t>& out) {
        static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        std::unordered_map<char, int> char_to_index;
        for (int i = 0; i < 64; ++i) {
            char_to_index[base64_chars[i]] = i;
        }

        std::vector<uint8_t>& decoded = out;
        int val = 0, valb = -8;

        for (char c : encoded) {
            if (c == '=') break;

            auto it = char_to_index.find(c);
            if (it == char_to_index.end()) {
                throw std::invalid_argument("Invalid Base64 character: " + std::string(1, c));
            }

            int byte = it->second;
            val = (val << 6) + byte;
            valb += 6;

            if (valb >= 0) {
                decoded.push_back((val >> valb) & 0xFF);
                valb -= 8;
            }
        }

        int padding = 0;
        for (char c : encoded) {
            if (c == '=') ++padding;
        }
        if (padding > 0 && padding != (4 - (encoded.size() % 4)) % 4) {
            throw std::invalid_argument("Invalid Base64 padding");
        }
    }


    void to_json(json& j, const yjw::RawBuffer& obj) {
        j = base64_encode(obj);
    }

    void from_json(const json& j, yjw::RawBuffer& obj) {
        if (j.is_string())
        {
            std::string s = j.get<std::string>();
            base64_decode(s, obj);
        }
    }
}