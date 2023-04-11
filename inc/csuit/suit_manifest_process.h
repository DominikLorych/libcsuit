/*
 * Copyright (c) 2020-2023 SECOM CO., LTD. All Rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

/*!
    \file   suit_manifest_process.h

    \brief  Declarations of structures and functions
 */

#ifndef SUIT_MANIFEST_PROCESS_H
#define SUIT_MANIFEST_PROCESS_H

#include "t_cose/t_cose_common.h"
#include "suit_manifest_data.h"

#define BIT(nr) (1UL << (nr))
/* draft-suit-manifest */
#define SUIT_PARAMETER_CONTAINS_VENDOR_IDENTIFIER BIT(SUIT_PARAMETER_VENDOR_IDENTIFIER)
#define SUIT_PARAMETER_CONTAINS_CLASS_IDENTIFIER BIT(SUIT_PARAMETER_CLASS_IDENTIFIER)
#define SUIT_PARAMETER_CONTAINS_IMAGE_DIGEST BIT(SUIT_PARAMETER_IMAGE_DIGEST)
#define SUIT_PARAMETER_CONTAINS_COMPONENT_SLOT BIT(SUIT_PARAMETER_COMPONENT_SLOT)
#define SUIT_PARAMETER_CONTAINS_STRICT_ORDER BIT(SUIT_PARAMETER_STRICT_ORDER)
#define SUIT_PARAMETER_CONTAINS_SOFT_FAILURE BIT(SUIT_PARAMETER_SOFT_FAILURE)
#define SUIT_PARAMETER_CONTAINS_IMAGE_SIZE BIT(SUIT_PARAMETER_IMAGE_SIZE)
#define SUIT_PARAMETER_CONTAINS_CONTENT BIT(SUIT_PARAMETER_CONTENT)
#define SUIT_PARAMETER_CONTAINS_URI BIT(SUIT_PARAMETER_URI)
#define SUIT_PARAMETER_CONTAINS_SOURCE_COMPONENT BIT(SUIT_PARAMETER_SOURCE_COMPONENT)
#define SUIT_PARAMETER_CONTAINS_INVOKE_ARGS BIT(SUIT_PARAMETER_INVOKE_ARGS)
#define SUIT_PARAMETER_CONTAINS_DEVICE_IDENTIFIER BIT(SUIT_PARAMETER_DEVICE_IDENTIFIER)

/* draft-suit-update-management */
#define SUIT_PARAMETER_CONTAINS_USE_BEFORE BIT(SUIT_PARAMETER_USE_BEFORE)
#define SUIT_PARAMETER_CONTAINS_MINIMUM_BATTERY BIT(SUIT_PARAMETER_MINIMUM_BATTERY)
#define SUIT_PARAMETER_CONTAINS_UPDATE_PRIORITY BIT(SUIT_PARAMETER_UPDATE_PRIORITY)
#define SUIT_PARAMETER_CONTAINS_VERSION BIT(SUIT_PARAMETER_VERSION)
#define SUIT_PARAMETER_CONTAINS_WAIT_INFO BIT(SUIT_PARAMETER_WAIT_INFO)

/* draft-suit-trust-domains */
#define SUIT_PARAMETER_CONTAINS_ENCRYPTION_INFO BIT(SUIT_PARAMETER_ENCRYPTION_INFO)

typedef union suit_rep_policy {
    uint64_t val;
    struct {
        uint64_t record_on_success: 1;
        uint64_t record_on_failure: 1;
        uint64_t sysinfo_success: 1;
        uint64_t sysinfo_failure: 1;
        uint64_t padding: 60;
    };
} suit_rep_policy_t;

typedef struct suit_reference {
    UsefulBufC      manifest_uri;
    suit_digest_t   manifest_digest;
} suit_reference_t;

typedef struct suit_record {
    suit_component_identifier_t     manifest_id;
    int64_t                         manifest_section;
    uint64_t                        section_offset;
    uint64_t                        component_index;
    suit_parameters_list_t          parameters;
} suit_record_t;

typedef struct suit_report_recoreds {
    size_t          len;
    suit_record_t   suit_records[SUIT_MAX_ARRAY_LENGTH];
} suit_report_records_t;

