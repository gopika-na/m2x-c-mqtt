/** @file chart.h
 *  @brief Wrapper for AT&T M2X <a href="https://m2x.att.com/developer/documentation/v2/charts">Charts API</a>
 */

#ifndef M2X_CHART_H
#define M2X_CHART_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/charts#List-Charts">List Charts</a> endpoint
 * @param ctx - m2x_context structure
 * @param query - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_chart_list(m2x_context *ctx, const char *query);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/charts#Create-Chart">Create Chart</a> endpoint
 * @param ctx - m2x_context structure
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_chart_create(m2x_context *ctx, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/charts#View-Chart-Details">View Chart Details</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the chart to get details
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_chart_view(m2x_context *ctx, const char *id);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/charts#Update-Chart">Update Chart</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the chart to update
 * @param data - Query parameters passed as keyword arguments. View M2X API Docs for listing of available parameters
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_chart_update(m2x_context *ctx, const char *id, const char *data);
/** @brief Method for <a href="https://m2x.att.com/developer/documentation/v2/charts#Delete-Chart">Delete Chart</a> endpoint
 * @param ctx - m2x_context structure
 * @param id (str) - ID of the chart to update
 * @return m2x_response - The API response, see M2X API docs for details
 */
m2x_response m2x_chart_delete(m2x_context *ctx, const char *id);


#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_CHART_H */
