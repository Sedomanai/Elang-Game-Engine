#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <algorithm>
#include <type_traits>

#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <chrono>
#include <random>
#include <cmath>
#include <limits>
#include <functional>
#define NOMINMAX
#ifdef _WIN32
#include "windows.h"
#endif

#pragma warning( push )
#pragma warning( disable : 26495 )
#pragma warning( push )
#pragma warning( disable : 4267 )
#pragma warning( push )
#pragma warning( disable : 4334 )
#include <SDL/SDL.h>
#include <GL/glew.h> // This MUST come first (for QT)
#include <tsl/robin_map.h>
#include <tsl/robin_set.h>
#include <entt/entt.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/common.hpp>
#include <encoder/fpng.h>
#include <lodepng/lodepng.h>
#include <tweeny/tweeny.h>
#pragma warning( pop )
#pragma warning( pop )
#pragma warning( pop )

#include "elang_builder.h"
#include "common/define.h"
#include "common/string.h"
#include "common/vector.h"
#include "common/fileio.h"
#include "common/batch.h"
#include "tools/registry.h"
#include "tools/controls.h"
#include "tools/hashmap.h"

