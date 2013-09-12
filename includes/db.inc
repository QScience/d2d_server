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
function d2d_server_import_instance_byd2did($d2did) {
  $q = db_select("d2d_instances", "di")
    ->fields("di", array())
    ->condition("d2d_id", $d2did)
    ->execute()
    ->fetchObject();
  //dsm($q);
  db_insert("d2d_entity")
    ->fields( array(
        'd2did' => $q->d2d_id,
        'title' => $q->name,
        'description' => $q->description,
        'category' => 'test',
        'users_number' => -1,
        'created' => REQUEST_TIME,
      ))
    ->execute();
}
