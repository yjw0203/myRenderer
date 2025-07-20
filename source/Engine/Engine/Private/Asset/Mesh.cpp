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
        /*
        if (padding > 0 && padding != (4 - (encoded.size() % 4)) % 4) {
            throw std::invalid_argument("Invalid Base64 padding");
        }
        */
    }

    void to_json(json& j, const yjw::Transform& obj)
    {
        j = json{
            {"m_location", obj.m_location},
            {"m_rotate", obj.m_rotate},
            {"m_scale", obj.m_scale}
        };
    }

    void from_json(const json& j, yjw::Transform& obj)
    {
        if (j.count("m_location")) {
            j.at("m_location").get_to(obj.m_location);
        }
        if (j.count("m_rotate")) {
            j.at("m_rotate").get_to(obj.m_rotate);
        }
        if (j.count("m_scale")) {
            j.at("m_scale").get_to(obj.m_scale);
        }
    }

}

namespace glm
{
    void to_json(json& j, const yjw::Vector3& obj)
    {
        j = json{
            {"x", obj.x},
            {"y", obj.y},
            {"z", obj.z}
        };
    }

    void from_json(const json& j, yjw::Vector3& obj)
    {
        if (j.count("x")) {
            j.at("x").get_to(obj.x);
        }
        if (j.count("y")) {
            j.at("y").get_to(obj.y);
        }
        if (j.count("z")) {
            j.at("z").get_to(obj.z);
        }
    }

    void to_json(json& j, const yjw::Quaternion& obj)
    {
        j = json{
            {"x", obj.x},
            {"y", obj.y},
            {"z", obj.z},
            {"w", obj.w}
        };
    }

    void from_json(const json& j, yjw::Quaternion& obj)
    {
        if (j.count("x")) {
            j.at("x").get_to(obj.x);
        }
        if (j.count("y")) {
            j.at("y").get_to(obj.y);
        }
        if (j.count("z")) {
            j.at("z").get_to(obj.z);
        }
        if (j.count("w")) {
            j.at("w").get_to(obj.w);
        }
    }

    void to_json(json& j, const yjw::Matrix4x4& obj)
    {
        std::vector<float> vec;
        vec.resize(16);
        vec[0] = obj[0][0];
        vec[1] = obj[0][1];
        vec[2] = obj[0][2];
        vec[3] = obj[0][3];
        vec[4] = obj[1][0];
        vec[5] = obj[1][1];
        vec[6] = obj[1][2];
        vec[7] = obj[1][3];
        vec[8] = obj[2][0];
        vec[9] = obj[2][1];
        vec[10] = obj[2][2];
        vec[11] = obj[2][3];
        vec[12] = obj[3][0];
        vec[13] = obj[3][1];
        vec[14] = obj[3][2];
        vec[15] = obj[3][3];
        j = vec;
    }

    void from_json(const json& j, yjw::Matrix4x4& obj)
    {
        std::vector<float> vec = j.get<std::vector<float>>();
        obj[0][0] = vec[0];
        obj[0][1] = vec[1];
        obj[0][2] = vec[2];
        obj[0][3] = vec[3];
        obj[1][0] = vec[4];
        obj[1][1] = vec[5];
        obj[1][2] = vec[6];
        obj[1][3] = vec[7];
        obj[2][0] = vec[8];
        obj[2][1] = vec[9];
        obj[2][2] = vec[10];
        obj[2][3] = vec[11];
        obj[3][0] = vec[12];
        obj[3][1] = vec[13];
        obj[3][2] = vec[14];
        obj[3][3] = vec[15];
    }
}