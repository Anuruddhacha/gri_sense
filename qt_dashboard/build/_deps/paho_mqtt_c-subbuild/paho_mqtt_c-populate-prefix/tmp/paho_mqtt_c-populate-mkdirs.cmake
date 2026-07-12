# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-src")
  file(MAKE_DIRECTORY "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-build"
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix"
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/tmp"
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/src/paho_mqtt_c-populate-stamp"
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/src"
  "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/src/paho_mqtt_c-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/src/paho_mqtt_c-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/MSI/Desktop/ESP_32/agri_sense/mqtt_sub/build/_deps/paho_mqtt_c-subbuild/paho_mqtt_c-populate-prefix/src/paho_mqtt_c-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
