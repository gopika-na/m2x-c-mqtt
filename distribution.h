/** @file distribution.h
 *  @brief Wrapper for AT&T M2X <a href="https://m2x.att.com/developer/documentation/v2/distribution">Distribution API</a>
 */

#ifndef M2X_DISTRIBUTION_H
#define M2X_DISTRIBUTION_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#List-Distributions">List Distributions</a> endpoint
 * @param ctx - m2x_context structure
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_list(m2x_context *ctx);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Create-Distribution">Create Distribution</a> endpoint
 * @param ctx - m2x_context structure
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_create(m2x_context *ctx, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#View-Distribution-Details">View Distribution Details</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to view details
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_view(m2x_context *ctx, const char *id);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Update-Distribution-Details">Update Distribution Details</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to view details
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_update(m2x_context *ctx, const char *id, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Delete-Distribution">Delete Distribution</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to delete device
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_delete(m2x_context *ctx, const char *id);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#List-Devices-from-an-existing-Distribution">List Devices from an existing Distribution</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to retrieve a list of devices
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_devices(m2x_context *ctx, const char *id);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Add-Device-to-an-existing-Distribution">Add Device to an existing Distribution</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to add device
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_add_device(m2x_context *ctx, const char *id, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#List-Data-Streams">List Data Streams</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to list data streams
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_streams(m2x_context *ctx, const char *id);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Create-Update-Data-Stream">Create/Update Data Stream</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to create/update data streams
 * @param name (str) - Stream name to create/update
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_update_stream(m2x_context *ctx, const char *id, const char *name, const char *data);
#define m2x_distribution_create_stream m2x_distribution_update_stream
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#View-Data-Stream">View Data Stream</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to get details of specific data stream
 * @param name (str) - Stream name to get details
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_view_stream(m2x_context *ctx, const char *id, const char *name);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/distribution#Delete-Data-Stream">Delete Data Stream</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the distribution to delete existing data stream
 * @param name (str) - Stream name to delete
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_distribution_delete_stream(m2x_context *ctx, const char *id, const char *name);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_DISTRIBUTION_H */

