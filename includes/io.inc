<?php
/**
 * @file
 */


/**
 * Performs a remote call to the d2d_client using D2D in order
 * to retrieve the users number.
 */
function _d2d_server_fetch_users_number($d2did)  {

  global $user;
  
  // Get the current server
  //$url = variable_get('patterns_client_server', PATTERNS_CLIENT_SERVER);
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  // Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_client_get_users_number', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please ensure your Peer instance has provided you permisssions to perform this operation.'), 'error');
    return -1;
  }else if (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }else{
    d2d_server_set_users_number_by_d2did($d2did, $result['return']);
    return 0;
  }

}

/**
 * Performs a remote call to the d2d_client using D2D in order
 * to retrieve the site name and site slogan.
 */
function _d2d_server_fetch_site_information($d2did)  {

  global $user;
  
  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  // Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_client_get_site_information', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please ensure your Peer instance has provided you permisssions to perform this operation.'), 'error');
  }else if (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }else{
    d2d_server_set_site_info_by_d2did($d2did, d2d_explode($result['return']));
  }
}

/**
 * Performs a remote call to the d2d_client using D2D in order
 * to retrieve nodes number.
 */
function _d2d_server_fetch_nodes_number($d2did)  {

  global $user;
  
  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  // Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_client_get_nodes_number', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please ensure your Peer instance has provided you permisssions to perform this operation.'), 'error');
  }else if (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }else{
    d2d_server_set_nodes_number_by_d2did($d2did, $result['return']);
  }
}

/**
 * Performs a remote call to the d2d_client using D2D in order
 * to retrieve tags set by d2d client.
 */
function _d2d_server_fetch_tags($d2did)  {

  global $user;
  
  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  // Prepare arguments for the call. In this case just the maximum number of patterns expected
  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_client_get_tags', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message(check_plain($url) . ' - ' . t('Unknown error. Please ensure your Peer instance has provided you permisssions to perform this operation.'), 'error');
  }else if (!is_null($result['error'])) {
    drupal_set_message(t('The Patterns server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }else{
    d2d_server_set_tags_by_d2did($d2did, $result['return']);
  }
}
