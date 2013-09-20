<?php
/**
 * @file
 */

/**
 * find tag's tid by tag's name
 * operate taxonomy_term_data table.
 */
function _d2d_server_findtagtid_by_tagname($tagname) {
  $split_strings = preg_split('/[\,]+/', $tagname);

  $or = db_or();
  foreach ($split_strings as $value) {
    $or->condition('name', trim($value));
  }

  $query = db_select('taxonomy_term_data', 'ttd')
    ->fields('ttd', array('tid'))
    ->condition($or)
    ->execute()
    ->fetchAllAssoc('tid');
  return $query;
}

/**
 * get an entity's all taxonomy fields' name.
 */
function _d2d_server_get_all_taxonomyfields_name($entity_type) {

  //get all tags field for patterns_server.
  $tags_table_db = array();
  $fields_info = field_info_instances($entity_type);
  if (isset($fields_info[$entity_type]) && count($fields_info[$entity_type])) {
    $fields_info = $fields_info[$entity_type];
    foreach ($fields_info as $field_name => $_value) {
      $field_info = field_info_field($field_name);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $tags_table_db[] = $field_name;
      }
    }
  }
  if (!count($tags_table_db)) {
    return FALSE;
  }
  else {
    return $tags_table_db;
  }
}
