#pragma once

#if defined(__clang__)
#define Meta(...) __attribute__((annotate("CR"#__VA_ARGS__)))
#define Property(...) public: Meta(...)
#define PROPERTY(...) public: Meta(...)
#define Method(...) public: Meta(...)
#define METHOD(...) public: Meta(...)
#else
#define Meta(...)
#define Property(...) public: Meta(...)
#define PROPERTY(...) public: Meta(...)
#define Method(...) public: Meta(...)
#define METHOD(...) public: Meta(...)
#endif
