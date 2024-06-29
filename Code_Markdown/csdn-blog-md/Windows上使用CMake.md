
2016-03-04
CMake简介

你或许听过好几种 Make 工具，例如 GNU Make ，QT 的 qmake ，微软的 MS nmake，BSD Make（pmake），Makepp，等等。这些 Make 工具遵循着不同的规范和标准，所执行的 Makefile 格式也千差万别。这样就带来了一个严峻的问题：如果软件想跨平台，必须要保证能够在不同平台编译。而如果使用上面的 Make 工具，就得为每一种标准写一次 Makefile ，这将是一件让人抓狂的工作。

CMake就是针对上面问题所设计的工具：它首先允许开发者编写一种平台无关的 CMakeList.txt 文件来定制整个编译流程，然后再根据目标用户的平台进一步生成所需的本地化 Makefile 和工程文件，如 Unix 的 Makefile 或 Windows 的 Visual Studio 工程。从而做到“Write once, run everywhere”。
Windows上使用CMake

Windows上使用CMake也很方便，除了传统的命令行方式使用CMake，还有一个简单的GUI程序cmake-gui.exe来使用CMake。
安装CMake
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719191741660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

Windows上安装CMake很简单，去https://cmake.org/ 上面下载最新的CMake安装包就可以了。安装的时候还可以选择是否把CMake加到系统的PATH中，如下图所示： use-cmake-on-windows

为了方便起见，可以把CMake加到系统PATH中。
CMake的GUI用法

安装好CMake，会创建一个快捷方式，点击运行就会运行CMake-gui.exe，这个是CMake的GUI程序，以下图所示： use-cmake-on-windows
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719191806800.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

source code编辑框就是输入代码的所在的路径，这个路径能够找到一个CMakeLists.txt文件。

build the binaries编辑框就是编译输出的中间文件和最终的二进制文件的目录。

因为CMake最终通过CMakeLists.txt文件生成Windows上对应的vs工程文件，不同的vs版本也会影响到最终生成vs工程文件，所以configure对话框就是选择代码编译工具的，如图所示： use-cmake-on-windows
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719191815913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

下面以google test工程的代码为例来使用CMake-gui，输入google test对应的路径，点击Generate按钮就会在E:/googletest/googletest/build目录生成vs编译工程文件： use-cmake-on-windows

use-cmake-on-windows
![在这里插入图片描述](https://img-blog.csdnimg.cn/202007191918220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20200719191831931.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


用vs打开gtest.sln文件，就可以编译google test代码了。
CMake的命令行用法

CMake的命令行用法比GUI的用法复杂，但是功能更加强大，值得一学。以下是CMake命令行调用的方法：

cmake [<options>] (<path-to-source> | <path-to-existing-build>)
cmake [(-D<var>=<value>)...] -P <cmake-script-file>
cmake --build <dir> [<options>] [-- <build-tool-options>...]
cmake -E <command> [<options>...]
cmake --find-package <options>...

生成编译工程文件

cmake 就是生成可以编译工程文件。当时运行的目录在哪里，生成的可编译工程文件就在哪个目录。比如CMakeLists.txt文件在f:cmake目录，而当时在f:cmakebuild目录运行cmake ..，则生成的编译工程文件在f:cmakebuild目录。

也可以再生成编译工程文件的时候通过-D来添加变量值，比如CMakeLists.txt内容如下：

cmake_minimum_required (VERSION 2.6)
project (a)
message(${EXECUTABLE_OUTPUT_PATH})
add_executable(b b.cpp)

我们可以通过-D选择来设置EXECUTABLE_OUTPUT_PATH的值，也是编译的文件的输出目录：

cmake -D EXECUTABLE_OUTPUT_PATH="another_output" ..

这样，我们就给CMakeLists.txt编译脚本传递了新的EXECUTABLE_OUTPUT_PATH值。
编译工程

CMake除了生成编译工程文件，它也可以调用系统的编译工程来编译工程，如：

cmake --build .

默认是编译debug模式，也可以传递在–后面传递MSBUILD参数来控制：

cmake --build . -- /p:Configuration=Release

命令行工具模式

CMake有一个-E的命令行工具模式，提供了一些常用的功能，比复制文件、创建目录、读写注册表、读写环境变量、计算md5值等等。脚本可以调用这些功能。
编写CMakeLists.txt
创建可以执行程序工程

首先从创建一个最简单的可执行程序开始，CMakeLists.txt内容如下：

cmake_minimum_required (VERSION 2.6)
project (LearnCMake)
message(${LearnCMake_SOURCE_DIR})
message(${LearnCMake_BINARY_DIR})
add_executable(FirstExecutable hello_world.cpp)

第1行是cmake需要的最低版本，目前这个是VERSION 2.6，一般不用修改。

第2~4行表示我们创建了一个名为LearnCMake工程，对应生成一个LearnCMake.sln。project函数表示创建一个工程。同时，也生成了4个变量：

    PROJECT_SOURCE_DIR, _SOURCE_DIR。工程的源代码目录。
    PROJECT_BINARY_DIR, _BINARY_DIR。工程的二进制文件目录。

第5行表示添加一个名为FirstExecutable的可执行程序项目，它的源代码为hello_world.cpp。下面是add_executable的完整用法：

add_executable(<name> [WIN32] [MACOSX_BUNDLE]
               [EXCLUDE_FROM_ALL]
               source1 [source2 ...])

默认的是创建控制台工程，加上WIN32表示创建的是win32工程，如下：

add_executable(FirstExecutable WIN32 hello_world.cpp)

后面是项目的代码，可以添加多个代码文件，用空格分开。
创建库工程

创建库工程跟创建可执行程序工程类似，创建库工程使用add_library函数，如下例子：

cmake_minimum_required (VERSION 3.0)
project (LearnCMake)
add_library(FirstLibrary first_library.cpp)
add_library(SecondLibrary second_library.cpp)
add_executable(FirstExecutable hello_world.cpp)
target_link_libraries(FirstExecutable FirstLibrary)

add_library的用法如下：

add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            source1 [source2 ...])