typedef struct suit_report_result {
    int64_t         result_code;
    suit_record_t   result_record;
} suit_report_result_t;

/*!
 * This passes enough data to construct SUIT_Report.
 */
typedef struct suit_report_args {
    /* SUIT_Report */
    suit_reference_t suit_reference;
    UsefulBufC suit_nonce;
    suit_report_records_t suit_report_records;
    bool success;
    suit_report_result_t suit_report_result;

    suit_envelope_key_t level0;
    union {
        suit_manifest_key_t manifest_key;
    } level1;
    union {
        suit_con_dir_key_t condition_directive;
        suit_common_key_t common_key;
        suit_text_key_t text_key;
    } level2;
    union {
        suit_con_dir_key_t condition_directive;
        suit_parameter_key_t parameter;
        suit_text_component_key_t text_component_key;
    } level3;
    union {
        suit_parameter_key_t parameter;
    } level4;

    QCBORError qcbor_error;
    suit_err_t suit_error;

    suit_rep_policy_t report;
} suit_report_args_t;

/*!
 * This passes enough data to invoke a component.
 */
typedef struct suit_invoke_args {
    suit_component_identifier_t component_identifier;
    /* basically byte-string value, so may not '\0' terminated */
    uint8_t args[SUIT_MAX_ARGS_LENGTH];
    size_t args_len;

    suit_rep_policy_t report;
} suit_invoke_args_t;

typedef enum suit_store_key {
    SUIT_STORE  = 0,
    SUIT_COPY   = 1,
    SUIT_SWAP   = 2,
    SUIT_UNLINK = 3,
} suit_store_key_t;

/*!
 * Request to store data as component identifier.
 * This feature is used on fetching integrated-payload or integrated-dependency.
 * The memory object is integrated into the manifest, so there is no need to fetch actually.
 */
typedef struct suit_store_args {
    suit_rep_policy_t report;

    suit_component_identifier_t dst;
    suit_component_identifier_t src; // used by SUIT_COPY and SUIT_SWAP
    UsefulBufC src_buf;

    UsefulBufC encryption_info;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];

    /* store, copy, swap, unlink */
    suit_store_key_t operation;
} suit_store_args_t;

/*!
 * Request to fetch and store data as component identifier.
 */
typedef struct suit_fetch_args {
    suit_component_identifier_t dst;
    size_t uri_len;
    char uri[SUIT_MAX_URI_LENGTH];

    /**
     *  Pointer to allocated memory in the caller.
     *  This could be NULL if the caller wants callee
     *  to allocate space corresponding to the component identifier.
     */
    void *ptr;
    /**
     *  The length of the allocated buffer.
     *  Should be overwritten as the actual length of the fetched object.
     */
    size_t buf_len;

    suit_rep_policy_t report;

    UsefulBufC args;
    /* in draft-ietf-suit-firmware-encryption */
    UsefulBufC                  encryption_info;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];
} suit_fetch_args_t;

typedef struct suit_fetch_ret {
    /**
     *  Pointer to fetched payload.
     */
    void *ptr;

    /**
     *  The length of the fetched payload.
     */
    size_t buf_len;
} suit_fetch_ret_t;

typedef struct suit_condition_args {
    suit_rep_policy_t report;

    suit_component_identifier_t dst;
    suit_con_dir_key_t condition;

    union {
        uint64_t        u64;
        UsefulBufC      str;
        struct {
            uint64_t        image_size;
            suit_digest_t   image_digest;
        };
    } expected;
} suit_condition_args_t;

