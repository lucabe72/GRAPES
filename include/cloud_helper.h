#ifndef CLOUD_HELPER_H
#define CLOUD_HELPER_H

#define CLOUD_HELPER_MAX_INSTANCES 10

#include <stdint.h>
#include <sys/time.h>

#include "net_helper.h"

/**
 * @file cloud_helper.h
 *
 * @brief Cloud communication facility interface.
 *
 * A clean interface throught which all the cloud communication procedure needed by SOM funcions
 * are handled.
 */

/**
 * Implementation dependant internal representation of the cloud clontext
 */
struct cloud_helper_contex;

/**
 * @brief Initialize all needed internal parameters.
 * Initialize the parameters for the cloud facilities and create a context representing the cloud.
 * Only one instance of net_helper is allowed for a specific nodeID.
 * @param[in] local NodeID associated with this instance of cloud_helper.
 * @param[in] config Cloud specific configuration options.
 */
struct cloud_helper_context* cloud_helper_init(struct nodeID *local, const char *config);

/**
 * @brief Identifies the cloud_helper instance associated to the specified nodeID
 * Returns the instance of cloud_helper that was initialized with the specified nodeID.
 */
struct cloud_helper_context* get_cloud_helper_for(struct nodeID *local);

/**
 * @brief Get the value for the specified key from the cloud.
 * This function send a request to the cloud for the value associated to the specified key.
 * Use the wait4cloud to listen for the answer and revc_from_cloud to read the response.
 * @param[in] context The contex representing the desired cloud_helper instance.
 * @param[in] key Key to retrieve.
 * @param[in] header_ptr A pointer to the header which will be added to the retrieved data. May be NULL
 * @param[in] header_size The length of the header.
 * @return 0 if the request was successfully sent, 1 0therwise
 */
int get_from_cloud(struct cloud_helper_context *context, char *key, uint8_t *header_ptr, int header_size);

/**
 * @brief Put on the cloud the value for a specified key.
 * This function transparently handles the sending routines.
 * @param[in] context The contex representing the desired cloud_helper instance.
 * @param[in] key Key to retrieve.
 * @param[in] buffer_ptr A pointer to the buffer in which to store the retrieved data.
 * @param[in] buffer_size The size of the data buffer 
 * @return 0 on success, 1 on failure
 */
int put_on_cloud(struct cloud_helper_context *context, char *key, uint8_t *buffer_ptr, int buffer_size);

/**
 * @brief Returns the nodeID identifing the cloud
 * This function transparently handles the identification of the cloud node.
 * @param[in] context The contex representing the desired cloud_helper instance.
 * @return nodeID identifying the cloud.
 */
struct nodeID* get_cloud_node(struct cloud_helper_context *context);

/**
 * @brief Check for cloud responses.
 * Check if the some cloud GET operation has concluded. It sets a timeout to return at most after a given time.
 * @param[in] context The contex representing the desired cloud_helper instance.
 * @param[in] tout A pointer to a timer to be used to set the waiting timeout.
 * @return 1 if the GET operation was succesful, -1 if the GET operation failed (unkwnown key), 0 otherwise.
 */
int wait4cloud(struct cloud_helper_context *context, struct timeval *tout);


/**
 * @brief Receive data from the cloud.
 * This function transparently handles the receving routines.
 * @param[out] buffer_ptr A pointer to the buffer in which to store the retrieved data.
 * @param[out] buffer_size The size of the data buffer
 * @return The number of received bytes or -1 if some error occurred.
 */
int recv_from_cloud(struct cloud_helper_context *context, uint8_t *buffer_ptr, int buffer_size);

#endif
