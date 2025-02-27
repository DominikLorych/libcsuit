cmake_minimum_required(VERSION 3.13)
project(libcsuit
	DESCRIPTION "SUIT Manifest parser and encoder"
	LANGUAGES C
	VERSION 1.0.0)

set(CMAKE_C_FLAGS "-Wall -O3")
set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -g -DDEBUG") # use `-DCMAKE_BUILD_TYPE=Debug'

if(BUILD_LIBCSUIT_LIB)
	option(QCBOR_LIBRARY "QCBOR Library Identifier" "QCBOR::QCBOR")
	option(T_COSE_LIBRARY "t_cose Library Identifier" "t_cose")

	find_package(t_cose REQUIRED)
	find_package(MbedTLS REQUIRED)
	find_package(QCBOR REQUIRED)

	set(SOURCE_LIBRARY
			src/suit_common.c
			src/suit_digest.c
			src/suit_cose.c
			src/suit_manifest_print.c
			src/suit_manifest_encode.c
			src/suit_manifest_decode.c
			src/suit_manifest_process.c
			)

	add_library(libcsuit ${SOURCE_LIBRARY})
	target_include_directories(libcsuit PUBLIC inc)
	target_link_libraries(libcsuit ${CRYPTO_LIBRARIES} ${QCBOR_LIBRARY} ${T_COSE_LIBRARY})
	target_compile_definitions(libcsuit PUBLIC LIBCSUIT_PSA_CRYPTO_C)
endif()

if(BUILD_EXAMPLES)
	set(CRYPTO_PROVIDER "MbedTLS")
	set(MBEDTLS ON)
	set(CRYPTO_PROVIDER "MbedTLS")
	set(CRYPTO_LIBRARY "mbedtls")
	set(QCBOR_LIBRARY "qcbor")
	set(T_COSE_LIBRARY "t_cose")
	#add_subdirectory(../t_cose t_cose)



	if(MBEDTLS)

		set(CRYPTO_LIBRARIES "mbedtls") #  "mbedx509" "mbedcrypto"
		option(USE_PKCS11_HELPER_LIBRARY "Build mbed TLS with the pkcs11-helper library." OFF)
		option(ENABLE_ZLIB_SUPPORT "Build mbed TLS with zlib library." OFF)
		option(ENABLE_PROGRAMS "Build mbed TLS programs." OFF)
		option(ENABLE_TESTING "Build mbed TLS tests." OFF)

		set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/lib)

		# Use custom config file for Mbed TLS
		add_definitions(-DMBEDTLS_CONFIG_FILE="${CMAKE_CURRENT_LIST_DIR}/misc/config/mbedtls_config.h")

		option(USE_STATIC_MBEDTLS_LIBRARY "Build mbed TLS static library." off)
		option(USE_SHARED_MBEDTLS_LIBRARY "Build mbed TLS shared library." on)

		if (CMAKE_SYSTEM MATCHES "OpenBSD")
			option(LINK_WITH_PTHREAD "Explicitly link mbed TLS library to pthread." on)
		endif()

		#add_subdirectory(../mbedtls mbedtls)
		add_definitions(-DMBEDTLS=1)

	else()

		set(CRYPTO_LIBRARIES "crypto")

	endif()

	include(FetchContent)
	FetchContent_Declare(
			MbedTLS
			SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../mbedtls/"
			OVERRIDE_FIND_PACKAGE
	)
	FetchContent_Declare(
			qcbor
			SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../QCBOR/"
			OVERRIDE_FIND_PACKAGE
	)
	FetchContent_Declare(
			t_cose
			SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../t_cose/"
			OVERRIDE_FIND_PACKAGE
	)
	FetchContent_MakeAvailable(Mbedtls QCBOR t_cose)

	set(SOURCE_DECODE
			src/suit_common.c
			src/suit_digest.c
			src/suit_cose.c
			src/suit_manifest_print.c
			src/suit_manifest_encode.c
			src/suit_manifest_decode.c
			examples/suit_examples_common.c
			examples/suit_examples_cose.c
			examples/suit_manifest_parser_main.c
			)
	add_executable(suit_manifest_parser_main ${SOURCE_DECODE})
	target_include_directories(suit_manifest_parser_main PUBLIC inc examples/inc)
	target_link_libraries(suit_manifest_parser_main ${CRYPTO_LIBRARIES} ${QCBOR_LIBRARY} ${T_COSE_LIBRARY} -lm)
	#set_target_properties(suit_manifest_parser_main PROPERTIES INSTALL_RPATH ${CRYPTO_LIBRARY} t_cose qcbor)

	set(SOURCE_ENCODE
			src/suit_common.c
			src/suit_digest.c
			src/suit_cose.c
			src/suit_manifest_print.c
			src/suit_manifest_encode.c
			src/suit_manifest_decode.c
			examples/suit_examples_common.c
			examples/suit_examples_cose
			examples/suit_manifest_encode_main.c
			)
	add_executable(suit_manifest_encode_main ${SOURCE_ENCODE})
	target_include_directories(suit_manifest_encode_main PUBLIC inc examples/inc)
	target_link_libraries(suit_manifest_encode_main ${CRYPTO_LIBRARIES} ${QCBOR_LIBRARY} ${T_COSE_LIBRARY} -lm)
	#set_target_properties(suit_manifest_encode_main PROPERTIES INSTALL_RPATH ${CRYPTO_LIBRARY} t_cose qcbor)

	set(SOURCE_PROCESS
			src/suit_common.c
			src/suit_digest.c
			src/suit_cose.c
			src/suit_manifest_print.c
			src/suit_manifest_encode.c
			src/suit_manifest_decode.c
			src/suit_manifest_process.c
			examples/suit_examples_common.c
			examples/suit_examples_cose
			examples/suit_manifest_process_main.c
			)
	add_executable(suit_manifest_process_main ${SOURCE_PROCESS})
	target_include_directories(suit_manifest_process_main PUBLIC inc examples/inc)
	target_link_options(suit_manifest_process_main PRIVATE "LINKER:--wrap=suit_fetch_callback")
	target_link_options(suit_manifest_process_main PRIVATE "LINKER:--wrap=suit_store_callback")
	target_link_options(suit_manifest_process_main PRIVATE "LINKER:--wrap=suit_invoke_callback")
	target_link_options(suit_manifest_process_main PRIVATE "LINKER:--wrap=suit_condition_callback")
	target_link_libraries(suit_manifest_process_main ${CRYPTO_LIBRARIES} ${QCBOR_LIBRARY} ${T_COSE_LIBRARY} -lm)

	if(MBEDTLS)
		# Compile code for use with the PSA Crypto API
		# Use target_compile_definitions because for some reason add_compile_definitions and add_definitions do not work properly when libcsuit is a subdirectory
		target_compile_definitions(suit_manifest_parser_main PUBLIC LIBCSUIT_PSA_CRYPTO_C)
		target_compile_definitions(suit_manifest_encode_main PUBLIC LIBCSUIT_PSA_CRYPTO_C)
		target_compile_definitions(suit_manifest_process_main PUBLIC LIBCSUIT_PSA_CRYPTO_C)
	endif()
endif()