typedef struct suit_parameter_args {
    uint64_t                    exists;

    UsefulBufC                  vendor_id;
    UsefulBufC                  class_id;
    UsefulBufC                  device_id;

    suit_digest_t               image_digest;
    uint64_t                    use_before;
    uint64_t                    component_slot;

    /* default True */
    suit_parameter_bool_t       strict_order;

    /* default True if suit-directive-try-each is invoked,
       default False if suit-directive-run-sequence is invoked */
    suit_parameter_bool_t       soft_failure;

    uint64_t                    image_size;

    UsefulBufC                  content;
    UsefulBufC                  uri;

    uint64_t                    source_component;

    /* used in suit-directive-fetch */
    UsefulBufC                  fetch_arguments;

    /* used in suit-directive-invoke */
    UsefulBufC                  invoke_args;


    /* in draft-ietf-suit-update-management */
    /* positive minimum battery level in mWh */
    uint64_t                     minimum_battery;

    /* the value is not defined, though 0 means "NOT GIVEN" here */
    uint64_t                     update_priority;

    /* processed if suit-condition-version is specified */
    UsefulBufC                  version;

    //??                        wait_info;

    /* in draft-ietf-suit-trust-domains */


    /* in draft-ietf-suit-firmware-encryption */
    UsefulBufC                  encryption_info;
} suit_parameter_args_t;

typedef union {
    uint16_t all;
    struct {
        /* NOTE: must be false inside process-dependency
         * see https://datatracker.ietf.org/doc/html/draft-ietf-suit-trust-domains-02#name-suit-directive-process-depe
         */
        uint16_t allow_missing          : 1;

        uint16_t manifest_component_id  : 1;
        uint16_t dependency_resolution  : 1;
        uint16_t payload_fetch          : 1;
        uint16_t install                : 1;
        uint16_t uninstall              : 1;

        uint16_t validate               : 1;
        uint16_t load                   : 1;
        uint16_t invoke                 : 1;

        uint16_t text                   : 1;
        uint16_t coswid                 : 1;
    };
} suit_process_flag_t;

typedef struct suit_inputs {
    UsefulBufC manifest;
    size_t left_len;
    size_t key_len;
    suit_mechanism_t mechanisms[SUIT_MAX_KEY_NUM];
    suit_process_flag_t process_flags;
    UsefulBufC suit_nonce;
    uint8_t dependency_depth;

    uint8_t *ptr;
    uint8_t buf[];
} suit_inputs_t;

typedef struct suit_extracted {
    suit_dependencies_t dependencies;
    suit_component_identifier_t manifest_component_id;

    uint8_t components_len;
    suit_component_with_index_t components[SUIT_MAX_INDEX_NUM];
    suit_payloads_t payloads;

    UsefulBufC manifest;
    suit_digest_t manifest_digest;
    UsefulBufC shared_sequence;
    UsefulBufC reference_uri;
    UsefulBufC dependency_resolution;
    suit_digest_t dependency_resolution_digest;
    UsefulBufC payload_fetch;
    suit_digest_t payload_fetch_digest;
    UsefulBufC install;
    suit_digest_t install_digest;
    UsefulBufC uninstall;
    suit_digest_t uninstall_digest;
    UsefulBufC validate;
    UsefulBufC load;
    UsefulBufC invoke;
    UsefulBufC text;
    suit_digest_t text_digest;
    UsefulBufC coswid;
    suit_digest_t coswid_digest;
} suit_extracted_t;

void suit_process_digest(QCBORDecodeContext *context, suit_digest_t *digest);

