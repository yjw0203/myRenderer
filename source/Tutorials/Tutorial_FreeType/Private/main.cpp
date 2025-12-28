#include "font_renderer.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");  // 设置控制台为 UTF-8
#endif

    ChineseFontRenderer renderer;

    // 设置字体路径
    std::string fontPath = "C:/Windows/Fonts/simhei.ttf";  // 将字体文件放在可执行文件同目录

    std::cout << "Loading font: " << fontPath << std::endl;

    if (!renderer.loadFont(fontPath, 36)) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // 测试字符串
    std::vector<std::string> testStrings = {
        "123 ABC abc",
        "Hello, World!",
        "测试换行\nTest",
        "🍎🎉🚀 Unicode 表情符号"
    };

    for (size_t i = 0; i < testStrings.size(); ++i) {
        std::cout << "\nTest " << (i + 1) << ": " << testStrings[i] << std::endl;

        auto bitmap = renderer.renderText(testStrings[i], 4);

        if (bitmap.data.empty()) {
            std::cout << "Render failed!" << std::endl;
            continue;
        }

        std::cout << "Bitmap size: " << bitmap.width << " x " << bitmap.height << std::endl;

        // Save to file
        std::string filename = "output_" + std::to_string(i) + ".pgm";
        if (renderer.saveAsPNM(bitmap, filename)) {
            std::cout << "Saved to: " << filename << std::endl;
        }
    }

    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}