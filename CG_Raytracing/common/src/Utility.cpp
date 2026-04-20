#include <Utility.hpp>

#if defined(_MSC_VER)
	#include <intrin.h>
#elif defined(__linux__)
	#include <signal.h>
#endif

namespace cg_raytracing {
	namespace common {
		void DebugBreak() {
#if defined(_MSC_VER)
			__debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
			raise(SIGTRAP);
#else
#error "Unrecognized compiler"
#endif // _MSVC_VER
		}
	}
}
