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