/*!
    \brief  Decode & Process SUIT binary

    \param[in]      suit_inputs     To be procceed manifests and its public keys to verify.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    Process one or more SUIT_Envelope(s) like below.
    Libcsuit parse suit-install, suit-invoke, ... and call some callback functions respectively.
    If any error occurred, report callback function will be called.
    By default, libcsuit just calls suit_print_store, suit_print_fetch, etc.
    You can overwrite it with linker options (see Makefile.process for example).

    The figure below describes the flow in the process in pseudocode.
    Note that the arguments and function names are not the same as the actual.

    \code{.unparsed}
    +-App-------------------------------+    +-libcsuit--------------------------------------+
    | main() {                          |    |                                               |
    |   keys = init_keys();             |    |                                               |
    |   m = get_manifest();             |    |                                               |
    |   suit_process_envelope(m, keys); |===>| suit_process_envelope() {                     |
    |   }                               |    |   check_digests(keys, m);                     |
    | }                                 |    |   install(m) {                                |
    |                                   |    |     p = extract_params(m.shared, m.install);  |
    | suit_fetch_callback() {           |<===|     err = suit_fetch_callback(p.dst, p.uri);  |
    |   http_get(uri, ptr);             |    |                                               |
    |   return SUIT_SUCCESS;            |===>|                                               |
    | }                                 |    |     if (report_condition(p.report, err)) {    |
    | suit_report_callback() {          |<===|       suit_report_callback(err, at);          |
    |   report = create_suit_report();  |    |     }                                         |
    |   http_post(report, uri);         |    |                                               |
    |   return SUIT_SUCCESS;            |===>|     if (err) {                                |
    | }                                 |    |       return SUIT_ERR_ABORT;                  |
    |                                   |    |     }                                         |
    | suit_condition_callback() {       |<===|     err = suit_condition_callback(p.class_id) |
    |   class_id = get_class_id();      |    |                                               |
    |   if (class_id != p.class_id) {   |    |                                               |
    |     suit_report_callback();       |    |                                               |
    |     return SUIT_ERR_CONDITION;    |    |                                               |
    |   }                               |    |                                               |
    |   return SUIT_SUCCESS;            |===>|  }                                            |
    | }                                 |    |  invoke(m) {                                  |
    |                                   |    |    p = extract_params(m.shared, m.invoke);    |
    | suit_invoke_callback() {          |<===|    err = suit_invoke_callback(p.dst, p.args); |
    |   return system(args);            |===>|    if (report_condition(p.report, err)) {     |
    | }                                 |    |      suit_report_callback(err, at);           |
    |                                   |    |    }                                          |
    |                                   |    |  }                                            |
    +-----------------------------------+    +-----------------------------------------------+
    \endcode
 */
suit_err_t suit_process_envelope(suit_inputs_t *suit_inputs);

/*!
    \brief  SUIT fetch callback triggerd in libcsuit

    \param[in]      fetch_args      Fetch and suit-report arguments. See \ref suit_fetch_args_t.
    \param[out]     fetch_ret       Fetch result. See \ref suit_fetch_ret_t.
    Triggered on \ref SUIT_DIRECTIVE_FETCH.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_fetch_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-fetch
    requesting to fetch an image and dependent SUIT Manifest into component.
*/
suit_err_t suit_fetch_callback(suit_fetch_args_t fetch_args, suit_fetch_ret_t *fetch_ret);

/*!
    \brief  SUIT store callback triggerd in libcsuit
    \param[in]      store_args      Store and suit-report arguments. See \ref suit_store_args_t.
    Triggered on \ref SUIT_DIRECTIVE_FETCH of integrated-payload or integrated-dependency.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_store_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-write, suit-directive-copy,
    suit-directive-swap and suit-directive-unlink against component(s),
    and at suit-directive-fetch against integrated payload,
    requesting to store an image and dependent SUIT Manifest into component or
    to unlink a component.
*/
suit_err_t suit_store_callback(suit_store_args_t store_args);

/*!
    \brief  SUIT invoke callback triggerd in libcsuit
    \param[in]      invoke_args        Invoke and suit-report arguments. See \ref suit_invoke_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_invoke_callback. See Makefile.process as an example.
    The libcsuit triggers this function at suit-directive-invoke
    requesting to invoke a component.
*/
suit_err_t suit_invoke_callback(suit_invoke_args_t invoke_args);

/*!
    \brief  SUIT report callback triggerd in libcsuit
    \param[in]      condition_args     Condition and suit-report arguments. See \ref suit_condition_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_condition_callback. See Makefile.process as an example.
    The libcsuit may trigger this function at suit-condition-*
    requesting to check the condition.
*/
suit_err_t suit_condition_callback(suit_condition_args_t condition_args);

/*!
    \brief  SUIT report callback trigged in libcsuit
    \param[in]      report_args     Suit-report arguments and errors. See \ref suit_report_args_t.
    \return         This returns one of the error codes defined by \ref suit_err_t.

    This function is expected to be replaced by user defined function
    using -Xlinker --wrap suit_report_callback.
    The libcsuit may trigger this function based on SUIT_Rep_Policy.
    requesting to check the condition.
*/
suit_err_t suit_report_callback(suit_report_args_t report_args);

#endif /* SUIT_MANIFEST_PROCESS_H */


