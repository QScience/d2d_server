<?php
/**
 * @file
 * function operate on d2d tables
 */

/**
 * get all new friends
 */
function d2d_server_get_instances_newfriends() {
  $q = db_select("d2d_instances", "di")
    ->fields("di", array())
    ->condition("friendship_state", 3)
    ->execute()
    ->fetchAllAssoc('d2d_id');
  //dsm($q);

  $p = db_select("d2d_entity", 'de')
    ->fields('de', array('d2did'))
    ->execute()
    ->fetchAllAssoc('d2did');
  $p = array_keys($p);
  $results = array();
  foreach ($q as $key => $value) {
    if (!in_array($key, $p)) {
      $results[$key] = $value;
    }
  }
  //dsm($results);
  return $results;
}

/**
 * get a instances from d2d id
 */
function d2d_server_get_instance_by_d2did($d2did) {
  $q = db_select("d2d_instances", "di")
    ->fields("di", array())
    ->condition("d2d_id", $d2did)
    ->execute()
    ->fetchObject();
  return $q;
}

/**
 * update instance
 */
function d2d_server_update_instance($en) {
  db_update("d2d_entity")
    ->fields( array(
        'title' => $en['title'],
        'description' => $en['description'],
        'category' => $en['category'],
        'users_number' => -1,
        'updated' => REQUEST_TIME,
      ))
    ->condition('d2did', $en['d2did'])
    ->execute();
}

/**
 * get url field by d2did.
 */
function d2d_server_get_url_by_d2did($d2did) {
  $q = db_select("d2d_entity", 'de') 
    ->fields('de', array('url'))
    ->condition('d2did', $d2did)
    ->execute()
    ->fetchObject();
  return $q;
}

/**
 * set users_number by d2did.
 */
function d2d_server_set_users_number_by_d2did($d2did, $users_number) {
  db_update("d2d_entity")
    ->fields( array(
      'users_number' => $users_number,
      'infofetched' => REQUEST_TIME,
      ))
    ->condition('d2did', $d2did)
    ->execute();
}
