# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


set(CPACK_BINARY_7Z "OFF")
set(CPACK_BINARY_IFW "OFF")
set(CPACK_BINARY_INNOSETUP "OFF")
set(CPACK_BINARY_NSIS "ON")
set(CPACK_BINARY_NUGET "OFF")
set(CPACK_BINARY_WIX "OFF")
set(CPACK_BINARY_ZIP "OFF")
set(CPACK_BUILD_SOURCE_DIRS "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/src/11.0.2-c30c0a133f.clean;C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/x64-windows-rel")
set(CPACK_CMAKE_GENERATOR "Ninja")
set(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
set(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_FILE "C:/Program Files/CMake/share/cmake-3.31/Templates/CPack.GenericDescription.txt")
set(CPACK_DEFAULT_PACKAGE_DESCRIPTION_SUMMARY "FMT built using CMake")
set(CPACK_GENERATOR "NSIS")
set(CPACK_INNOSETUP_ARCHITECTURE "x64")
set(CPACK_INSTALL_CMAKE_PROJECTS "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/x64-windows-rel;FMT;ALL;/")
set(CPACK_INSTALL_PREFIX "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/pkgs/fmt_x64-windows")
set(CPACK_MODULE_PATH "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/src/11.0.2-c30c0a133f.clean/support/cmake")
set(CPACK_NSIS_DISPLAY_NAME "fmt 11.0.2")
set(CPACK_NSIS_INSTALLER_ICON_CODE "")
set(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_PACKAGE_NAME "fmt 11.0.2")
set(CPACK_NSIS_UNINSTALL_NAME "Uninstall")
set(CPACK_OUTPUT_CONFIG_FILE "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/x64-windows-rel/CPackConfig.cmake")
set(CPACK_PACKAGE_DEFAULT_LOCATION "/")
set(CPACK_PACKAGE_DESCRIPTION_FILE "C:/Program Files/CMake/share/cmake-3.31/Templates/CPack.GenericDescription.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "FMT built using CMake")
set(CPACK_PACKAGE_FILE_NAME "fmt-11.0.2-win64")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "fmt 11.0.2")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "fmt 11.0.2")
set(CPACK_PACKAGE_NAME "fmt")
set(CPACK_PACKAGE_RELOCATABLE "true")
set(CPACK_PACKAGE_VENDOR "Humanity")
set(CPACK_PACKAGE_VERSION "11.0.2")
set(CPACK_PACKAGE_VERSION_MAJOR "11")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_RESOURCE_FILE_LICENSE "C:/Program Files/CMake/share/cmake-3.31/Templates/CPack.GenericLicense.txt")
set(CPACK_RESOURCE_FILE_README "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/src/11.0.2-c30c0a133f.clean/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "C:/Program Files/CMake/share/cmake-3.31/Templates/CPack.GenericWelcome.txt")
set(CPACK_SET_DESTDIR "OFF")
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES ".*[.]a$;.*[.]a/;.*[.]so.*$;.*[.]so.*/;.*[.]xcodeproj$;.*[.]xcodeproj/;.*~$;.*~/;[.]vscode/$;[.]vscode//;/CMakeScripts$;/CMakeScripts/;/Testing$;/Testing/;/_CPack_Packages$;/_CPack_Packages/;/install_manifest[.]txt$;/install_manifest[.]txt/;CMakeCache[.]txt$;CMakeCache[.]txt/;CMakeFiles$;CMakeFiles/;CPack.*[.]cmake$;CPack.*[.]cmake/;CTestTestfile[.]cmake$;CTestTestfile[.]cmake/;FMT[.]build$;FMT[.]build/;Makefile$;Makefile/;bin/$;bin//;build/$;build//;cmake_install[.]cmake$;cmake_install[.]cmake/;fmt-.*[.]cmake$;fmt-.*[.]cmake/;fmt[.]pc$;fmt[.]pc/;/.git;/build/doxyxml;.vagrant")
set(CPACK_SOURCE_OUTPUT_CONFIG_FILE "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/x64-windows-rel/CPackSourceConfig.cmake")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "fmt-11.0.2")
set(CPACK_SYSTEM_NAME "win64")
set(CPACK_THREADS "1")
set(CPACK_TOPLEVEL_TAG "win64")
set(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "C:/Users/Noah/Desktop/Files/Programmioong/C++/FirstGui/vcpkg_installed/vcpkg/blds/fmt/x64-windows-rel/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
