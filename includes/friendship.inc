<?php

/**
 * @file
 *   Functions related to autoaccept friendship requests.
 */

/**
 * Implements hook_d2d_friendship_update(); insert a new d2d entity in db.
 *
 * @see d2d_api_d2d_friendship_update()
 */
function d2d_server_d2d_friendship_update($friend_id, $friendship_state, $old_friendship_state, $sender_id, $friend) {
  // Friend can be NULL, it is an optional in  d2d_api_friendship_update()
  if (!is_null($friend) && isset($friend['d2d_id'])) {
    d2d_server_create_d2dentity($friend['d2d_id']);
  }
}



/**
 * Implements hook_d2d_handle_incoming_request().
 */
function d2d_server_d2d_handle_incoming_request_preprocess() {
  $types['d2d_friendship'] = array(
    'callback' => 'd2d_server_friendship_incoming_preprocess',
  );
  return $types;
}

/**
 * D2D callback; processes a request to change the friendship state.
 *
 * @param array $exploded_arguments
 *   Associative array specifying the desired new friendship state.
 *   $arguments['state'] is TRUE if and only if the other instance
 *   wants to be friends and FALSE otherwise.
 *
 * @param array &$rpc_info
 *   Associative array containing general information about the request.
 *
 * @param string $encrypted_data
 *   The original encrypted data.
 *
 * @param string $signature
 *   The signature attached to the message.
 *
 * @param string &$error_string
 *   A string to be filled with the error, in case there is one.
 *
 * @return bool
 *   TRUE on success.
 */
function d2d_server_friendship_incoming_preprocess($exploded_arguments, &$rpc_info, $encrypted_data, $signature, &$error_string) {
  // Fetch more info about the instance
  $res = xmlrpc($rpc_info['url'], array('d2d.getInstanceInformation' => array()));
  if ($res === FALSE) {
    $error_string = 'Error retrieving instance information. Friendship refused.';
    return FALSE;
  }
  watchdog('d2d_server', 'ahah1');
  $exploded_res = d2d_explode($res);
  if ($exploded_res === FALSE || !d2d_check_array($exploded_res, array(
    'd2d_id' => 'd2d_check_d2d_id',
    'name' => 'd2d_check_name',
    'description' => 'd2d_check_description',
  ))) {
    $error_string = 'Failed to provided instance information. Friendship refused.';
    return FALSE;
  }

  if (isset($rpc_info['instance'])) {
    if ($rpc_info['instance']['d2d_id'] !== $exploded_res['d2d_id']) {
      $error_string = 'd2d_id mismatch. Is this a forgery attempt? Friendship request refused.';
      return FALSE;
    }
  }

  $ok = d2d_api_instance_update($rpc_info['instance']['id'], array(
    'name' => $exploded_res['name'],
    'description' =>$exploded_res['description'],
  ));

  if (!$ok) {
    // Not a fatal error.
    watchdog('d2d_server', t('Failing to update instance on d2d_server_friendship_incoming_preprocess hook.'));
  }
  else {
    $rpc_info['instance']['name'] = $exploded_res['name'];
    $rpc_info['instance']['description'] = $exploded_res['description'];
  }
  return TRUE;
}