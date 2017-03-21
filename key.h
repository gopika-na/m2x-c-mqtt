/** @file key.h
 *  @brief Wrapper for AT&T M2X <a href="https://m2x.att.com/developer/documentation/v2/keys">Keys API</a>
 */
#ifndef M2X_KEY_H
#define M2X_KEY_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#List-Keys">List Keys</a> endpoint
 * @param ctx - m2x_context structure
 * @param query - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_list(m2x_context *ctx, const char *query);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#Create-Key">Create Key</a> endpoint
 * @param ctx - m2x_context structure
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_create(m2x_context *ctx, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#View-Key-Details">View Key Details</a> endpoint
 * @param ctx - m2x_context structure
 * @param key (str) - ID of the key to view details
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_view(m2x_context *ctx, const char *key);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#Update-Key">Update Key</a> endpoint
 * @param ctx - m2x_context structure
 * @param key (str) - ID of the key to update
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_update(m2x_context *ctx, const char *key, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#Regenerate-Key">Regenerate Key</a> endpoint
 * @param ctx - m2x_context structure
 * @param key (str) - ID of the key to regenerate
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_regenerate(m2x_context *ctx, const char *key);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/keys#Delete-Key">Delete Key</a> endpoint
 * @param ctx - m2x_context structure
 * @param key (str) - ID of the key to delete
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_key_delete(m2x_context *ctx, const char *key);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_KEY_H */