默认的是静态库，也可以显式的设置库是否为静态库、动态库或者是模块。另外BUILD_SHARED_LIBS也可控制编译成哪种库。

target_link_libraries用来链接库，用法如下：

target_link_libraries(<target> [item1 [item2 [...]]]
                      [[debug|optimized|general] <item>] ...)

set设置变量

add_library、add_executable都可以添加多个源文件，如下：

cmake_minimum_required (VERSION 3.0)
project (LearnCMake)
add_executable(FirstExecutable main.cpp app_util.h app_util.cpp)
add_library(FirstLibrary app_util.h app_util.cpp)

我们可以通过定义一个AppUtilSrc变量来代替app_util.h app_util.cpp，如下：

cmake_minimum_required (VERSION 3.0)
project (LearnCMake)
set(AppUtilSrcs app_util.h app_util.cpp)
add_executable(FirstExecutable main.cpp ${AppUtilSrcs})
add_library(FirstLibrary ${AppUtilSrcs})

效果是跟上面等价的。还可以累积值：

set(AppUtilSrcs app_util.h app_util.cpp)
set(AppUtilSrcs ${AppUtilSrcs} b.cpp)

这样AppUtilSrcs就代表着3个文件了。
设置编译模式

设置mt编译模式：

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

设置md编译模式：

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

默认是多字节模式，设置成unicode模式：

add_definitions(-D_UNICODE)

另外add_definitions还可以设置其他的选项。
添加其他CMakeLists.txt

一个CMakeLists.txt里面的target如果要链接其他CMakeLists.txt中的target，可以使用add_subdirectory，我们以使用googletest库为例：

add_subdirectory("../thirdparty/googletest/googletest/" gtest)
file(GLOB_RECURSE gtest_lib_head_files "../thirdparty/googletest/googletest/*.h")
source_group("gtest" FILES ${gtest_lib_head_files})
include_directories("../thirdparty/googletest/googletest/include")
aux_source_directory("./pbase_unittest/src" pbase_unittest_src_files)
file(GLOB_RECURSE pbase_unittest_include_files "./pbase_unittest/include/*.h")
add_executable(pbase_unittest ${pbase_unittest_src_files} ${pbase_unittest_include_files} ${gtest_lib_head_files})
target_link_libraries(pbase_unittest gtest)

代码控制

如果想把./pbase/src目录下的所有源文件加入到工程，可以用aux_source_directory把某个目录下的源文件加入到某个变量中，稍后就可以使用这个变量代表的代码了，如：

aux_source_directory("./pbase/src" pbase_lib_src_files)
add_library(pbase ${pbase_lib_src_files})

添加头文件包含目录是：

include_directories("../thirdparty/googletest/googletest/include")

但是include_directories中的文件不会体现先visual studio工程中，而aux_source_directory只会添加源文件，会忽略头文件，如果想生存的visual studio工程里面也包含头文件，可以这样：

# add head files
file(GLOB_RECURSE pbase_lib_head_files "./pbase/include/*.h")
add_library(pbase ${pbase_lib_head_files})

如果想生存visual studio中的filter，可以使用source_group：

file(GLOB_RECURSE gtest_lib_head_files "../thirdparty/googletest/googletest/*.h")
source_group("gtest" FILES ${gtest_lib_head_files})

最终添加头文件到工程里标准模板是：

file(GLOB_RECURSE gtest_lib_head_files "../thirdparty/googletest/googletest/*.h")
source_group("gtest" FILES ${gtest_lib_head_files})
include_directories("../thirdparty/googletest/googletest/include")
add_executable(pbase_unittest ${gtest_lib_head_files})


