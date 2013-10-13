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
 */
function d2d_server_get_d2dentity_by_d2did($d2did) {
  $q = db_select("d2d_entity", 'de')
    ->fields('de', array('did'))
    ->condition('d2did', $d2did)
    ->execute()
    ->fetchObject();
  if ($q === FALSE) {
    return FALSE;
  }
  $de = entity_load('d2d_entity', array($q->did));
  return $de[$q->did];
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

/**
 * set site title and site slogan by d2did.
 */
function d2d_server_set_site_info_by_d2did($d2did, $site_info) {
  db_update("d2d_entity")
    ->fields( array(
      'site_name' => $site_info['name'],
      'site_slogan' => $site_info['slogan'],
      'infofetched' => REQUEST_TIME,
      ))
    ->condition('d2did', $d2did)
    ->execute();
}

/**
 * set nodes number by d2did.
 */
function d2d_server_set_nodes_number_by_d2did($d2did, $nodes_number) {
  db_update("d2d_entity")
    ->fields( array(
      'nodes_number' => $nodes_number,
      'infofetched' => REQUEST_TIME,
      ))
    ->condition('d2did', $d2did)
    ->execute();
}

/**
 * set tags by d2did.
 */
function d2d_server_set_tags_by_d2did($d2did, $tags) {

  //get the first taxonomy field of d2d_entity and the vocabulary used by the field.
  $has_tags_field = FALSE;
  $field_info;
  $fields_info = field_info_instances('d2d_entity');
  if (isset($fields_info['d2d_entity']) && count($fields_info['d2d_entity'])) {
    $fields_info = $fields_info['d2d_entity'];
    foreach ($fields_info as $field_name => $_value) {
      $field_info = field_info_field($field_name);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $has_tags_field = TRUE;
        break;
      }
    }
  }
  if (!$has_tags_field) {
    drupal_set_message(t("Tags '%tags' has been fetched, but they cannot be stored because no tags field is currently associated with the d2d instance entity.", array('%tags' => $tags)), "warning");
    return;
  }
  $vocabulary = $field_info['settings']['allowed_values'][0]['vocabulary'];
  $v_obj = taxonomy_vocabulary_machine_name_load($vocabulary);
  $terms = d2d_server_get_taxonomy_terms_byvid($v_obj->vid);
  //$terms = array_keys($terms);

  //get the tags of client.
  $split_strings = preg_split('/[\,]+/', $tags);
  $tags_clean = array();
  foreach ($split_strings as $value) {
    $value = trim($value);
    if (!empty($value)) {
      $tags_clean[] = $value;
    }
  }

  $tags_clean = array_unique($tags_clean);

  $tids = array();
  foreach ($tags_clean as $tag) {
    $tag_exist = FALSE;
    foreach ($terms as $term) {
      if ($tag == $term->name) {
        $tids[] = $term->tid;
        $tag_exist = TRUE;
        break;
      }
    }
    if (!$tag_exist) {
      $tm = new stdclass;
      $tm->vid = $v_obj->vid;
      $tm->name = $tag;
      taxonomy_term_save($tm);
      $tids[] = $tm->tid;
    }
  }

  $de = d2d_server_get_d2dentity_by_d2did($d2did);

  global $language;

  $f_name = $field_info['field_name'];
  $fields = array($field_info['id']);

  $temp = $de->$f_name;
  $cn = 0;
  if (isset($temp['und'])) {
    foreach ($temp['und'] as $key => $value) {
      $index = array_search($value['tid'], $tids);
      if ($index !== FALSE) {
        unset($tids[$index]);
      }
    }
    $cn = count($temp['und']);
  }

  foreach ($tids as $tid) {
    $temp['und'][$cn++]['tid'] = $tid;
  }

  $tags_limit = $field_info['cardinality'];
  if ($cn > $tags_limit && $tags_limit != -1) {
    drupal_set_message(t("Only %tags_limit tags saved because of your tags field setting.", array('%tags_limit' => $tags_limit)), 'warning');
  }

  $de->$f_name = $temp;
  field_sql_storage_field_storage_write('d2d_entity', $de, 'update', $fields);
  cache_clear_all("field:d2d_entity:$de->did", "cache_field");


  //foreach ($tags_clean as $tag) {
  //  foreach ($terms as $obj) {
  //    if ($tag == $obj->name) {
  //      //check whether existed or not.
  //      }
  //      break;
  //    }
  //  }
  //  //store new items.
  //
  //}

}


/**
 */
function d2d_server_get_taxonomy_terms_byvid($vid) {
  $q = db_select("taxonomy_term_data", 't')
    ->fields('t', array('tid', 'name'))
    ->condition('vid', $vid)
    ->execute()
    ->fetchAll();
  return $q;
}

/**
 */
function d2d_server_save_actions_form_clients($action_a) {
  db_insert('d2d_server_actions_from_clients')
    ->fields(array(
      'from_d2did' => $action_a['from'],
      'from_url' => $action_a['from_url'],
      'to_d2did' => $action_a['to'],
      'to_url' => $action_a['to_url'],
      'action_type' => $action_a['type'],
      'action_length' => $action_a['len'],
      'created' => REQUEST_TIME,
      ))
    ->execute();
}
