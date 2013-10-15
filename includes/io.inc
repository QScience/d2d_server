<?php
/**
 * @file
 */


/**
 * Implements hook_d2d_secure_rpc().
 */
function d2d_server_d2d_secure_rpc() {
  $methods = array();
  $methods['d2d_server_save_client_actions'] = array(
    'arguments' => array('info' => 'is_string'),
    'callback' => '_d2d_server_save_client_actions',
    'description' => 'Save the informations about client actions.',
    );
  return $methods;
}

/**
 * Performs a remote call to the d2d_passport using D2D in order
 * to retrieve all kinds of informations.
 */
function _d2d_server_fetch_infotypes($d2did, $infotypes = FALSE) {
  if ($infotypes === FALSE) {
    $infotypes = array();
    $infotypes['users_number'] = TRUE;
    $infotypes['nodes_number'] = TRUE;
    $infotypes['site_info'] = TRUE;
    $infotypes['tags'] = TRUE;
    $infotypes['friendship'] = TRUE;
  }

  global $user;

  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  $post = array('infotypes' => d2d_implode($infotypes));

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_passport_get_informations', $post, $error_string);
  $result = d2d_explode($result);


  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message($d2did . ' - ' . t('Fetch Informations Error. Please make sure that the instance has granted you the permissions to perform the operation.'), 'error');
    return -1;
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The d2d server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    $return = d2d_explode($result['return']);
    foreach ($return as $key => $value) {
      $function = "d2d_server_set_" . $key . "_by_d2did"; 
      $function($d2did, $value);
    }
    //d2d_server_set_users_number_by_d2did($d2did, $result['return']);
    return 0;
  }

}

/**
 * Performs a remote call to the d2d_passport using D2D in order
 * to retrieve the users number.
 */
function _d2d_server_fetch_users_number($d2did)  {

  global $user;

  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_passport_get_users_number', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message($d2did . ' - ' . t('Fetch Users Number Error. Please make sure that the instance has granted you the permissions to perform the operation.'), 'error');
    return -1;
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The d2d server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    d2d_server_set_users_number_by_d2did($d2did, $result['return']);
    return 0;
  }

}

/**
 * Performs a remote call to the d2d_passport using D2D in order
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

  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_passport_get_site_information', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message($d2did . ' - ' . t('Fetch Site Information Error. Please make sure that the instance has granted you the permissions to perform the operation.'), 'error');
    return -1;
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The d2d server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    d2d_server_set_site_info_by_d2did($d2did, d2d_explode($result['return']));
    return 0;
  }
}

/**
 * Performs a remote call to the d2d_passport using D2D in order
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

  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_passport_get_nodes_number', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message($d2did . ' - ' . t('Fetch Nodes Number Error. Please make sure that the instance has granted you the permissions to perform the operation.'), 'error');
    return -1;
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The d2d server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    d2d_server_set_nodes_number_by_d2did($d2did, $result['return']);
    return 0;
  }
}

/**
 * Performs a remote call to the d2d_passport using D2D in order
 * to retrieve tags set by d2d passport.
 */
function _d2d_server_fetch_tags($d2did)  {

  global $user;

  // Get the current server
  $url = d2d_server_get_url_by_d2did($d2did);
  if (!$url) {
    return -1;
  }
  $friend = d2d_api_friend_get_by_url($url->url);

  $post = array();

  // Send the request. We explode the result in two different steps
  $error_string = '';
  $result = d2d_call_secure_rpc($friend, 'd2d_passport_get_tags', $post, $error_string);
  $result = d2d_explode($result);

  //dsm($result);

  // Display communication errors if any
  if (!$result) {
    // Unknown error
    drupal_set_message($d2did . ' - ' . t('Fetch Tags Error. Please make sure that the instance has granted you the permissions to perform the operation.'), 'error');
    return -1;
  }
  elseif (!is_null($result['error'])) {
    drupal_set_message(t('The d2d server returned this response: @response', array('@response' => $result['error'])), 'error');
    return $result['error'];
  }
  else{
    d2d_server_set_tags_by_d2did($d2did, $result['return']);
    return 0;
  }
}

/**
 */
function _d2d_server_save_client_actions($arguments, $rpc_info) {
  $return = FALSE;
  $error;
  if (empty($arguments) || !isset($arguments['info'])) {
    $error = 'Error. Empty request received.';
  }
  else {
    $info = d2d_explode($arguments['info']);
    if (!isset($info['from']) || !isset($info['to']) ||
      !isset($info['type']) || !isset($info['len']) ||
      !isset($info['from_url']) || !isset($info['to_url']) ||
      !isset($info['from_name']) || !isset($info['to_name'])) {
      $error = 'Error. Information about Action is not complete.';
  }
  else {
    $q = d2d_server_get_d2dentity_by_d2did($info['from']);
    if ($q === FALSE) {
      $error = 'Error. The instance carrying out the action is not imported to d2d server yet.';
    }
    else {
      if (function_exists('nodejs_enqueue_message')) {
        $message = (object) array(
          'data' => (object) $info,
            'channel' => 'nodejs_user_' . 1, // to root
            'callback' => 'displayRequest',
            );
        nodejs_enqueue_message($message);
      }
      d2d_server_save_actions_form_clients($info);
      $return = TRUE;
    }
  }
}
return d2d_implode(array('return' => $return, 'error' => $error));
}



/**
 * Menu callback; outputs a JSON object with the actions from a certain date.
 */
function _d2d_server_get_actions_service() {

  $from = FALSE;
  if (isset($_POST['from'])) {
    $from = check_plain($_POST['from']);
    $actions = _d2d_server_get_actions_helper($from);
  }

  if ($from === FALSE) {
    drupal_json_output(array(
      'success' => FALSE,
      'message' => 'Invalid or missing FROM',
      ));
  }

  drupal_json_output(array(
    'success' => TRUE,
    'actions' => $actions,
    ));
}


/**
 * Returns all the actions more recent than a certain date.
 *
 * @param int $from
 *   The reference timestamp.
 *
 * @return array
 *   The array of actions more recent than $from
 */
function _d2d_server_get_actions_helper($from) {

  $query = db_select('d2d_server_actions_from_clients', 'a')
  ->fields('a')
  ->condition('a.created', $from, '>')
  ->orderBy('a.created', 'ASC');

  return $query->execute()->FetchAll();
}
